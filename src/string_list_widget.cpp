/*
 * string_list_widget.cpp
 *
 *  Created on: 11 Jan 2024
 *      Author: billy
 */

#include "string_list_widget.h"
#include "string_parameter.h"


StringListWidget :: StringListWidget (StringArrayParameter * const param_p, QTParameterWidget * const parent_p)
:		BaseParamWidget (& (param_p -> sap_base_param), parent_p)
{
	slw_param_p = param_p;

	slw_list_p = new QListWidget (parent_p);
	slw_list_p -> setSelectionMode (QAbstractItemView :: MultiSelection);

	AddOptions (bpw_param_p);


	if (bpw_param_p -> pa_refresh_service_flag)
		{
			QObject ::  connect (slw_list_p,  &QListWidget :: itemClicked, parent_p, &QTParameterWidget :: RefreshService);
		}

}


StringListWidget ::	~StringListWidget ()
{
	delete slw_list_p;
	slw_list_p = NULL;
}

void StringListWidget :: RemoveConnection ()
{
}


void StringListWidget :: AddOptions (Parameter *param_p)
{
	if (param_p -> pa_options_p)
		{
			StringParameterOptionNode *node_p = reinterpret_cast <StringParameterOptionNode *> (param_p -> pa_options_p -> ll_head_p);

			while (node_p)
				{
					const StringParameterOption *option_p = node_p -> spon_option_p;
					QListWidgetItem *item_p = new QListWidgetItem (option_p -> spo_description_s, slw_list_p);
					QString s (option_p -> spo_value_s);
					QVariant v (s);

					item_p -> setData (Qt :: UserRole, v);
					slw_list_p -> addItem (item_p);

					node_p = reinterpret_cast <StringParameterOptionNode *> (node_p -> spon_node.ln_next_p);
				}
		}

}


bool StringListWidget :: SetParameter (Parameter *param_p)
{
	bool success_flag = false;

	if (IsStringArrayParameter (param_p))
		{
			slw_param_p = (StringArrayParameter *) param_p;

			if (BaseParamWidget :: SetParameter (param_p))
				{
					const char **values_ss = GetStringArrayParameterCurrentValues (slw_param_p);


					success_flag = true;
				}
		}

	return success_flag;
}


bool StringListWidget :: SetFromParameterValue (Parameter *param_p)
{
	bool success_flag = false;

	if (IsStringArrayParameter (param_p))
		{
			StringArrayParameter *new_string_array_param_p = reinterpret_cast <StringArrayParameter *> (param_p);
			const char **values_ss = GetStringArrayParameterCurrentValues (new_string_array_param_p);
			size_t num_values = GetNumberOfStringArrayCurrentParameterValues (new_string_array_param_p);

			if (SetStringArrayParameterCurrentValues (slw_param_p, const_cast <char **> (values_ss), num_values))
				{
					if (new_string_array_param_p -> sap_base_param.pa_options_p)
						{
							if (!CopyStringParameterOptions (& (new_string_array_param_p -> sap_base_param), & (slw_param_p -> sap_base_param)))
								{
									slw_list_p -> clear ();
									AddOptions (& (slw_param_p -> sap_base_param));

									for (size_t i = 0; i < num_values; ++ i, ++ values_ss)
										{
											QList <QListWidgetItem *> matches = slw_list_p -> findItems (*values_ss, Qt :: MatchCaseSensitive);
											const qsizetype num_entries = matches.size ();

											for (qsizetype j = 0; j < num_entries; ++ j)
												{
													matches.at (j) -> setSelected (true);
												}
										}

								}
						}

				}



		}

	return success_flag;
}


void StringListWidget :: SetDefaultValue ()
{
	const char **values_ss = GetStringArrayParameterDefaultValues (slw_param_p);



//	ple_text_box_p -> setText (value_s);
}


QWidget *StringListWidget :: GetQWidget ()
{
	return slw_list_p;
}


