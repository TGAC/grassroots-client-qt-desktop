
#include <climits>

#include "signed_int_spinner.h"

int BaseIntSpinner :: SIS_UNSET_VALUE = INT32_MIN;


BaseIntSpinner :: BaseIntSpinner (Parameter *param_p, QWidget *parent_p)
	: QSpinBox (parent_p)
{
	sis_param_p = param_p;
	sis_null_flag = false;
}


BaseIntSpinner :: ~BaseIntSpinner ()
{
}


bool BaseIntSpinner :: ClearValue (	)
{
	bool success_flag = false;

	if (! (sis_param_p -> pa_required_flag))
		{
			QString nullstr;

			valueFromText (nullstr);
			sis_null_flag = true;
		}

	return success_flag;
}


int BaseIntSpinner :: valueFromText (const QString &text_r) const
{
	int i = BaseIntSpinner :: SIS_UNSET_VALUE;

	if (! ((text_r.isNull ()) || (text_r.isEmpty ())))
		{
			bool b;

			i = text_r.toInt (&b);
			sis_null_flag = !b;
		}
	else if (! (sis_param_p -> pa_required_flag))
		{
			sis_null_flag = true;
		}

	return i;
}


QString BaseIntSpinner :: textFromValue (int value) const
{
	if (sis_null_flag)
		{
			return QString ();
		}
	else
		{
			return QString :: number (value);
		}
}



bool BaseIntSpinner :: IsValueSet () const
{
	return !sis_null_flag;
}


QValidator :: State BaseIntSpinner :: validate (QString &input_r, int &pos_r) const
{
	QValidator :: State state = QValidator :: Invalid;

	if (sis_param_p -> pa_required_flag)
		{
			state = QSpinBox :: validate (input_r, pos_r);
		}
	else
		{
			if ((input_r.isNull ()) || (input_r.isEmpty ()))
				{
					state = QValidator :: Acceptable;
					sis_null_flag = true;
				}
			else
				{
					state = QSpinBox :: validate (input_r, pos_r);
				}
		}

	return state;
}
