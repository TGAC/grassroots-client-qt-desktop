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

#include "signed_int_param_spin_box.h"
#include "prefs_widget.h"
#include "math_utils.h"

#include "signed_int_parameter.h"
#include "limits.h"


int32 SignedIntParamSpinBox :: SIPSB_DEFAULT_MIN = INT_MIN;

int32 SignedIntParamSpinBox :: SIPSB_DEFAULT_MAX = INT_MAX;


SignedIntParamSpinBox :: SignedIntParamSpinBox (SignedIntParameter * const param_p, QTParameterWidget * const parent_p)
:	BaseParamWidget (& (param_p -> sip_base_param), parent_p)
{
	sipsb_spin_box_p = new SignedIntSpinner (& (param_p -> sip_base_param), parent_p);
	sipsb_param_p = param_p;

	if (param_p -> sip_min_value_p)
		{
			sipsb_spin_box_p -> setMinimum (* (param_p -> sip_min_value_p));
		}
	else
		{
			sipsb_spin_box_p -> setMinimum (SignedIntParamSpinBox :: SIPSB_DEFAULT_MIN);
		}

	if (param_p -> sip_max_value_p)
		{
			sipsb_spin_box_p -> setMaximum (* (param_p -> sip_max_value_p));
		}
	else
		{
			sipsb_spin_box_p -> setMaximum (SignedIntParamSpinBox :: SIPSB_DEFAULT_MAX);
		}
}


SignedIntParamSpinBox :: ~SignedIntParamSpinBox ()
{
}

void SignedIntParamSpinBox :: SetDefaultValue ()
{
	const int32 *def_value_p = GetSignedIntParameterDefaultValue (sipsb_param_p);

	if (def_value_p)
		{
			sipsb_spin_box_p -> setValue (*def_value_p);
		}
	else
        {
            if (!IsRequired ())
				{
					sipsb_spin_box_p -> ClearValue ();
				}
		}
}


bool SignedIntParamSpinBox :: SetFromParameterValue (Parameter *param_p)
{
	bool success_flag = false;


	return success_flag;
}


bool SignedIntParamSpinBox :: StoreParameterValue (bool refresh_flag)
{
	bool b = false;

	if (sipsb_spin_box_p -> IsValueSet ())
		{
			const int value = sipsb_spin_box_p -> value ();
			b = SetSignedIntParameterCurrentValue (sipsb_param_p, &value);

			qDebug () << "Setting " << bpw_param_p -> pa_name_s << " to " << value;
		}
	else
		{
			qDebug () << "Setting " << bpw_param_p -> pa_name_s << "to NULL value";
			b = true;
		}

	return b;
}


bool SignedIntParamSpinBox :: SetValueFromText (const char *value_s)
{
	bool success_flag  = true;
	int value;

	if (GetValidInteger (&value_s, &value))
		{
			sipsb_spin_box_p -> setValue (value);
			success_flag = true;
		}

	return success_flag;
}


bool SignedIntParamSpinBox :: SetValueFromJSON (const json_t * const value_p)
{
	bool success_flag = false;

	const json_t *param_value_p = json_object_get (value_p, PARAM_CURRENT_VALUE_S);

	if (param_value_p)
		{
			qDebug () << "SignedIntParamSpinBox :: SetValueFromJSON  " << bpw_param_p -> pa_name_s << Qt :: endl;

			if ((!param_value_p) || (json_is_null (param_value_p)))
				{
					sipsb_spin_box_p -> ClearValue ();
					success_flag = true;

					qDebug () << "SignedIntParamSpinBox :: SetValueFromJSON  clearing " << bpw_param_p -> pa_name_s << Qt :: endl;
				}
			else
				{
					if (json_is_integer (param_value_p))
						{
							const int d = json_integer_value (param_value_p);

							qDebug () << "SignedIntParamSpinBox :: SetValueFromJSON d:  " << d << Qt :: endl;

							sipsb_spin_box_p -> setValue (d);
							success_flag = true;

							PrintLog (STM_LEVEL_INFO, __FILE__, __LINE__, "value %d for %s\n", d, bpw_param_name_s);
						}
					else
						{
							PrintJSONToErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, param_value_p, "JSON is not integer for %s\n", bpw_param_name_s);
						}
				}

		}
	else
		{
			PrintJSONToErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, value_p, "Failed to get %s\n", PARAM_CURRENT_VALUE_S);
		}

	return success_flag;
}


QWidget *SignedIntParamSpinBox :: GetQWidget ()
{
	return sipsb_spin_box_p;
}
