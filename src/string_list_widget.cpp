/*
 * string_list_widget.cpp
 *
 *  Created on: 11 Jan 2024
 *      Author: billy
 */

#include "string_list_widget.h"


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
			StringListNode *node_p = reinterpret_cast <StringListNode *> (param_p -> pa_options_p -> ll_head_p);

			while (node_p)
				{
					slw_list_p -> addItem (node_p -> sln_string_s);

					node_p = reinterpret_cast <StringListNode *> (node_p -> sln_node.ln_next_p);
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
			const int num_entries = slw_list_p -> count ();

			for (int i = 0; i < num_entries; ++ i)
				{
					slw_list_p -> item (i) -> setSelected (false);
				}
		}

	return true;
}



bool StringListWidget :: SetValueFromJSON (const json_t * const value_p)
{
	bool success_flag = false;

	if (json_is_string (value_p))
		{
			const char *value_s = json_string_value (value_p);

			ple_text_box_p -> setText (value_s);
			success_flag = true;
		}
	else if (json_is_null (value_p))
		{
			ple_text_box_p -> setText (nullptr);
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
	QString s = ple_text_box_p -> text ();
	QByteArray ba = s.toLocal8Bit ();
	const char *value_s = ba.constData ();


        if ((!(IsRequired ())) || refresh_flag || (!IsStringEmpty (value_s)))
		{
			if (GetErrorFlag ())
				{
					SetErrorMessage (nullptr);
				}

			if (IsStringEmpty (value_s))
				{
					value_s = nullptr;
				}

			success_flag = UpdateConfigValue (value_s);
		}
	else
		{
			SetErrorMessage (BaseParamWidget ::  BPW_REQUIRED_S);
		}

	return success_flag;
}


bool StringListWidget :: UpdateConfigValue (const char * const value_s)
{
	bool success_flag = SetStringParameterCurrentValue (ple_param_p, value_s);

	qDebug () << "Setting " << bpw_param_p -> pa_name_s << " to " << value_s;

	return success_flag;
}

