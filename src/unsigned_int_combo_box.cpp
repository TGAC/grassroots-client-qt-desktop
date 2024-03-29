#include "unsigned_int_combo_box.h"


#include "string_utils.h"
#include "math_utils.h"


UnsignedIntComboBox :: UnsignedIntComboBox (UnsignedIntParameter * const param_p, QTParameterWidget * const parent_p)
	: BaseComboBox (& (param_p -> uip_base_param), parent_p)
{
	uicb_param_p = param_p;

	if (param_p -> uip_base_param.pa_options_p)
		{
			UnsignedIntParameterOptionNode *node_p = reinterpret_cast <UnsignedIntParameterOptionNode *> (param_p -> uip_base_param.pa_options_p -> ll_head_p);
			bool success_flag = true;

			while (node_p && success_flag)
				{
					UnsignedIntParameterOption *option_p = node_p -> uipon_option_p;

					success_flag = AddOption (option_p -> uipo_value, option_p -> uipo_description_s);

					if (success_flag)
						{
							node_p = reinterpret_cast <UnsignedIntParameterOptionNode *> (node_p -> uipon_node.ln_next_p);
						}
				}
		}



}

UnsignedIntComboBox :: ~UnsignedIntComboBox ()
{

}

bool UnsignedIntComboBox :: AddOption (const uint32 value, const char *description_s)
{
	QVariant *v_p = new QVariant (value);
	bcb_combo_box_p -> addItem (description_s, *v_p);

	return true;
}


void UnsignedIntComboBox :: SetDefaultValue ()
{
	const uint32 *def_p = GetUnsignedIntParameterDefaultValue (uicb_param_p);

	if (def_p)
		{
			SetValue (*def_p);
		}
}


bool UnsignedIntComboBox :: SetValueFromText (const char *value_s)
{
	bool success_flag  = false;
	int32 value;

	if (GetValidInteger (&value_s, &value))
		{
			if (value >= 0)
				{
					success_flag = SetValue (static_cast <uint32> (value));
				}
		}

	return success_flag;
}


bool UnsignedIntComboBox :: SetValueFromJSON (const json_t * const value_p)
{
	bool success_flag = false;
	const json_t *param_value_p = json_object_get (value_p, PARAM_CURRENT_VALUE_S);

	if (param_value_p)
		{
			if (json_is_integer (param_value_p))
				{
					int i = json_integer_value (param_value_p);

					if (i >= 0)
						{
							success_flag = SetValue (static_cast <uint32> (i));
						}
				}
		}

	return success_flag;
}


bool UnsignedIntComboBox :: StoreParameterValue (bool refresh_flag)
{
	bool success_flag = false;
	int index = bcb_combo_box_p -> currentIndex ();

	if (index != -1)
		{
			QVariant v = bcb_combo_box_p -> itemData (index);
			uint32 i = static_cast <uint32> (v.toInt ());

			success_flag = SetUnsignedIntParameterCurrentValue (uicb_param_p, &i);
		}

	return success_flag;
}


bool UnsignedIntComboBox :: SetFromParameterValue (Parameter *param_p)
{
	bool success_flag = false;


	return success_flag;
}


bool UnsignedIntComboBox :: SetValue (const uint32 value)
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
