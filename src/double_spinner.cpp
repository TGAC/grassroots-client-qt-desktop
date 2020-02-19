
#include <cfloat>

#include "double_spinner.h"

double DoubleSpinner :: DS_UNSET_VALUE = -DBL_MAX;


DoubleSpinner :: DoubleSpinner (Parameter *param_p, QWidget *parent_p)
	: QDoubleSpinBox (parent_p)
{
	ds_param_p = param_p;
	ds_value_set_flag = false;
}


DoubleSpinner :: ~DoubleSpinner ()
{
}


double DoubleSpinner :: valueFromText (const QString &text_r) const
{
	double d = DoubleSpinner :: DS_UNSET_VALUE;

	if (! ((text_r.isNull ()) || (text_r.isEmpty ())))
		{
			d = text_r.toDouble (&ds_value_set_flag);
		}

	return d;
}


QString DoubleSpinner :: textFromValue (double value) const
{
	if (ds_value_set_flag)
		{
			return QString :: number (value);
		}
	else
		{
			return QString ("");
		}
}



bool DoubleSpinner :: IsValueSet () const
{
	return ds_value_set_flag;
}


QValidator :: State DoubleSpinner :: validate (QString &input_r, int &pos_r) const
{
	bool validated_flag = false;
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
				}
			else
				{
					state = QDoubleSpinBox :: validate (input_r, pos_r);
				}
		}

	return state;
}
