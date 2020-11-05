
#include <cfloat>

#include "double_spinner.h"

double DoubleSpinner :: DS_SPECIAL_VALUE = -DBL_MAX;
const char *DoubleSpinner :: DS_SPECIAL_VALUE_TEXT_S = "<no value>";


DoubleSpinner :: DoubleSpinner (Parameter *param_p, QWidget *parent_p)
	: QDoubleSpinBox (parent_p)
{
	ds_param_p = param_p;
	ds_null_flag = false;

	if (! (ds_param_p -> pa_required_flag))
		{
			setSpecialValueText (DS_SPECIAL_VALUE_TEXT_S);
		}
}


DoubleSpinner :: ~DoubleSpinner ()
{
}


bool DoubleSpinner :: ClearValue (	)
{
	bool success_flag = false;

	if (! (ds_param_p -> pa_required_flag))
		{
			ds_null_flag = true;
			setValue (DoubleSpinner :: DS_SPECIAL_VALUE);
		}

	return success_flag;
}


double DoubleSpinner :: valueFromText (const QString &text_r) const
{
	double d = DoubleSpinner :: DS_SPECIAL_VALUE;

	if (! ((text_r.isNull ()) || (text_r.isEmpty ())))
		{
			bool b;

			d = text_r.toDouble (&b);
			ds_null_flag = !b;
		}
	else if (! (ds_param_p -> pa_required_flag))
		{
			ds_null_flag = true;
		}

	return d;
}


QString DoubleSpinner :: textFromValue (double value) const
{
	if (ds_null_flag)
		{
			return QString ();
		}
	else
		{
			return QString :: number (value);
		}
}



bool DoubleSpinner :: IsValueSet () const
{
	return !ds_null_flag;
}


/*
QValidator :: State DoubleSpinner :: validate (QString &input_r, int &pos_r) const
{
	QValidator :: State state = QValidator :: Invalid;

	if (ds_param_p -> pa_required_flag)
		{
			state = QDoubleSpinBox :: validate (input_r, pos_r);
		}
	else
		{
			if ((input_r.isNull ()) || (input_r.isEmpty ()))
				{
					state = QValidator :: Acceptable;
					ds_null_flag = true;
				}
			else
				{
					state = QDoubleSpinBox :: validate (input_r, pos_r);
				}
		}

	return state;
}
*/
