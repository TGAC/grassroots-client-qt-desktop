/*
 ** Copyright 2014-2016 The Earlham Institute
 **
 ** Licensed under the Apache License, Version 2.0 (the "License");
 ** you may not use this file except in compliance with the License.
 ** You may obtain a copy of the License at
 **
 **     http://www.apache.org/licenses/LICENSE-2.0
 **
 ** Unless required by applicable law or agreed to in writing, software
 ** distributed under the License is distributed on an "AS IS" BASIS,
 ** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 ** See the License for the specific language governing permissions and
 ** limitations under the License.
 */
#include <QLabel>
#include <QDesktopServices>

#include <QHBoxLayout>
#include <QMessageBox>

#include "qt_parameter_widget.h"
#include "file_chooser_widget.h"


#include "param_char_edit.h"
#include "param_check_box.h"
#include "param_date_widget.h"
#include "param_double_spin_box.h"
#include "signed_int_param_spin_box.h"
#include "unsigned_int_param_spin_box.h"
#include "base_combo_box.h"
#include "param_char_edit.h"
#include "param_line_edit.h"
#include "param_text_box.h"
#include "prefs_widget.h"
#include "string_table_widget.h"
#include "param_json_editor.h"
#include "json_table_widget.h"

#include "string_array_parameter.h"
#include "time_array_parameter.h"

#include "double_combo_box.h"
#include "signed_int_combo_box.h"
#include "string_combo_box.h"
#include "unsigned_int_combo_box.h"


#include "qt_client_data.h"

// WHEATIS INCLUDES
#include "parameter.h"
#include "parameter_set.h"
#include "parameter_group.h"
#include "string_utils.h"
#include "provider.h"
#include "json_tools.h"
#include "service.h"

const int QTParameterWidget :: QPW_NUM_COLUMNS = 2;


static bool AddNonRepeatedParams(const Parameter *param_p, void *data_p);



QTParameterWidget :: QTParameterWidget (const char *name_s, const char * const description_s, const char * const uri_s, const json_t *provider_p, ParameterSet *parameters_p, ServiceMetadata *metadata_p, const ServicePrefsWidget * const prefs_widget_p, const ParameterLevel initial_level, const QTClientData *client_data_p)
:	qpw_params_p (parameters_p),
	qpw_parent_prefs_widget_p (prefs_widget_p),
	qpw_widgets_map (QHash <Parameter *, BaseParamWidget *> ()),
	qpw_level (initial_level),
	qpw_client_data_p (client_data_p)
{
	QVBoxLayout *layout_p = new QVBoxLayout;
	QVBoxLayout *info_layout_p = new QVBoxLayout;

	/*
	qpw_layout_p = new QGridLayout;
	qpw_layout_p -> setColumnStretch (1, 100);
	qpw_layout_p -> setAlignment (Qt :: AlignVCenter);
	 */
	qpw_layout_p = new QFormLayout;
	qpw_layout_p -> setFieldGrowthPolicy(QFormLayout :: ExpandingFieldsGrow);
	layout_p -> addLayout (info_layout_p);

	setLayout (layout_p);

	QString str;

	str.append ("<b>");
	str.append (name_s);
	str.append ("</b>");

	QLabel *label_p = new QLabel (str, this);
	label_p -> setSizePolicy (QSizePolicy :: Fixed, QSizePolicy :: Fixed);
	info_layout_p -> addWidget (label_p);


	str.clear ();
	str.append (description_s);

	label_p = new QLabel (str, this);
	label_p -> setSizePolicy (QSizePolicy :: Fixed, QSizePolicy :: Fixed);
	info_layout_p -> addWidget (label_p);

	if (uri_s)
		{
			str.clear ();
			str.append ("For more information, go to <a href=\"");
			str.append (uri_s);
			str.append ("\">");
			str.append (uri_s);
			str.append ("</a>");

			label_p = new QLabel (str, this);
			connect (label_p,  &QLabel :: linkActivated, this, &QTParameterWidget :: OpenLink);

			label_p -> setSizePolicy (QSizePolicy :: Fixed, QSizePolicy :: Fixed);
			label_p -> setAlignment (Qt :: AlignCenter);
			info_layout_p -> addWidget (label_p);
		}

	if (provider_p)
		{
			if (json_is_array (provider_p))
				{
					size_t size = json_array_size (provider_p);
					size_t i;
					json_t *item_p;

					json_array_foreach (provider_p, i, item_p)
					{
						AddProvider (item_p, i, size - 1, info_layout_p);
					}
				}		/* if (json_is_array (provider_p)) */
			else if (json_is_object (provider_p))
				{
					AddProvider (provider_p, 0, 0, info_layout_p);
				}

		}		/* if (provider_p) */


	if (metadata_p)
		{
			AddServiceMetadata (metadata_p, info_layout_p);
		}		/* if (metadata_p) */

	QFrame *line_p = new QFrame;
	line_p -> setFrameShape (QFrame :: HLine);
	QPalette palette = line_p -> palette ();
	palette.setColor (QPalette :: WindowText, Qt :: gray);
	line_p -> setPalette (palette);

	layout_p -> addWidget (line_p);

	layout_p -> insertSpacing (layout_p -> count (), 10);
	layout_p -> addLayout (qpw_layout_p);


	qpw_refresh_active = false;

	if (qpw_params_p)
		{
			AddParameters (qpw_params_p);
		}		/* if (parameters_p) */

	qpw_refresh_active = true;

}


