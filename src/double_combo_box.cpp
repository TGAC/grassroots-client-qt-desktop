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


bool DoubleComboBox :: StoreParameterValue ()
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
