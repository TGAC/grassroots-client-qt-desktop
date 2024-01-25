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
#include <QDebug>

#include "unsigned_int_param_spin_box.h"
#include "prefs_widget.h"
#include "math_utils.h"

#include "limits.h"





UnsignedIntParamSpinBox :: UnsignedIntParamSpinBox (UnsignedIntParameter * const param_p, QTParameterWidget * const parent_p)
:	BaseParamWidget (& (param_p -> uip_base_param), parent_p)
{
	uipsb_param_p = param_p;
	uipsb_spin_box_p = new UnsignedIntSpinner (param_p -> uip_base_param.pa_required_flag, parent_p);

	if (param_p -> uip_min_value_p)
		{
			uipsb_spin_box_p -> setMinimum (static_cast <int> (* (param_p -> uip_min_value_p)));
		}

	if (param_p -> uip_max_value_p)
		{
			uipsb_spin_box_p -> setMaximum (* (param_p -> uip_max_value_p));
		}
}


UnsignedIntParamSpinBox :: ~UnsignedIntParamSpinBox ()
{
}

void UnsignedIntParamSpinBox :: SetDefaultValue ()
{
	const uint32 *def_value_p = GetUnsignedIntParameterDefaultValue (uipsb_param_p);

	if (def_value_p)
		{
			uipsb_spin_box_p -> setValue (*def_value_p);
		}
	else
		{
			if (! (bpw_param_p -> pa_required_flag))
				{
					uipsb_spin_box_p -> ClearValue ();
				}
		}
}


bool UnsignedIntParamSpinBox :: StoreParameterValue (bool refresh_flag)
{
	bool b = false;

	if (uipsb_spin_box_p -> IsValueSet ())
		{			
			const uint32 value = uipsb_spin_box_p -> GetValue ();
			b = SetUnsignedIntParameterCurrentValue (uipsb_param_p, &value);

			qDebug () << "UnsignedIntParamSpinBox :: StoreParameterValue: Setting " << bpw_param_p -> pa_name_s << " to " << value;
		}
	else
		{
			qDebug () << "UnsignedIntParamSpinBox :: StoreParameterValue: Setting " << bpw_param_p -> pa_name_s << "to NULL value";
			b = SetUnsignedIntParameterCurrentValue (uipsb_param_p, NULL);
		}


	return b;
}


bool UnsignedIntParamSpinBox :: SetFromParameterValue (Parameter *param_p)
{
	bool success_flag = false;

	if (IsUnsignedIntParameter (param_p))
		{
			const uint32 *value_p = GetUnsignedIntParameterCurrentValue (reinterpret_cast <UnsignedIntParameter *> (param_p));

			if (SetUnsignedIntParameterCurrentValue (uipsb_param_p, value_p))
				{
					if (value_p)
						{
							uipsb_spin_box_p -> setValue (*value_p);
						}
					else
						{
							uipsb_spin_box_p -> ClearValue ();
						}

					SetWidgetEnabled (! (param_p -> pa_read_only_flag));

					success_flag = true;
				}
		}

	return success_flag;
}


bool UnsignedIntParamSpinBox :: SetValueFromText (const char *value_s)
{
	bool success_flag  = true;

	int value;

	qDebug () << "UnsignedIntParamSpinBox :: SetValueFromText  " << bpw_param_p -> pa_name_s << " value " << value_s <<  Qt :: endl;

	if (GetValidInteger (&value_s, &value))
		{
			if (value >= 0)
				{
					uipsb_spin_box_p -> setValue (value);
					success_flag = true;
				}
		}

	return success_flag;
}


bool UnsignedIntParamSpinBox :: SetValueFromJSON (const json_t * const param_value_p)
{
	bool success_flag = false;

	qDebug () << "UnsignedIntParamSpinBox :: SetValueFromJSON  " << bpw_param_p -> pa_name_s << Qt :: endl;

	if ((!param_value_p) || (json_is_null (param_value_p)))
		{
			uipsb_spin_box_p -> ClearValue ();
			success_flag = true;

			qDebug () << "UnsignedIntParamSpinBox :: SetValueFromJSON  clearing " << bpw_param_p -> pa_name_s << Qt :: endl;
		}
	else
		{
			if (json_is_integer (param_value_p))
				{
					const int d = json_integer_value (param_value_p);

					qDebug () << "UnsignedIntParamSpinBox :: SetValueFromJSON d:  " << d << Qt :: endl;

					if (d >= 0)
						{
							uipsb_spin_box_p -> setValue (d);
							success_flag = true;

							PrintLog (STM_LEVEL_INFO, __FILE__, __LINE__, "value %d for %s\n", d, bpw_param_name_s);
						}
					else
						{
							PrintErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, "value %d is not an unsigned integer for %s\n", d, bpw_param_name_s);
						}
				}
			else
				{
					PrintJSONToErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, param_value_p, "JSON is not integer for %s\n", bpw_param_name_s);
				}
		}

	return success_flag;
}


QWidget *UnsignedIntParamSpinBox :: GetQWidget ()
{
	return uipsb_spin_box_p;
}