void QTParameterWidget :: AddServiceMetadata (const ServiceMetadata *metadata_p, QBoxLayout *layout_p)
{
	QFormLayout *metadata_layout_p = new QFormLayout;

	if (metadata_p -> sm_application_category_p)
		{
			if (!AddLabelForSchemaTerm (metadata_p -> sm_application_category_p, "Category:", metadata_layout_p))
				{

				}
		}

	if (metadata_p -> sm_application_subcategory_p)
		{
			if (!AddLabelForSchemaTerm (metadata_p -> sm_application_subcategory_p, "SubCategory:", metadata_layout_p))
				{

				}
		}

	if (metadata_p -> sm_input_types_p)
		{
			if (!AddListForSchemaTerms (metadata_p -> sm_input_types_p, "Input:", metadata_layout_p))
				{

				}
		}

	if (metadata_p -> sm_output_types_p)
		{
			if (!AddListForSchemaTerms (metadata_p -> sm_output_types_p, "Output:", metadata_layout_p))
				{

				}
		}


	layout_p -> addLayout (metadata_layout_p);
}


bool QTParameterWidget :: AddListForSchemaTerms (const LinkedList *terms_p, const char * const key_s, QFormLayout *layout_p)
{
	QVBoxLayout *list_layout_p = new QVBoxLayout;
	bool success_flag = true;
	SchemaTermNode *node_p = reinterpret_cast <SchemaTermNode *> (terms_p -> ll_head_p);

	while (node_p && success_flag)
		{
			const SchemaTerm *term_p = node_p -> stn_term_p;

			QString str;
			char *url_s = GetExpandedContextTerm (term_p -> st_url_s);

			str.append ("<a href=\"");

			if (url_s)
				{
					str.append (url_s);
					FreeCopiedString (url_s);
				}
			else
				{
					str.append (term_p -> st_url_s);
				}

			str.append ("\">");

			str.append (term_p -> st_name_s);
			str.append ("</a>");

			QLabel *label_p = new QLabel (str, this);

			if (label_p)
				{
					if (term_p -> st_description_s)
						{
							label_p -> setToolTip (term_p -> st_description_s);
						}

					list_layout_p -> addWidget (label_p);
					connect (label_p,  &QLabel :: linkActivated, this, &QTParameterWidget :: OpenLink);
				}
			else
				{
					success_flag = false;
				}

			node_p = reinterpret_cast <SchemaTermNode *> (node_p -> stn_node.ln_next_p);
		}


	if (success_flag)
		{
			layout_p -> addRow (new QLabel (key_s), list_layout_p);
		}
	else
		{
			delete list_layout_p;
		}


	return success_flag;
}


bool QTParameterWidget :: AddLabelForSchemaTerm (const SchemaTerm *term_p, const char * const key_s, QFormLayout *layout_p)
{
	bool success_flag = false;
	QString str;

	str.append ("<a href=\"");

	char *url_s = GetExpandedContextTerm (term_p -> st_url_s);

	if (url_s)
		{
			str.append (url_s);
			FreeCopiedString (url_s);
		}
	else
		{
			str.append (term_p -> st_url_s);
		}
	str.append ("\">");

	str.append (term_p -> st_name_s);
	str.append ("</a>");

	QLabel *label_p = new QLabel (str, this);

	if (label_p)
		{
			if (term_p -> st_description_s)
				{
					label_p -> setToolTip (term_p -> st_description_s);
				}

			layout_p -> addRow (new QLabel (key_s), label_p);
			connect (label_p,  &QLabel :: linkActivated, this, &QTParameterWidget :: OpenLink);
			success_flag = true;
		}

	return success_flag;
}


void QTParameterWidget :: AddProvider (const json_t *provider_p, const size_t i, const size_t last_index, QVBoxLayout *info_layout_p)
{
	QLabel *logo_p = nullptr;
	QLabel *text_p = nullptr;

	const char *provider_logo_s = GetProviderLogo (provider_p);

	if (provider_logo_s)
		{
			CurlTool *curl_tool_p = AllocateMemoryCurlTool (0);

			if (curl_tool_p)
				{
					if (SetUriForCurlTool (curl_tool_p, provider_logo_s))
						{
							CURLcode res;

							SetCurlToolTimeout (curl_tool_p, 5L);

							res = RunCurlTool (curl_tool_p);

							if (res == CURLE_OK)
								{
									size_t length = GetCurlToolDataSize (curl_tool_p);
									const uchar *data_p = reinterpret_cast <const uchar *> (GetCurlToolData (curl_tool_p));

									QPixmap pix;

									if (pix.loadFromData (data_p, length))
										{
											logo_p = new QLabel;

											//	logo_p -> setAlignment (Qt :: AlignRight);
											logo_p -> setPixmap (pix);
										}
								}
						}

					FreeCurlTool (curl_tool_p);
				}
		}

	const char *provider_name_s = GetProviderName (provider_p);

	if (provider_name_s)
		{
			const char *provider_uri_s = GetProviderURI (provider_p);
			QString str;

			if (i == 0)
				{
					str.append ("Provided by ");
				}
			else if (i < last_index)
				{
					str.append (", ");
				}
			else if (i == last_index)
				{
					str.append ("and ");
				}

			if (provider_uri_s)
				{
					str.append ("<a href=\"");
					str.append (provider_uri_s);
					str.append ("\">");
				}

			str.append (provider_name_s);

			if (provider_uri_s)
				{
					str.append ("</a>");
				}

			const char *provider_description_s = GetProviderDescription (provider_p);

			if (provider_description_s)
				{
					str.append (", ");
					str.append (provider_description_s);

					if (i < last_index)
						{
							str.append (", ");
						}
					else
						{
							str.append (".");
						}

					text_p = new QLabel (str);
					//label_p -> setWordWrap (true);
					connect (text_p,  &QLabel :: linkActivated, this, &QTParameterWidget :: OpenLink);
					text_p -> setSizePolicy (QSizePolicy :: Minimum, QSizePolicy :: Fixed);
					text_p -> setAlignment (Qt :: AlignLeft);
					//label_p -> setFrameShape (QFrame :: Box);


					if (logo_p)
						{
							QHBoxLayout *layout_p = new QHBoxLayout;

							layout_p -> addWidget (text_p);
							layout_p -> addWidget (logo_p);
							info_layout_p -> addLayout (layout_p);
						}
					else
						{
							info_layout_p -> addWidget (text_p);
						}
				}

		}



}



