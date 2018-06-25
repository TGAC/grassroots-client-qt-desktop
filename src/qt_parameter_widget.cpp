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

#include "qt_parameter_widget.h"
#include "file_chooser_widget.h"


#include "param_check_box.h"
#include "param_double_spin_box.h"
#include "param_spin_box.h"
#include "param_combo_box.h"
#include "param_line_edit.h"
#include "param_text_box.h"
#include "prefs_widget.h"
#include "param_table_widget.h"
#include "param_json_editor.h"
#include "repeatable_param_group_box.h"

#include "qt_client_data.h"

// WHEATIS INCLUDES
#include "parameter.h"
#include "parameter_set.h"
#include "parameter_group.h"
#include "string_utils.h"
#include "provider.h"


const int QTParameterWidget :: QPW_NUM_COLUMNS = 2;


QTParameterWidget :: QTParameterWidget (const char *name_s, const char * const description_s, const char * const uri_s, const json_t *provider_p, ParameterSet *parameters_p, ServiceMetadata *metadata_p, const PrefsWidget * const prefs_widget_p, const ParameterLevel initial_level, const QTClientData *client_data_p)
:	qpw_params_p (parameters_p),
	qpw_prefs_widget_p (prefs_widget_p),
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


	if (qpw_params_p)
		{
			AddParameters (qpw_params_p);
		}		/* if (parameters_p) */

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
	SchemaTermNode *node_p = (SchemaTermNode *) terms_p -> ll_head_p;

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
					list_layout_p -> addWidget (label_p);
					connect (label_p,  &QLabel :: linkActivated, this, &QTParameterWidget :: OpenLink);
				}
			else
				{
					success_flag = false;
				}

			node_p = (SchemaTermNode *) node_p -> stn_node.ln_next_p;
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
	QLabel *logo_p = 0;
	QLabel *text_p = 0;

	const char *provider_logo_s = GetProviderLogo (provider_p);

	if (provider_logo_s)
		{
			CurlTool *curl_tool_p = AllocateCurlTool (CM_MEMORY);

			if (curl_tool_p)
				{
					if (SetUriForCurlTool (curl_tool_p, provider_logo_s))
						{
							CURLcode res = RunCurlTool (curl_tool_p);

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



void QTParameterWidget :: AddRow (QWidget *first_p, QWidget *second_p, const int row_span)
{
	int row = qpw_layout_p -> rowCount ();

	qpw_layout_p -> addRow (first_p, second_p);
}


void QTParameterWidget :: ParameterWidgetAdded (Parameter *param_p, BaseParamWidget *widget_p)
{
	qpw_widgets_map.insert (param_p, widget_p);
}


void QTParameterWidget :: AddParameters (ParameterSet *params_p)
{
	ParameterNode *node_p = reinterpret_cast <ParameterNode *> (params_p -> ps_params_p -> ll_head_p);
	ParameterGroupNode *param_group_node_p = reinterpret_cast <ParameterGroupNode *> (params_p -> ps_grouped_params_p -> ll_head_p);

	while (param_group_node_p)
		{
			ParameterGroup *group_p = param_group_node_p -> pgn_param_group_p;
			ParameterWidgetContainer *container_p = 0;

			if (group_p -> pg_repeatable_flag)
				{
					container_p = new RepeatableParamGroupBox (group_p, this);
				}
			else
				{
					container_p = new ParamGroupBox (group_p, this, false, false);
				}

			int row = qpw_layout_p -> rowCount ();

			QWidget *box_p = container_p -> GetWidget ();
			qpw_layout_p -> addRow (box_p);
			qpw_groupings.insert (group_p -> pg_name_s, container_p);

			param_group_node_p = reinterpret_cast <ParameterGroupNode *> (param_group_node_p -> pgn_node.ln_next_p);
		}

	while (node_p)
		{
			Parameter * const param_p = node_p -> pn_parameter_p;

			if (!qpw_widgets_map.contains (param_p))
				{
					ParameterWidgetContainer *container_p = 0;

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
	BaseParamWidget *child_p = CreateWidgetForParameter (param_p, add_params_flag);

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

			if (!CompareParameterLevels (param_p -> pa_level, qpw_level))
				{
					child_p -> SetVisible (false);
				}
		}
}




BaseParamWidget *QTParameterWidget :: GetWidgetForParameter (const char * const param_name_s) const
{
	BaseParamWidget *widget_p = 0;

	const QList <Parameter *> keys = qpw_widgets_map.keys ();

	for (int i = keys.size () - 1; i >= 0; -- i)
		{
			Parameter *param_p = keys.at (i);

			if (strcmp (param_p -> pa_name_s, param_name_s) == 0)
				{
					widget_p = qpw_widgets_map.value (param_p);
					i = -1;		/* force exit from loop */
				}
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
	QList <Parameter *> keys = qpw_widgets_map.keys ();

	for (int i = keys.size () - 1; i >= 0; --i)
		{
			Parameter *param_p = keys.at (i);
			BaseParamWidget *widget_p = qpw_widgets_map.value (param_p);
			widget_p -> SetDefaultValue ();
		}

}


void QTParameterWidget :: UpdateParameterLevel (const ParameterLevel level, const QWidget * const parent_widget_p)
{
	QHash <Parameter *, BaseParamWidget *> :: const_iterator i;

	for (i = qpw_widgets_map.constBegin (); i != qpw_widgets_map.constEnd (); ++ i)
		{
			BaseParamWidget *widget_p = reinterpret_cast <BaseParamWidget *> (i.value ());

			widget_p -> CheckLevelDisplay (level, parent_widget_p);
		}

	QList <ParameterWidgetContainer *> groupings = qpw_groupings.values ();
	for (int i = qpw_groupings.count () - 1; i >= 0; -- i)
		{
			ParameterWidgetContainer *container_p = groupings.at (i);
			container_p -> CheckVisibility (level);
		}

	qpw_level = level;	
}


BaseParamWidget *QTParameterWidget :: CreateWidgetForParameter (Parameter * const param_p, bool add_param_flag)
{
	BaseParamWidget *widget_p = 0;

	if (param_p -> pa_options_p)
		{
			widget_p = ParamComboBox :: Create (param_p, this);
		}
	else
		{
			switch (param_p -> pa_type)
				{
					case PT_BOOLEAN:
						widget_p = new ParamCheckBox (param_p, this);
						break;

					case PT_CHAR:
						{
							ParamLineEdit *editor_p = new ParamLineEdit (param_p, this, QLineEdit :: Normal);
							editor_p -> SetMaxLength (1);
							widget_p = editor_p;
						}
						break;

					case PT_SIGNED_REAL:
					case PT_UNSIGNED_REAL:
						widget_p = new ParamDoubleSpinBox (param_p, this);
						break;

					case PT_FILE_TO_READ:
						widget_p = new FileChooserWidget (param_p, this, QFileDialog :: ExistingFile);
						break;

					case PT_FILE_TO_WRITE:
						widget_p = new FileChooserWidget (param_p, this, QFileDialog :: AnyFile);
						break;

					case PT_STRING:
					case PT_KEYWORD:
						widget_p = new ParamLineEdit (param_p, this, QLineEdit :: Normal);
						break;

					case PT_PASSWORD:
						widget_p = new ParamTextBox (param_p, this);
						break;

					case PT_SIGNED_INT:
					case PT_NEGATIVE_INT:
						widget_p = new ParamSpinBox (param_p, this, true);
						break;

					case PT_UNSIGNED_INT:
						widget_p = new ParamSpinBox (param_p, this, false);
						break;

					case PT_DIRECTORY:
						widget_p = new FileChooserWidget (param_p, this, QFileDialog :: Directory);
						break;

					case PT_LARGE_STRING:
					case PT_FASTA:
						widget_p = new ParamTextBox (param_p, this);
						break;

					case PT_JSON:
						widget_p = new ParamJSONEditor (param_p, this);
						break;

					case PT_TABLE:
						widget_p = new ParamTableWidget (param_p, this);
						break;

					default:
							break;

				}		/* switch (param_p -> pa_type) */
		}

	if (widget_p)
		{
			//QWidget *w_p = widget_p -> GetUIQWidget ();

			widget_p -> SetDefaultValue ();

			qpw_widgets_map.insert (param_p, widget_p);

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


ParameterSet *QTParameterWidget :: GetParameterSet () const
{
	/* make sure that all of the parameter values are up to date */
	QList <BaseParamWidget *> widgets = qpw_widgets_map.values ();

	for (int i = widgets.size () - 1; i >= 0; -- i)
		{
			BaseParamWidget *widget_p = widgets.at (i);

			if (! (widget_p -> StoreParameterValue ()))
				{
					PrintErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, "Failed to set parameter value for %s", widget_p -> GetParameterName ());
				}
		}

	return qpw_params_p;
}



json_t *QTParameterWidget :: GetParameterValuesAsJSON () const
{
	return GetParameterSetAsJSON (qpw_params_p, qpw_client_data_p -> qcd_base_data.cd_schema_p, false);
}
