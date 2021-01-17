#include <QDebug>


#include "repeatable_param_group_box.h"
#include "parameter_group.h"
#include "string_utils.h"
#include "qt_parameter_widget.h"
#include "qt_client_data.h"


RepeatableParamGroupBox :: RepeatableParamGroupBox (ParameterGroup *group_p, QTParameterWidget *qt_param_widget_p,  bool removable_flag, bool add_params_flag)
 : ParamGroupBox (group_p, qt_param_widget_p, removable_flag, add_params_flag)
{
	rpgb_entries_p = new QListWidget (this);
	rpgb_entries_p -> setSelectionMode (QAbstractItemView :: SingleSelection);

	rpgb_remove_entry_button_p = new QPushButton (QIcon ("images/remove"), "Remove", this);
	connect (rpgb_remove_entry_button_p, &QPushButton :: clicked, this, &RepeatableParamGroupBox :: RemoveEntry);

	rpgb_label_param_p = nullptr;

	if (group_p -> pg_repeatable_param_p)
		{
			rpgb_label_param_p = group_p -> pg_repeatable_param_p;
		}

}


RepeatableParamGroupBox :: ~RepeatableParamGroupBox ()
{

}



void RepeatableParamGroupBox :: Init (bool add_params_flag)
{
	QVBoxLayout *layout_p = new QVBoxLayout ();

	qDebug () << "RepeatableParamGroupBox :: init for " << pgb_parameter_group_p -> pg_name_s << Qt :: endl;


	layout_p -> addWidget (rpgb_entries_p);

	QHBoxLayout *buttons_layout_p = new QHBoxLayout;
	QPushButton *row_button_p = new QPushButton (QIcon ("images/add"), "Add", this);
	connect (row_button_p, &QPushButton :: clicked, this, &RepeatableParamGroupBox :: AddEntry);
	buttons_layout_p -> addWidget (row_button_p);

	buttons_layout_p -> addWidget (rpgb_remove_entry_button_p);

	layout_p -> addItem (buttons_layout_p);

	layout_p -> addItem (pgb_layout_p);


	connect (rpgb_entries_p, &QListWidget :: itemSelectionChanged, this, &RepeatableParamGroupBox :: SelectedListEntryChanged);

	setLayout (layout_p);
}


json_t *RepeatableParamGroupBox :: GetParametersAsJSON ()
{
	json_t *res_p = json_array ();

	if (res_p)
		{
			/*
			 * Add the initial parameter names to this array
			 */
			bool success_flag = true;
			ParameterNode *param_node_p = reinterpret_cast <ParameterNode *> (pgb_parameter_group_p -> pg_params_p -> ll_head_p);
			const char *group_s = this -> GetGroupName ();

			while (param_node_p && success_flag)
				{
					json_t *param_json_p = json_object ();

					success_flag = false;

					if (param_json_p)
						{
							if (json_array_append_new (res_p, param_json_p) == 0)
								{
									if (SetJSONString (param_json_p, PARAM_NAME_S, param_node_p -> pn_parameter_p -> pa_name_s))
										{
											json_t *param_values_p = json_array ();

											if (param_values_p)
												{
													if (json_object_set_new (param_json_p, PARAM_CURRENT_VALUE_S, param_values_p) == 0)
														{
															if (SetJSONString (param_json_p, PARAM_GROUP_S, group_s))
																{
																	success_flag = true;
																}
														}

													if (!success_flag)
														{
															json_decref (param_values_p);
														}
												}

										}
								}
							else
								{
									json_decref (param_json_p);
								}
						}

					if (success_flag)
						{
							param_node_p = reinterpret_cast <ParameterNode *> (param_node_p -> pn_node.ln_next_p);
						}
				}


			if (success_flag)
				{
					const int num_entries = rpgb_entries_p -> count ();
					const size_t num_params = json_array_size (res_p);

					for (int i = 0; i < num_entries; ++ i)
						{
							QListWidgetItem *item_p = rpgb_entries_p -> item (i);
							QVariant v = item_p -> data (Qt :: UserRole);
							QString s = v.toString ();
							QByteArray ba = s.toLocal8Bit ();

							const char *json_s = ba.data ();

							if (json_s)
								{
									json_error_t err;
									json_t *entry_json_p = json_loads (json_s, 0, &err);

									qDebug () << "getting " << i << ": " << json_s << Qt :: endl;

									if (entry_json_p)
										{
											json_t *entry_parameters_json_p = json_object_get (entry_json_p, PARAM_SET_PARAMS_S);

											if (entry_parameters_json_p)
												{
													const size_t num_entry_params = json_array_size (entry_parameters_json_p);

													for (size_t j = 0; j < num_entry_params; ++ j)
														{
															json_t *entry_param_p = json_array_get (entry_parameters_json_p, j);

															const char *param_s = GetJSONString (entry_param_p, PARAM_NAME_S);

															if (param_s)
																{
																	/*
																	 * Find the matching parameter json in our
																	 * results array
																	 */
																	json_t *param_p = nullptr;

																	for (size_t k = 0; k < num_params; ++ k)
																		{
																			json_t *tmp_p = json_array_get (res_p, k);
																			const char *tmp_name_s = GetJSONString (tmp_p, PARAM_NAME_S);

																			if (strcmp (tmp_name_s, param_s) == 0)
																				{
																					param_p = tmp_p;
																					k = num_params;		/* force exit from loop */
																				}

																		}

																	if (param_p)
																		{
																			json_t *value_p = json_object_get (entry_param_p, PARAM_CURRENT_VALUE_S);

																			if (value_p)
																				{
																					json_t *param_values_p = json_object_get (param_p, PARAM_CURRENT_VALUE_S);

																					if (param_values_p)
																						{
																							if (json_array_append (param_values_p, value_p) == 0)
																								{

																								}
																							else
																								{

																								}
																						}
																				}
																		}


																}		/* if (param_s) */
															else
																{
																	PrintJSONToErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, entry_json_p, "Failed to get %s\n", PARAM_SET_PARAMS_S);
																}


														}		/* for (size_t j = 0; j < num_entry_params; ++ j) */




												}		/* if (entry_parameters_json_p)*/
											else
												{
													PrintJSONToErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, entry_json_p, "Failed to get %s\n", PARAM_SET_PARAMS_S);
												}

										}		/* if (entry_json_p) */
									else
										{
											qDebug () << "failed to convert " << i << ": " << json_s << " to json " << Qt :: endl;
										}
								}
							else
								{
									qDebug () << "no data for " << i << Qt :: endl;
								}

						}		/* for (int i = 0; i < num_entries; ++ i) */

				}		/* if (success_flag) */


		}		/* if (res_p) */


	return res_p;
}