void QTParameterWidget :: OpenLink (const QString &link_r)
{
	if (!QDesktopServices :: openUrl (QUrl (link_r)))
		{
			QWebEngineView *browser_p = new QWebEngineView;

			qpw_browsers.append (browser_p);
			browser_p -> load (QUrl (link_r));
			browser_p -> show ();
		}
}

const QTClientData *QTParameterWidget :: GetClientData () const
{
	return qpw_client_data_p;
}



void QTParameterWidget :: AddRow (QWidget *first_p, QWidget *second_p, const int row_span)
{
	int row = qpw_layout_p -> rowCount ();

	qpw_layout_p -> addRow (first_p, second_p);
}


void QTParameterWidget :: ParameterWidgetAdded (Parameter *param_p, BaseParamWidget *widget_p)
{
	qpw_widgets_map.insert (param_p, widget_p);

	PrintLog (STM_LEVEL_INFO, __FILE__, __LINE__, "Added \"%s\"", param_p -> pa_name_s);
}


void QTParameterWidget :: AddParameters (ParameterSet *params_p)
{
	ParameterNode *node_p = reinterpret_cast <ParameterNode *> (params_p -> ps_params_p -> ll_head_p);
	ParameterGroupNode *param_group_node_p = reinterpret_cast <ParameterGroupNode *> (params_p -> ps_grouped_params_p -> ll_head_p);

	while (param_group_node_p)
		{
			QWidget *widget_p = nullptr;

			ParameterGroup *group_p = param_group_node_p -> pgn_param_group_p;

			if (group_p -> pg_repeatable_flag)
				{
					RepeatableParamGroupBox *box_p = new RepeatableParamGroupBox (group_p, this, false, false);

					box_p -> Init (false);
					qpw_repeatable_groupings.insert (group_p -> pg_name_s, box_p);
					widget_p = box_p -> GetWidget ();
				}
			else
				{
					ParamGroupBox *box_p = new ParamGroupBox (group_p, this, false, false);

					box_p -> Init (false);
					qpw_groupings.insert (group_p -> pg_name_s, box_p);
					widget_p = box_p -> GetWidget ();
				}


			int row = qpw_layout_p -> rowCount ();
			qpw_layout_p -> addRow (widget_p);

			param_group_node_p = reinterpret_cast <ParameterGroupNode *> (param_group_node_p -> pgn_node.ln_next_p);
		}

	while (node_p)
		{
			Parameter * const param_p = node_p -> pn_parameter_p;

			if (!qpw_widgets_map.contains (param_p))
				{
					ParameterWidgetContainer *container_p = nullptr;

					if (param_p -> pa_group_p)
						{
							container_p = qpw_groupings.value (param_p -> pa_group_p -> pg_name_s);
						}

					AddParameterWidget (param_p, container_p, false);
				}

			node_p = reinterpret_cast <ParameterNode *> (node_p -> pn_node.ln_next_p);
		}		/* while (node_p) */

}


void QTParameterWidget :: AddParameterWidget (Parameter *param_p, ParameterWidgetContainer *container_p, bool add_params_flag)
{
	BaseParamWidget *child_p = CreateWidgetForParameter (param_p, container_p, add_params_flag);

	if (child_p)
		{
			if (container_p)
				{
					container_p -> AddParameterWidget (child_p);
					//group_p -> setSizePolicy (QSizePolicy :: Fixed, QSizePolicy :: Expanding);
				}
			else
				{
					AddRow (child_p -> GetLabel (), child_p -> GetUIQWidget (), 1);
				}

			ParameterWidgetAdded (param_p, child_p);


			//			if (!CompareParameterLevels (param_p -> pa_level, qpw_level))
			//				{
			//					child_p -> SetVisible (false);
			//				}
		}
}



void QTParameterWidget :: ClearErrors ()
{
	QHash <Parameter *, BaseParamWidget *> :: const_iterator i = qpw_widgets_map.constBegin ();

	while (i != qpw_widgets_map.constEnd ())
		{
			BaseParamWidget *widget_p = i.value ();

			widget_p -> SetErrorMessage (nullptr);
			++ i;
		}
}


BaseParamWidget *QTParameterWidget :: GetWidgetForParameter (const char * const param_name_s) const
{
	BaseParamWidget *widget_p = nullptr;
	Parameter *param_p = GetParameterFromParameterSetByName (qpw_params_p, param_name_s);

	if (param_p)
		{
			widget_p = qpw_widgets_map.value (param_p);

			/*
			QList <Parameter *> keys = qpw_widgets_map.keys ();

			printf ("param_name_s \"%s\"\n", param_name_s);

			for (int i = 0; i < keys.size (); ++ i)
				{
					Parameter *param_p = keys.at (i);
					BaseParamWidget *temp_widget_p = qpw_widgets_map.value (param_p);

					QString s = temp_widget_p -> GetLabel () -> text ();
					QByteArray ba = s.toLocal8Bit ();
					const char *label_s = ba.constData ();

					printf ("name \"%s\" label \"%s\"\n", param_p -> pa_name_s, label_s);
				}

			 */
		}

	return widget_p;
}


QTParameterWidget :: ~QTParameterWidget ()
{
	QList <Parameter *> keys = qpw_widgets_map.keys ();

	while (!keys.isEmpty ())
		{
			Parameter *param_p = keys.takeLast ();

			BaseParamWidget *widget_p = qpw_widgets_map.take (param_p);

			widget_p -> RemoveConnection ();
			delete widget_p;
		}

	for (int i = qpw_browsers.size(); i > 0;-- i)
		{
			QWebEngineView *browser_p = qpw_browsers.back ();
			qpw_browsers.pop_back ();
			delete browser_p;
		}

	if (qpw_params_p)
		{
			FreeParameterSet (qpw_params_p);
		}
}


