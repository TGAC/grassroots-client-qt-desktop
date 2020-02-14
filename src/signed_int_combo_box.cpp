#include "signed_int_combo_box.h"

#include "string_utils.h"
#include "math_utils.h"


SignedIntComboBox :: SignedIntComboBox (SignedIntParameter * const param_p, QTParameterWidget * const parent_p)
	: BaseComboBox (& (param_p -> sip_base_param), parent_p)
{
	sicb_param_p = param_p;
}

SignedIntComboBox :: ~SignedIntComboBox ()
{

}

bool SignedIntComboBox :: AddOption (const int32 value, const char *description_s)
{
	QVariant *v_p = new QVariant (value);
	bcb_combo_box_p -> addItem (description_s, *v_p);

	return true;
}


void SignedIntComboBox :: SetDefaultValue ()
{
	const int32 *def_p = GetSignedIntParameterDefaultValue (sicb_param_p);

	if (def_p)
		{
			SetValue (*def_p);
		}
}


bool SignedIntComboBox :: SetValueFromText (const char *value_s)
{
	bool success_flag  = false;
	int32 value;

	if (GetValidInteger (&value_s, &value))
		{
			success_flag = SetValue (value);
		}

	return success_flag;
}


bool SignedIntComboBox :: SetValueFromJSON (const json_t * const value_p)
{
	bool success_flag = false;

	if (json_is_integer (value_p))
		{
			int i = json_integer_value (value_p);
			success_flag = SetValue (i);
		}

	return success_flag;
}


bool SignedIntComboBox :: StoreParameterValue ()
{
	bool success_flag = false;
	int index = bcb_combo_box_p -> currentIndex ();

	if (index != -1)
		{
			QVariant v = bcb_combo_box_p -> itemData (index);
			int i = v.toInt ();

			success_flag = SetSignedIntParameterCurrentValue (sicb_param_p, &i);
		}

	return success_flag;
}



bool SignedIntComboBox :: SetValue (const int32 value)
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
