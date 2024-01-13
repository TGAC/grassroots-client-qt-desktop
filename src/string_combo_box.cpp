#include "string_combo_box.h"

#include "qt_parameter_widget.h"

StringComboBox :: StringComboBox (StringParameter * const param_p, QTParameterWidget * const parent_p)
	: BaseComboBox (& (param_p -> sp_base_param), parent_p)
{
	scb_param_p = param_p;

	SetParameterOptions (& (param_p -> sp_base_param));

	SetDefaultValue ();
}

StringComboBox :: ~StringComboBox ()
{

}


bool StringComboBox :: SetParameterOptions (Parameter *param_p)
{
	bool success_flag = true;

	if (param_p -> pa_options_p)
		{
			StringParameterOptionNode *node_p = reinterpret_cast <StringParameterOptionNode *> (param_p -> pa_options_p -> ll_head_p);

			while (node_p && success_flag)
				{
					StringParameterOption *option_p = node_p -> spon_option_p;

					success_flag = AddOption (option_p -> spo_value_s, option_p -> spo_description_s);

					if (success_flag)
						{
							node_p = reinterpret_cast <StringParameterOptionNode *> (node_p -> spon_node.ln_next_p);
						}
					else
						{
							PrintErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, "AddOption () failed for \"%s\": \"%s\"\n\n", option_p -> spo_value_s, option_p -> spo_description_s);
						}
				}
		}

	return success_flag;
}


bool StringComboBox :: SetParameter (Parameter *param_p)
{
	bool success_flag = false;

	if (BaseComboBox :: SetParameter (param_p))
		{
			const char *current_value_s = GetStringParameterCurrentValue ((StringParameter *) param_p);
			bcb_combo_box_p -> clear ();

			if (SetParameterOptions (param_p))
				{
					success_flag = SetValueFromText (current_value_s);
				}
		}

	return success_flag;
}


bool StringComboBox :: SetFromParameterValue (Parameter *param_p)
{
	bool success_flag = false;

	if (IsStringParameter (param_p))
		{
			StringParameter *str_param_p = reinterpret_cast <StringParameter *> (param_p);
			const char *value_s = GetStringParameterCurrentValue (str_param_p);

			PrintLog (STM_LEVEL_SEVERE, __FILE__, __LINE__, "Param \"%s\" has value \"%s\"\n", param_p -> pa_name_s, value_s);


			bcb_combo_box_p -> clear ();

			if (SetParameterOptions (param_p))
				{
					if (SetStringParameterCurrentValue (scb_param_p, value_s))
						{
							SetValueFromText (value_s);
							success_flag = true;
						}
				}
		}

	return success_flag;
}


bool StringComboBox :: AddOption (const char *value_s, const char *description_s)
{
	QVariant v (value_s);
	bcb_combo_box_p -> addItem (description_s, v);

	return true;
}


void StringComboBox :: SetDefaultValue ()
{
	const char *def_s = GetStringParameterDefaultValue (scb_param_p);

	if (def_s)
		{
			SetValueFromText (def_s);
		}
}


bool StringComboBox :: SetValueFromText (const char *value_s)
{
	bool success_flag  = false;
	QVariant v (value_s);
	int index = bcb_combo_box_p -> findData (v);



	PrintLog (STM_LEVEL_SEVERE, __FILE__, __LINE__, "\"%s\" looking for \"%s\" in %d options\n", bpw_param_name_s, value_s, bcb_combo_box_p -> count ());


	if (index != -1)
		{
			bcb_combo_box_p -> setCurrentIndex (index);
			success_flag  = true;
		}
	else
		{
			PrintErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, "Could not find \"%s\" in combobox for \"%s\" in service \"%s\"\n", value_s, bpw_param_name_s, bpw_parent_p -> GetServiceName ());
		}

	return success_flag;
}


bool StringComboBox :: SetValueFromJSON (const json_t * const value_p)
{
	bool success_flag = false;

	if (json_is_string (value_p))
		{
			const char *value_s = json_string_value (value_p);
			success_flag = SetValueFromText (value_s);
		}

	return success_flag;
}


bool StringComboBox :: StoreParameterValue (bool refresh_flag)
{
	bool success_flag = false;
	int index = bcb_combo_box_p -> currentIndex ();

	if (index != -1)
		{
			QVariant v = bcb_combo_box_p -> itemData (index);
			QString s = v.toString ();
			QByteArray ba = s.toLocal8Bit ();
			const char *value_s = ba.constData ();

			success_flag = SetStringParameterCurrentValue (scb_param_p, value_s);
		}

	return success_flag;
}