void QTParameterWidget :: ResetToDefaults ()
{
	QHash <Parameter *, BaseParamWidget *> :: const_iterator i;

	for (i = qpw_widgets_map.constBegin (); i != qpw_widgets_map.constEnd (); ++ i)
		{
			BaseParamWidget *widget_p = i.value ();
			widget_p -> SetDefaultValue ();
		}
}



size_t QTParameterWidget :: GetNumberOfRepeatedValues (QHash <Parameter *, const json_t *> *grouped_params_p)
{
	size_t num_values = 0;

	/*
	 * Get the maximum length
	 */
	QHashIterator <Parameter *, const json_t *> params_itr (*grouped_params_p);

	while (params_itr.hasNext ())
		{
			params_itr.next ();

			const json_t *param_json_p = params_itr.value ();
			const json_t *current_values_p = json_object_get (param_json_p, PARAM_CURRENT_VALUE_S);

			if (current_values_p)
				{
					if (json_is_array (current_values_p))
						{
							const size_t t = json_array_size (current_values_p);

							if (num_values < t)
								{
									num_values = t;
								}
						}
					else
						{
							if (num_values < 1)
								{
									num_values = 1;
								}

						}

				}		/* if (current_values_p) */

		}		/* while (params_itr.hasNext ()) */

	return num_values;
}


bool QTParameterWidget :: SetRepeatableGroupParamValuesFromJSON (const json_t *params_array_json_p, QHash <const json_t *, const json_t *> *repeatable_param_names_p)
{
	bool success_flag = false;
	QHashIterator <const char *, RepeatableParamGroupBox *> groups_itr (qpw_repeatable_groupings);

	while (groups_itr.hasNext ())
		{
			groups_itr.next ();

			const char *group_name_s = groups_itr.key ();
			RepeatableParamGroupBox *box_p = groups_itr.value ();
			const ParameterGroup *group_p = box_p -> GetParameterGroup ();

			/*
			 * Get all of the Parameters in this group
			 */
			QHash <Parameter *, const json_t *> grouped_params;
			QHash <Parameter *, Parameter *> non_label_params;
			const json_t *param_json_p;
			size_t i;
			size_t num_params;

			json_array_foreach (params_array_json_p, i, param_json_p)
			{
				const char *param_group_s = GetJSONString (param_json_p, PARAM_GROUP_S);

				if (param_group_s && (strcmp (group_name_s, param_group_s) == 0))
					{
						const char *param_name_s = GetJSONString (param_json_p, PARAM_NAME_S);

						if (param_name_s)
							{
								Parameter *param_p = GetParameterFromParameterSetByName (qpw_params_p, param_name_s);

								if (param_p)
									{
										grouped_params.insert (param_p, param_json_p);
										non_label_params.insert (param_p, param_p);

										repeatable_param_names_p -> insert (param_json_p, param_json_p);
									}
							}
					}		/* if (param_group_s && (strcmp (group_name_s, param_group_s) == 0)) */

			}		/* json_array_foreach (params_array_json_p, j, param_json_p) */

			/*
			 * Now remove the non label params from non_label_params
			 */

			ParameterNode *param_node_p = (ParameterNode *) (group_p -> pg_repeatable_label_params_p -> ll_head_p);

			while (param_node_p)
				{
					non_label_params.remove (param_node_p -> pn_parameter_p);
					param_node_p = (ParameterNode *) (param_node_p -> pn_node.ln_next_p);
				}



			size_t num_values = GetNumberOfRepeatedValues (&grouped_params);

			if ((num_params = grouped_params.size ()) > 0)
				{
					box_p -> ClearList ();

					/*
					 * Build and add the entries to the lists
					 */

					/*
					 * Build the label
					 */
					ByteBuffer *buffer_p = AllocateByteBuffer (1024);

					if (buffer_p)
						{

							for (i = 0; i < num_values; ++ i)
								{
									QHashIterator <Parameter *, const json_t *> params_itr (grouped_params);
									bool success_flag = true;
									char *label_s = NULL;
									const ParameterGroup *group_p = box_p -> GetParameterGroup ();
									ParameterNode *param_node_p = (ParameterNode *) (group_p -> pg_repeatable_label_params_p -> ll_head_p);

									/* do the label params first */
									while (param_node_p)
										{
											Parameter *param_p = param_node_p -> pn_parameter_p;
											const json_t *param_json_p = grouped_params.value (param_p);

											SetGroupedParameterValue (param_p, param_json_p, i, buffer_p);

											param_node_p = (ParameterNode *) (param_node_p -> pn_node.ln_next_p);
										}		/* while (param_node_p) */

									/* now do the other params */
									QHash <Parameter *, Parameter *> :: iterator itr;
									for (itr = non_label_params.begin (); itr != non_label_params.end (); ++ itr)
										{
											Parameter *param_p = itr.key ();
											const json_t *param_json_p = grouped_params.value (param_p);

											SetGroupedParameterValue (param_p, param_json_p, i, nullptr);
										}

									if (success_flag)
										{
											const SchemaVersion *sv_p = qpw_client_data_p -> qcd_base_data.cd_schema_p;

											json_t *group_json_p = GetParameterGroupAsJSON (box_p -> GetParameterGroup (), true, false, sv_p);

											if (group_json_p)
												{
													if (qpw_client_data_p -> qcd_verbose_flag)
														{
															PrintJSONToLog (STM_LEVEL_INFO, __FILE__, __LINE__, group_json_p, "Adding \"%s\" to list\n", label_s);
														}

													box_p -> AddListEntry (GetByteBufferData (buffer_p), group_json_p);
													json_decref (group_json_p);
												}
										}

									ResetByteBuffer (buffer_p);



								}		/* for (i = 0; i < num_values; ++ i) */

							FreeByteBuffer (buffer_p);
						}		/* if (buffer_p) */

				}		/* if ((num_params = params.size ()) > 0) */

		}		/* while (i.hasNext ()) */


	return success_flag;
}