json_t *RepeatableParamGroupBox :: GetParametersAsGroupJSON ()
{
	json_t *res_p = json_array ();

	if (res_p)
		{
			const int num_entries = rpgb_entries_p -> count ();

			for (int i = 0; i < num_entries; ++ i)
				{
					QListWidgetItem *item_p = rpgb_entries_p -> item (i);
					QVariant v = item_p -> data (Qt :: UserRole);
					QString s = v.toString ();
					QByteArray ba = s.toLocal8Bit ();

					const char *json_s = ba.data ();

					if (json_s)
						{
							json_error_t err;
							json_t *entry_json_p = json_loads (json_s, 0, &err);

							qDebug () << "getting " << i << ": " << json_s << Qt :: endl;

							if (entry_json_p)
								{
									if (json_array_append_new (res_p, entry_json_p) != 0)
										{
											json_decref (entry_json_p);
										}
								}
							else
								{
									qDebug () << "failed to convert " << i << ": " << json_s << " to json " << Qt :: endl;
								}
						}
					else
						{
							qDebug () << "no data for " << i << Qt :: endl;
						}

				}

		}		/* if (res_p) */


	return res_p;
}




void RepeatableParamGroupBox :: AddEntry ()
{
	StoreValues ();

	const SchemaVersion *sv_p = pgb_parent_p -> GetClientData () -> qcd_base_data.cd_schema_p;
	json_t *group_json_p = GetParameterGroupAsJSON (pgb_parameter_group_p, true, false, sv_p);

	if (group_json_p)
		{
			QString label;
			bool set_label_flag = false;
			Parameter *param_p = pgb_parameter_group_p -> pg_repeatable_param_p;

			if (param_p)
				{
					bool alloc_flag = false;
					char *value_s = GetParameterValueAsString (param_p, &alloc_flag);

					if (value_s)
						{
							label = value_s;
							set_label_flag = true;
						}

					if (alloc_flag)
						{
							FreeCopiedString (value_s);
						}
				}

			if (!set_label_flag)
				{
					int i = rpgb_entries_p -> count ();
					label = QString :: number (i);
				}

			QListWidgetItem *item_p = new QListWidgetItem (label, rpgb_entries_p);

			char *value_s = json_dumps (group_json_p, 0);
			if (value_s)
				{
					item_p -> setData (Qt :: UserRole, value_s);
					free (value_s);
				}

			qDebug () << "adding " << label << ": " << item_p -> data (Qt :: UserRole) << Qt :: endl;

			rpgb_entries_p -> addItem (item_p);

			json_decref (group_json_p);
		}

}

void RepeatableParamGroupBox :: RemoveEntry ()
{
	QList <QListWidgetItem *> l =	rpgb_entries_p -> selectedItems ();
	const int count = l.size ();

	if (count == 1)
		{
			QListWidgetItem *item_p = l.at (0);

			qDebug () << "removing " << item_p -> text () << ": " << item_p -> data (Qt :: UserRole) << Qt :: endl;

			rpgb_entries_p -> removeItemWidget (item_p);
			delete item_p;
		}
}


void RepeatableParamGroupBox :: SelectedListEntryChanged ()
{
	QList <QListWidgetItem *> l =	rpgb_entries_p -> selectedItems ();
	const int count = l.size ();

	if (count == 1)
		{
			QListWidgetItem *item_p = l.at (0);
			qDebug () << "selected " << item_p -> text () << ": " << item_p -> data (Qt :: UserRole) << Qt :: endl;
		}

	if (count > 0)
		{
			rpgb_remove_entry_button_p -> setEnabled (true);
		}
	else
		{
			rpgb_remove_entry_button_p -> setEnabled (false);
		}
}



