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

#include "param_double_spin_box.h"
#include "prefs_widget.h"
#include "math_utils.h"


const double ParamDoubleSpinBox :: pdsb_min_bound = -1000000.0;
const double ParamDoubleSpinBox :: pdsb_max_bound = 1000000.0;


ParamDoubleSpinBox :: ParamDoubleSpinBox (DoubleParameter * const param_p, QTParameterWidget * const parent_p)
:		BaseParamWidget (& (param_p -> dp_base_param), parent_p)
{
	pdsb_spinner_p = new DoubleSpinner (& (param_p -> dp_base_param), parent_p);
	pdsb_param_p = param_p;
	int default_precision = 4;
	const char *prec_value_s = GetParameterKeyValue (& (param_p -> dp_base_param), PA_DOUBLE_PRECISION_S);
	const double step = 0.10f;


	if (prec_value_s)
		{
			GetValidInteger (&prec_value_s, &default_precision);
		}

	pdsb_spinner_p -> setDecimals (default_precision);

	pdsb_spinner_p -> setMinimum (ParamDoubleSpinBox :: pdsb_min_bound);
	pdsb_spinner_p -> setMaximum (ParamDoubleSpinBox :: pdsb_max_bound);
	pdsb_spinner_p -> setSingleStep (step);

	if (param_p -> dp_base_param.pa_refresh_service_flag)
		{
			QObject ::  connect (pdsb_spinner_p, static_cast <void (QDoubleSpinBox :: *) (double)> (&QDoubleSpinBox :: valueChanged), parent_p, &QTParameterWidget :: RefreshService);
		}
}


ParamDoubleSpinBox :: ~ParamDoubleSpinBox ()
{}


void ParamDoubleSpinBox :: SetDefaultValue ()
{
	const double64 *value_p = GetDoubleParameterDefaultValue (pdsb_param_p);

	if (value_p)
		{
			pdsb_spinner_p -> setValue (*value_p);
		}
}


bool ParamDoubleSpinBox :: StoreParameterValue (bool refresh_flag)
{
	bool success_flag = false;

	if (pdsb_spinner_p -> IsValueSet ())
		{
			const double value = pdsb_spinner_p -> value ();

			if ((CompareDoubles (value, ParamDoubleSpinBox :: pdsb_min_bound) != 0) &&
					(CompareDoubles (value, ParamDoubleSpinBox :: pdsb_max_bound) != 0))
				{
					success_flag = SetDoubleParameterCurrentValue (pdsb_param_p, &value);
					qDebug () << "Setting " << bpw_param_p -> pa_name_s << " to " << value;

					if (GetErrorFlag ())
						{
							SetErrorMessage (nullptr);
						}
				}
		}

	if (!success_flag)
		{
			if ((IsRequired ()) && (!refresh_flag))
				{
					SetErrorMessage (BaseParamWidget ::  BPW_REQUIRED_S);
				}
			else
				{
					success_flag = SetDoubleParameterCurrentValue (pdsb_param_p, NULL);
				}
		}

	return success_flag;
}


bool ParamDoubleSpinBox :: SetFromParameterValue (Parameter *param_p)
{
	bool success_flag = false;

	if (IsDoubleParameter (param_p))
		{
			const double64 *value_p = GetDoubleParameterCurrentValue (reinterpret_cast <DoubleParameter *> (param_p));

			if (SetDoubleParameterCurrentValue (pdsb_param_p, value_p))
				{
					if (value_p)
						{
							pdsb_spinner_p -> setValue (*value_p);
						}
					else
						{
							pdsb_spinner_p -> ClearValue ();
						}

					SetWidgetEnabled (! (param_p -> pa_read_only_flag));

					success_flag = true;
				}
		}

	return success_flag;
}


bool ParamDoubleSpinBox :: SetValueFromText (const char *value_s)
{
	bool success_flag  = false;
	double value;

	if (GetValidRealNumber (&value_s, &value, NULL))
		{
			pdsb_spinner_p -> setValue (value);
			success_flag = true;
		}

	return success_flag;
}



bool ParamDoubleSpinBox :: SetValueFromJSON (const json_t * const value_p)
{
	bool success_flag = false;

	if ((!value_p) || (json_is_null (value_p)))
		{
			pdsb_spinner_p -> ClearValue ();
			success_flag = true;
		}
	else if (json_is_number (value_p))
		{
			const double d = json_number_value (value_p);

			pdsb_spinner_p -> setValue (d);
			success_flag = true;
		}


	return success_flag;
}




QWidget *ParamDoubleSpinBox :: GetQWidget ()
{
	return pdsb_spinner_p;
}