bool QTParameterWidget :: SetGroupedParameterValue (Parameter *param_p, const json_t *param_json_p , const size_t index, ByteBuffer *label_buffer_p)
{
	bool success_flag = true;

	if (param_json_p)
		{
			const json_t *current_values_p = json_object_get (param_json_p, PARAM_CURRENT_VALUE_S);
			const json_t *entry_p = NULL;

			if (current_values_p)
				{
					if (json_is_array (current_values_p))
						{
							if (index < json_array_size (current_values_p))
								{
									entry_p = json_array_get (current_values_p, index);
								}

						}
					else if ((index == 0) && (! (json_is_null (current_values_p))))
						{
							entry_p = current_values_p;
						}

					if (entry_p)
						{
							if (json_is_string (entry_p))
								{
									if (label_buffer_p)
										{
											const char *value_s = json_string_value (entry_p);
											AppendStringsToByteBuffer (label_buffer_p, value_s, " ", NULL);
										}
								}

							if (qpw_client_data_p -> qcd_verbose_flag)
								{
									PrintJSONToLog (STM_LEVEL_FINE, __FILE__, __LINE__, entry_p, "param \"%s\" entry " SIZET_FMT "\n", param_p -> pa_name_s, index);
								}

							success_flag = SetParameterCurrentValueFromJSON (param_p, entry_p);
						}
					else
						{
							if (qpw_client_data_p -> qcd_verbose_flag)
								{
									PrintLog (STM_LEVEL_FINE, __FILE__, __LINE__, "param \"%s\" NULL entry " SIZET_FMT "\n", param_p -> pa_name_s, index);
								}

							success_flag = SetParameterCurrentValueFromJSON (param_p, json_null ());
						}

				}		/* if (current_values_p) */

		}		/* if (param_json_p) */

	return success_flag;
}



bool QTParameterWidget :: SetParamValuesFromJSON (const json_t *param_set_json_p)
{
	bool success_flag = false;
	const char *service_name_s = qpw_parent_prefs_widget_p -> GetServiceName ();
	ParameterSet *new_params_p = CreateParameterSetFromJSON (param_set_json_p, NULL, false);

	if (new_params_p)
		{
			ParameterNode *param_node_p = (ParameterNode *) (new_params_p -> ps_params_p -> ll_head_p);

			while (param_node_p)
				{
					Parameter *new_param_p = param_node_p -> pn_parameter_p;
					Parameter *existing_param_p = GetParameterFromParameterSetByName (qpw_params_p, new_param_p -> pa_name_s);

					if (existing_param_p)
						{
							bool reuse_flag = false;
							BaseParamWidget *widget_p = GetWidgetForParameter (existing_param_p -> pa_name_s);

							if (widget_p)
								{
									/*
									 * Can we reuse the existing widget?
									 */
									if (existing_param_p -> pa_type == new_param_p -> pa_type)
										{
											if (widget_p -> IsComboBoxWidget ())
												{
													if (new_param_p -> pa_options_p)
														{
															reuse_flag = true;
														}
												}
											else
												{
													if (! (new_param_p -> pa_options_p))
														{
															reuse_flag = true;
														}
												}
										}

									if (reuse_flag)
										{
											widget_p -> SetParameter (new_param_p);
										}
									else
										{
											PrintLog (STM_LEVEL_SEVERE, __FILE__, __LINE__, "cannot reuse param \"%s\"\n", new_param_p -> pa_name_s);
										}
								}
							else
								{
									PrintLog (STM_LEVEL_SEVERE, __FILE__, __LINE__, "no widget for param \"%s\"\n", new_param_p -> pa_name_s);
								}

						}		/* if (existing_param_p) */

					param_node_p = (ParameterNode *) (param_node_p -> pn_node.ln_next_p);
				}		/* while (param_node_p) */


		}		/* if (new_params_p) */

	return success_flag;
}


