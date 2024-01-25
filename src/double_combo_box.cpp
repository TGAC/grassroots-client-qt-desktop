#include "double_combo_box.h"

#include "string_utils.h"
#include "math_utils.h"


DoubleComboBox :: DoubleComboBox (DoubleParameter * const param_p, QTParameterWidget * const parent_p)
	: BaseComboBox (& (param_p -> dp_base_param), parent_p)
{
	dcb_param_p = param_p;
}

DoubleComboBox :: ~DoubleComboBox ()
{

}

bool DoubleComboBox :: AddOption (const double64 value, const char *description_s)
{
	QVariant *v_p = new QVariant (value);
	bcb_combo_box_p -> addItem (description_s, *v_p);

	return true;
}


void DoubleComboBox :: SetDefaultValue ()
{
	const double64 *def_p = GetDoubleParameterDefaultValue (dcb_param_p);

	if (def_p)
		{
			SetValue (*def_p);
		}
	else
		{
		}

}


bool DoubleComboBox :: SetFromParameterValue (Parameter *param_p)
{
	bool success_flag = false;

	if (IsDoubleParameter (param_p))
		{
			DoubleParameter *dbl_param_p = reinterpret_cast <DoubleParameter *> (param_p);
			const double64 *value_p = GetDoubleParameterCurrentValue (dbl_param_p);

			bcb_combo_box_p -> clear ();

			if (SetParameterOptions (param_p))
				{
					if (SetDoubleParameterCurrentValue (dcb_param_p, value_p))
						{
							SetValue (*value_p);

							SetWidgetEnabled (! (param_p -> pa_read_only_flag));

							success_flag = true;
						}
				}
		}

	return success_flag;
}



bool DoubleComboBox :: SetValueFromText (const char *value_s)
{
	bool success_flag  = false;
	double64 value;

	if (GetValidRealNumber (&value_s, &value, nullptr))
		{
			success_flag = SetValue (value);
		}

	return success_flag;
}


bool DoubleComboBox :: SetValueFromJSON (const json_t * const value_p)
{
	bool success_flag = false;

	if (json_is_number (value_p))
		{
			double64 value = json_number_value (value_p);
			success_flag = SetValue (value);
		}

	return success_flag;
}


bool DoubleComboBox :: StoreParameterValue (bool refresh_flag)
{
	bool success_flag = false;
	int index = bcb_combo_box_p -> currentIndex ();

	if (index != -1)
		{
			QVariant v = bcb_combo_box_p -> itemData (index);
			double64 d = v.toReal ();

			success_flag = SetDoubleParameterCurrentValue (dcb_param_p, &d);
		}

	return success_flag;
}



bool DoubleComboBox :: SetValue (const double64 value)
{
	bool success_flag = false;
	QVariant v (value);
	int index = bcb_combo_box_p -> findData (v);

	if (index != -1)
		{
			bcb_combo_box_p -> setCurrentIndex (index);
			success_flag  = true;
		}

	return success_flag;
}



bool DoubleComboBox :: SetParameterOptions (Parameter *param_p)
{
	bool success_flag = true;

	if (param_p -> pa_options_p)
		{
			DoubleParameterOptionNode *node_p = reinterpret_cast <DoubleParameterOptionNode *> (param_p -> pa_options_p -> ll_head_p);

			while (node_p && success_flag)
				{
					DoubleParameterOption *option_p = node_p -> dpon_option_p;

					success_flag = AddOption (option_p -> dpo_value, option_p -> dpo_description_s);

					if (success_flag)
						{
							node_p = reinterpret_cast <DoubleParameterOptionNode *> (node_p -> dpon_node.ln_next_p);
						}
					else
						{
							PrintErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, "AddOption () failed for " DOUBLE64_FMT " : \"%s\"\n", option_p -> dpo_value, option_p -> dpo_description_s);
						}
				}
		}

	return success_flag;
}


