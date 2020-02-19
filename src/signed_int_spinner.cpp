
#include <climits>

#include "signed_int_spinner.h"

int SignedIntSpinner :: SIS_UNSET_VALUE = INT32_MIN;


SignedIntSpinner :: SignedIntSpinner (Parameter *param_p, QWidget *parent_p)
	: QSpinBox (parent_p)
{
	sis_param_p = param_p;
	sis_value_set_flag = false;
}


SignedIntSpinner :: ~SignedIntSpinner ()
{
}


int SignedIntSpinner :: valueFromText (const QString &text_r) const
{
	int i = SignedIntSpinner :: SIS_UNSET_VALUE;

	if (! ((text_r.isNull ()) || (text_r.isEmpty ())))
		{
			i = text_r.toInt (&sis_value_set_flag);
		}

	return i;
}


QString SignedIntSpinner :: textFromValue (int value) const
{
	if (sis_value_set_flag)
		{
			return QString :: number (value);
		}
	else
		{
			return QString ("");
		}
}



bool SignedIntSpinner :: IsValueSet () const
{
	return sis_value_set_flag;
}


QValidator :: State SignedIntSpinner :: validate (QString &input_r, int &pos_r) const
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
				}
			else
				{
					state = QSpinBox :: validate (input_r, pos_r);
				}
		}

	return state;
}