//bool QTParameterWidget :: SetParamValuesFromJSON (const json_t *param_set_json_p)
//{
//	bool success_flag = false;
//	const char *service_name_s = qpw_parent_prefs_widget_p -> GetServiceName ();
//	const json_t *params_json_p = json_object_get (param_set_json_p, PARAM_SET_PARAMS_S);
//
//	ParameterSet *new_params_p = CreateParameterSetFromJSON (param_set_json_p, NULL, false);
//
//
//
//	if (params_json_p)
//		{
//			if (json_is_array (params_json_p))
//				{
//					json_t *param_json_p;
//					size_t i;
//					QHash <const json_t *, const json_t *> repeatable_param_names;
//					success_flag = true;
//
//					SetRepeatableGroupParamValuesFromJSON (params_json_p, &repeatable_param_names);
//
//
//					json_array_foreach (params_json_p, i, param_json_p)
//					{
//						Parameter *param_p = CreateParameterFromJSON (param_json_p, NULL, false);
//
//						if (param_p)
//							{
//								BaseParamWidget *widget_p = GetWidgetForParameter (param_p -> pa_name_s);
//
//								if (widget_p)
//									{
//										json_t *current_value_p = json_object_get (param_json_p, PARAM_CURRENT_VALUE_S);
//										const Parameter *existing_param_p = widget_p -> GetParameter ();
//										bool reuse_flag = false;
//										/*
//										 * Can we reuse the existing widget?
//										 */
//										if (existing_param_p -> pa_type == param_p -> pa_type)
//											{
//												if (widget_p -> IsComboBoxWidget ())
//													{
//														if (param_p -> pa_options_p)
//															{
//																reuse_flag = true;
//															}
//													}
//												else
//													{
//														if (!param_p -> pa_options_p)
//															{
//																reuse_flag = true;
//															}
//													}
//											}
//
//										if (!reuse_flag)
//											{
//												ParameterWidgetContainer *container_p = nullptr;
//
//												if (existing_param_p -> pa_group_p)
//													{
//
//														container_p = qpw_groupings.value (existing_param_p -> pa_group_p -> pg_name_s);
//													}
//
//												BaseParamWidget *new_widget_p = CreateWidgetForParameter (param_p, container_p, false);
//
//												PrintLog (STM_LEVEL_INFO, __FILE__, __LINE__,  "using new widget for %s is at row %d", param_p -> pa_name_s);
//
//												if (new_widget_p)
//													{
//														if (container_p)
//															{
//																QWidget *old_ui_widget_p = widget_p -> GetUIQWidget ();
//
//																if (container_p -> ReplaceWidget (old_ui_widget_p, new_widget_p))
//																	{
//																		widget_p = new_widget_p;
//																	}
//															}
//														else
//															{
//																int existing_row = -1;
//																QWidget *new_ui_widget_p = new_widget_p -> GetUIQWidget ();
//																QFormLayout :: ItemRole role;
//
//																qpw_layout_p -> getWidgetPosition (widget_p -> GetLabel (), &existing_row, &role);
//
//
//																PrintLog (STM_LEVEL_INFO, __FILE__, __LINE__,  "%s is at row %d", param_p -> pa_name_s, existing_row);
//
//																if (existing_row != -1)
//																	{
//																		PrintLog (STM_LEVEL_INFO, __FILE__, __LINE__,  "removing %s at row %d", param_p -> pa_name_s, existing_row);
//																		qpw_layout_p -> removeRow (existing_row);
//																		PrintLog (STM_LEVEL_INFO, __FILE__, __LINE__,  "inserting new row %s at row %d", param_p -> pa_name_s, existing_row);
//																		qpw_layout_p -> insertRow (existing_row, widget_p -> GetLabel (), new_ui_widget_p) ;
//																		PrintLog (STM_LEVEL_INFO, __FILE__, __LINE__,  "inserted new row %s at row %d", param_p -> pa_name_s, existing_row);
//																		widget_p = new_widget_p;
//
//																	}
//																else
//																	{
//																		PrintErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__,  "Failed to get existng row for %s", param_p -> pa_name_s);
//																	}
//															}
//													}
//
//											}
//
//										if (! (widget_p -> SetValueFromJSON (current_value_p)))
//											{
//												success_flag = false;
//												PrintJSONToErrors (STM_LEVEL_WARNING, __FILE__, __LINE__, param_json_p, "Failed to set %s -> %s from json\n", service_name_s, param_p -> pa_name_s);
//											}
//									}		/* if (widget_p) */
//								else
//									{
//										PrintJSONToErrors (STM_LEVEL_WARNING, __FILE__, __LINE__, param_json_p, "Failed to get widget for %s -> %s from json\n", service_name_s, param_p -> pa_name_s);
//									}
//
//							}		/* if (param_p) */
//						else
//							{
//								PrintJSONToErrors (STM_LEVEL_WARNING, __FILE__, __LINE__, param_json_p, "Failed to Create parameter %d from json for %s\n", i, service_name_s);
//							}
//
//					}		/* json_array_foreach (params_json_p, i, param_p) */
//
//				}		/* if (json_is_array (params_json_p)) */
//
//		}
//	else
//		{
//			PrintJSONToErrors (STM_LEVEL_WARNING, __FILE__, __LINE__, param_set_json_p, "Failed to get child %s from json", PARAM_SET_PARAMS_S);
//		}
//
//	return success_flag;
//}



void QTParameterWidget :: RefreshService ()
{
	/* Is the widget live or still adding its widgets? */
	if (qpw_refresh_active)
		{
			qpw_refresh_active = false;
			json_t *params_json_p = GetServiceParamsAsJSON (false, qpw_params_p -> ps_current_level);

			if (params_json_p)
				{
					bool params_json_added_flag = false;
					json_t *req_p = json_array ();

					if (req_p)
						{
							const SchemaVersion *sv_p = qpw_client_data_p -> qcd_base_data.cd_schema_p;
							json_t *service_req_p = GetServiceRefreshRequestFromJSON (qpw_parent_prefs_widget_p -> GetServiceName (), params_json_p, sv_p, true, PL_ALL);

							if (service_req_p)
								{
									params_json_added_flag  = true;

									if (json_array_append_new (req_p, service_req_p) == 0)
										{
											if (qpw_client_data_p->qcd_verbose_flag)
												{
													PrintJSONToLog (STM_LEVEL_INFO, __FILE__, __LINE__, req_p, "Sending: ");
												}

											setCursor (Qt :: BusyCursor);
											json_t *results_p = CallServices (req_p, nullptr, qpw_client_data_p -> qcd_base_data.cd_connection_p);
											setCursor (Qt :: ArrowCursor);

											if (results_p)
												{
													if (qpw_client_data_p->qcd_verbose_flag)
														{
															PrintJSONToLog (STM_LEVEL_INFO, __FILE__, __LINE__, results_p, "Received: ");
														}

													const char *service_name_s = qpw_parent_prefs_widget_p -> GetServiceName ();

													if (service_name_s)
														{
															const json_t *services_json_p = json_object_get (results_p, SERVICES_NAME_S);

															if (services_json_p)
																{
																	if (json_is_array (services_json_p))
																		{
																			size_t i;
																			const size_t num_services = json_array_size (services_json_p);

																			for (i = 0; i < num_services; ++ i)
																				{
																					const json_t *service_json_p = json_array_get (services_json_p, i);
																					const char *name_s = GetJSONString (service_json_p, SERVICE_NAME_S);

																					if (name_s)
																						{
																							if (strcmp (name_s, service_name_s) == 0)
																								{
																									const json_t *op_p = json_object_get (service_json_p, OPERATION_S);

																									if (op_p)
																										{
																											SetParamValuesFromJSON (op_p);
																										}
																								}
																						}

																				}		/* for (i = 0; i < num_services; ++ i) */
																		}
																}
														}

												}
											else
												{
													QMessageBox :: critical (this, "Error refreshing service", "Failed to get updated service values");
												}

										}

								}

						}

					if (!params_json_added_flag)
						{
							json_decref (params_json_p);
						}
				}

			qpw_refresh_active = true;
		}		/* if (qpw_refresh_active) */
}


