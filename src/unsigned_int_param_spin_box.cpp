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
	uips_param_p = param_p;
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
	const uint32 *def_value_p = GetUnsignedIntParameterDefaultValue (uips_param_p);

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


bool UnsignedIntParamSpinBox :: StoreParameterValue ()
{
	bool b = false;

	if (uipsb_spin_box_p -> IsValueSet ())
		{
			const uint32 value = uipsb_spin_box_p -> GetValue ();
			b = SetUnsignedIntParameterCurrentValue (uips_param_p, &value);

			qDebug () << "Setting " << bpw_param_p -> pa_name_s << " to " << value;
		}
	else
		{
			qDebug () << "Setting " << bpw_param_p -> pa_name_s << "to NULL value";
			b = true;
		}


	return b;
}


bool UnsignedIntParamSpinBox :: SetValueFromText (const char *value_s)
{
	bool success_flag  = true;
	int value;

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


bool UnsignedIntParamSpinBox :: SetValueFromJSON (const json_t * const value_p)
{
	bool success_flag = false;

	if (json_is_integer (value_p))
		{
			const int d = json_integer_value (value_p);

			if (d >= 0)
				{
					uipsb_spin_box_p -> setValue (d);
					success_flag = true;
				}
		}

	return success_flag;
}


QWidget *UnsignedIntParamSpinBox :: GetQWidget ()
{
	return uipsb_spin_box_p;
}