bool StringListWidget :: SetValueFromText (const char *value_s)
{
	if (value_s)
		{
			QString s (value_s);
			QStringList entries = s.split ("\n");

			const qsizetype num_values = entries.size ();

			QMap <QString, QString> map;

			for (qsizetype i = 0; i < num_values; ++ i)
				{
					QString s = entries.at (i);
					map.insert (s, s);
				}

			const int num_entries = slw_list_p -> count ();

			for (int i = 0; i < num_entries; ++ i)
				{
					QString s = slw_list_p -> item (i) -> text ();
					slw_list_p -> item (i) -> setSelected (map.contains (s));
				}

		}
	else
		{
			SelectAllEntries (false);
		}

	return true;
}


void StringListWidget :: SelectAllEntries (const bool selected_flag)
{
	const int num_entries = slw_list_p -> count ();

	for (int i = 0; i < num_entries; ++ i)
		{
			slw_list_p -> item (i) -> setSelected (selected_flag);
		}
}


bool StringListWidget :: SetValueFromJSON (const json_t * const value_p)
{
	bool success_flag = false;

	if (json_is_array (value_p))
		{
			size_t i;
			json_t *entry_p;

			SelectAllEntries (false);

			json_array_foreach (value_p, i, entry_p)
				{
				if (json_is_string (entry_p))
					{
						const char *entry_s = json_string_value (entry_p);
						QList <QListWidgetItem *> l = slw_list_p -> findItems (entry_s, Qt :: MatchFixedString);

						for (qsizetype j = 0; j < l.count (); ++ j)
							{
								QListWidgetItem *item_p = l.at (j);
								item_p -> setSelected (true);
							}

					}

				}

			success_flag = true;
		}
	else if (json_is_null (value_p))
		{
			SelectAllEntries (false);
			success_flag = true;
		}
	else
		{
			PrintJSONToErrors (STM_LEVEL_WARNING, __FILE__, __LINE__, value_p, "Failed to set value for StringListWidget %s", bpw_param_name_s);
		}

	return success_flag;
}



bool StringListWidget :: StoreParameterValue (bool refresh_flag)
{
	bool success_flag = false;
	char **values_ss = nullptr;
	size_t num_values = 0;

	if (GetValues (&values_ss, &num_values))
		{
			if ((!(IsRequired ())) || refresh_flag || (num_values > 0))
				{
					if (GetErrorFlag ())
						{
							SetErrorMessage (nullptr);
						}

					success_flag = UpdateConfigValue (values_ss, num_values);
				}
			else
				{
					SetErrorMessage (BaseParamWidget ::  BPW_REQUIRED_S);
				}

			if (values_ss)
				{
					FreeMemory (values_ss);
				}
		}

	return success_flag;
}


bool StringListWidget :: UpdateConfigValue (char ** const values_ss, const size_t num_values)
{
	bool success_flag = SetStringArrayParameterCurrentValues (slw_param_p, values_ss, num_values);

	//qDebug () << "Setting " << bpw_param_p -> pa_name_s << " to " << value_s;

	return success_flag;
}


bool StringListWidget :: GetValues (char *** values_sss, size_t *num_values_p)
{
	bool success_flag = false;
	QStringList values;
	const int num_entries = slw_list_p -> count ();
	int num_values;

	for (int i = 0; i < num_entries; ++ i)
		{
			QListWidgetItem *item_p = slw_list_p -> item (i);

			if (item_p -> isSelected ())
				{
					QString s = item_p -> text ();
					values.append (s);
				}
		}

	if ((num_values = values.size ()) > 0)
		{
			char **results_ss = reinterpret_cast <char **> (AllocMemoryArray (num_values, sizeof (char *)));

			if (results_ss)
				{
					char **result_ss = results_ss;

					for (qsizetype i = 0; i < num_values; ++ i, ++ result_ss)
						{
							QString s = values.at (i);
							QByteArray ba = s.toLocal8Bit ();
							*result_ss = ba.data ();
						}

					*values_sss = results_ss;
					*num_values_p = num_values;
					success_flag = true;
				}
		}
	else
		{
			*values_sss = nullptr;
			*num_values_p = 0;
			success_flag = true;
		}

	return success_flag;
}