void QTParameterWidget :: UpdateParameterLevel (const ParameterLevel level, const QWidget * const parent_widget_p)
{
	QHash <Parameter *, BaseParamWidget *> :: const_iterator i;

	qpw_params_p -> ps_current_level = level;

	for (i = qpw_widgets_map.constBegin (); i != qpw_widgets_map.constEnd (); ++ i)
		{
			BaseParamWidget *widget_p = reinterpret_cast <BaseParamWidget *> (i.value ());
			widget_p -> CheckLevelDisplay (level, parent_widget_p);
		}

	QList <ParamGroupBox *> groupings = qpw_groupings.values ();
	for (int i = groupings.count () - 1; i >= 0; -- i)
		{
			ParamGroupBox *container_p = groupings.at (i);
			container_p -> CheckVisibility (level);
		}

	QList <RepeatableParamGroupBox *> repeatable_groupings = qpw_repeatable_groupings.values ();
	for (int i = repeatable_groupings.count () - 1; i >= 0; -- i)
		{
			ParamGroupBox *container_p = repeatable_groupings.at (i);
			container_p -> CheckVisibility (level);
		}

	qpw_level = level;	
}


BaseParamWidget *QTParameterWidget :: CreateWidgetForParameter (Parameter * const param_p,  ParameterWidgetContainer *container_p, bool add_param_flag)
{
	BaseParamWidget *widget_p = nullptr;

	/*
	 * Is the parameter in a repeatable group?
	 */
	//	if ((param_p -> pa_group_p) && (param_p -> pa_group_p -> pg_repeatable_flag))
	//		{
	//			if (IsStringArrayParameter (param_p))
	//				{
	//					widget_p = new ParamLineEdit (reinterpret_cast <StringArrayParameter *> (param_p), this, QLineEdit :: Normal);
	//				}
	//			else if (IsTimeArrayParameter (param_p))
	//				{
	//					widget_p = new ParamDateWidget (reinterpret_cast <TimeArrayParameter *> (param_p), this);
	//				}
	//		}		/* if ((param_p -> pa_group_p) && (param_p -> pa_group_p -> pg_repeatable_flag)) */
	//	else

	{
		if (IsBooleanParameter (param_p))
			{
				widget_p = new ParamCheckBox (reinterpret_cast <BooleanParameter *> (param_p), this);
			}
		else if (IsCharParameter (param_p))
			{
				widget_p = new ParamCharEdit (reinterpret_cast <CharParameter *> (param_p), this, QLineEdit :: Normal);
			}
		else if (IsDoubleParameter (param_p))
			{
				if (param_p -> pa_options_p)
					{
						widget_p = new DoubleComboBox (reinterpret_cast <DoubleParameter *> (param_p), this);
					}
				else
					{
						widget_p = new ParamDoubleSpinBox (reinterpret_cast <DoubleParameter *> (param_p), this);
					}
			}
		else if (IsSignedIntParameter (param_p))
			{
				if (param_p -> pa_options_p)
					{
						widget_p = new SignedIntComboBox (reinterpret_cast <SignedIntParameter *> (param_p), this);
					}
				else
					{
						widget_p = new SignedIntParamSpinBox (reinterpret_cast <SignedIntParameter *> (param_p), this);
					}
			}
		else if (IsUnsignedIntParameter (param_p))
			{
				if (param_p -> pa_options_p)
					{
						widget_p = new UnsignedIntComboBox (reinterpret_cast <UnsignedIntParameter *> (param_p), this);
					}
				else
					{
						widget_p = new UnsignedIntParamSpinBox (reinterpret_cast <UnsignedIntParameter *> (param_p), this);
					}
			}
		else if (IsTimeParameter (param_p))
			{
				widget_p = new ParamDateWidget (reinterpret_cast <TimeParameter *> (param_p), this);
			}
		else if (IsStringParameter (param_p))
			{
				StringParameter *string_param_p = reinterpret_cast <StringParameter *> (param_p);

				if ((param_p -> pa_type == PT_STRING) || (param_p -> pa_type == PT_KEYWORD))
					{
						if (param_p -> pa_options_p)
							{
								widget_p = new StringComboBox (string_param_p, this);
							}
						else
							{
								widget_p = new ParamLineEdit (string_param_p, this, QLineEdit :: Normal);
							}
					}
				else if ((param_p -> pa_type == PT_LARGE_STRING) || (param_p -> pa_type == PT_FASTA))
					{
						widget_p = new ParamTextBox (string_param_p, this);
					}
				else if (param_p -> pa_type == PT_TABLE)
					{
						widget_p = new StringTableWidget (string_param_p, this);
					}
			}
		else if (IsJSONParameter (param_p))
			{
				JSONParameter *json_param_p = reinterpret_cast <JSONParameter *> (param_p);

				if (param_p -> pa_type == PT_JSON)
					{
						widget_p = new ParamJSONEditor (json_param_p, this);
					}
				else if (param_p -> pa_type == PT_JSON_TABLE)
					{
						widget_p = new JSONTableWidget (json_param_p, this);
					}
			}
		else if (IsResourceParameter (param_p))
			{
				ResourceParameter *resource_param_p = reinterpret_cast <ResourceParameter *> (param_p);
				QFileDialog :: FileMode mode = QFileDialog :: FileMode :: AnyFile;

				if (param_p -> pa_type == PT_FILE_TO_READ)
					{
						mode = QFileDialog :: FileMode :: ExistingFile;
					}
				else if (param_p -> pa_type == PT_FILE_TO_WRITE)
					{
						mode = QFileDialog :: FileMode :: AnyFile;
					}
				if (param_p -> pa_type == PT_DIRECTORY)
					{
						mode = QFileDialog :: FileMode :: Directory;
					}


				widget_p = new FileChooserWidget (resource_param_p, this, mode);
			}
		else if (IsStringArrayParameter (param_p))
			{
			}

	}


	if (widget_p)
		{
			widget_p -> SetDefaultValue ();

			if (param_p -> pa_read_only_flag)
				{
					QWidget *qt_widget_p = widget_p -> GetUIQWidget ();

					if (qt_widget_p)
						{
							qt_widget_p -> setEnabled (false);
						}
				}

			ParameterWidgetAdded (param_p, widget_p);

		}		/* if (widget_p) */

	if (add_param_flag)
		{
			if (!AddParameterToParameterSet (qpw_params_p, param_p))
				{
					PrintErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, "Failed to add new parameter %s", widget_p -> GetParameterName ());
				}
		}

	return widget_p;
}




json_t *QTParameterWidget :: GetServiceParamsAsJSON (bool full_flag, const ParameterLevel level) const
{
	json_t *res_p = nullptr;
	const SchemaVersion *sv_p = qpw_client_data_p -> qcd_base_data.cd_schema_p;
	const char *service_name_s = qpw_parent_prefs_widget_p -> GetServiceName();

	//ParameterSet *params_p = GetParameterSet (false);
	json_t *params_json_p = GetParameterSetAsJSON (true);

	if (params_json_p)
		{
			res_p = params_json_p; //GetServiceRunRequestFromJSON (service_name_s, params_json_p, sv_p, true, level);
		}

	return res_p;
}




json_t *QTParameterWidget :: GetParameterSetAsJSON (bool refresh_flag) const
{
	QHash <const Parameter *, BaseParamWidget *> repeated_widgets;

	/* make sure that all of the parameter values are up to date */
	QList <BaseParamWidget *> widgets = qpw_widgets_map.values ();


	/* check for any repeatable paramaeter groups */
	QHash <const char *, RepeatableParamGroupBox *> :: const_iterator i;

	for (i = qpw_repeatable_groupings.constBegin (); i != qpw_repeatable_groupings.constEnd (); ++ i)
		{
			RepeatableParamGroupBox *box_p = i.value ();
			const QList <BaseParamWidget *> *children_p = box_p -> GetChildren ();
			QList <BaseParamWidget *> :: const_iterator j;

			for (j = children_p -> constBegin (); j != children_p -> constEnd (); ++ j)
				{
					BaseParamWidget *widget_p = *j;

					if (! (widget_p -> StoreParameterValue (refresh_flag)))
						{
							PrintErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, "Failed to set parameter value for %s", widget_p -> GetParameterName ());
							return nullptr;
						}

					repeated_widgets.insert (widget_p -> GetParameter (), widget_p);

				}

		}

	for (int i = widgets.size () - 1; i >= 0; -- i)
		{
			BaseParamWidget *widget_p = widgets.at (i);

			if (!repeated_widgets.contains (widget_p -> GetParameter ()))
				{
					if (! (widget_p -> StoreParameterValue (refresh_flag)))
						{
							PrintErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, "Failed to set parameter value for %s", widget_p -> GetParameterName ());
							return nullptr;
						}
				}
		}

	const SchemaVersion *sv_p = qpw_client_data_p -> qcd_base_data.cd_schema_p;

	json_t *param_set_json_p = GetParameterSetSelectionAsJSON (qpw_params_p, sv_p, false, &repeated_widgets, AddNonRepeatedParams);

	if (qpw_client_data_p->qcd_verbose_flag)
		{
			PrintJSONToLog (STM_LEVEL_INFO, __FILE__, __LINE__, param_set_json_p, "*** BEGIN Non-repeated");
			PrintLog (STM_LEVEL_INFO, __FILE__, __LINE__, "*** END Non-repeated");
		}

	if (param_set_json_p)
		{
			json_t *params_array_p = json_object_get (param_set_json_p, PARAM_SET_PARAMS_S);

			if (params_array_p)
				{
					/* Now add the repeated groups */
					for (i = qpw_repeatable_groupings.constBegin (); i != qpw_repeatable_groupings.constEnd (); ++ i)
						{
							RepeatableParamGroupBox *box_p = i.value ();
							json_t *group_json_p = box_p -> GetParametersAsJSON ();

							if (group_json_p)
								{
									if (json_array_extend (params_array_p, group_json_p) != 0)
										{
											PrintJSONToErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, group_json_p, "Failed to add repeated group json for %s", box_p -> GetGroupName ());
										}

									json_decref (group_json_p);
								}
						}

				}		/* if (params_array_p) */


		}		/* if (params_json_p) */


	if (qpw_client_data_p->qcd_verbose_flag)
		{
			PrintJSONToLog (STM_LEVEL_INFO, __FILE__, __LINE__, param_set_json_p, "*** BEGIN repeated");
			PrintLog (STM_LEVEL_INFO, __FILE__, __LINE__, "*** END repeated");
		}


	return param_set_json_p;
}


struct RepeatableParamsData
{
	QHash <BaseParamWidget *, BaseParamWidget *> *rpd_repeated_widgets_p;

};


static bool AddNonRepeatedParams(const Parameter *param_p, void *data_p)
{
	QHash <const Parameter *, BaseParamWidget *> *repeated_widgets_p = reinterpret_cast <QHash <const Parameter *, BaseParamWidget *> *> (data_p);

	return (!repeated_widgets_p -> contains (param_p));
}


json_t *QTParameterWidget :: GetParameterValuesAsJSON () const
{
	return GetParameterSetAsJSON (false);
}
