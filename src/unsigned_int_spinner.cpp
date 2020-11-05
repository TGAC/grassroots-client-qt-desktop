
#include <climits>

#include "QDebug"

#include "unsigned_int_spinner.h"
#include "unsigned_int_validator.h"



int UnsignedIntSpinner :: UIS_SPECIAL_VALUE = -1;
const char *UnsignedIntSpinner :: UIS_SPECIAL_VALUE_TEXT_S = "<no value>";

UnsignedIntSpinner :: UnsignedIntSpinner (bool required_flag, QWidget *parent_p)
	: QSpinBox (parent_p)
{
	uis_required_flag = required_flag;
	uis_null_flag = false;
	uis_validator_p = new UnsignedIntValidator (parent_p);

	if (required_flag)
		{
			setMinimum (0);
		}
	else
		{
			setMinimum (UIS_SPECIAL_VALUE);
			setSpecialValueText (UIS_SPECIAL_VALUE_TEXT_S);

		}

	setMaximum (INT_MAX);
}



UnsignedIntSpinner :: ~UnsignedIntSpinner ()
{
}

/*
void UnsignedIntSpinner :: setMinimum (uint32 min)
{
	int i = MapUIntToInt (min);
	QSpinBox :: setMinimum (i);
}


void UnsignedIntSpinner :: setMaximum (uint32 max)
{
	int i = MapUIntToInt (max);
	QSpinBox :: setMaximum (i);
}
*/

bool UnsignedIntSpinner :: ClearValue (	)
{
	bool success_flag = false;

	if (!uis_required_flag)
		{
			uis_null_flag = true;

			setValue (UIS_SPECIAL_VALUE);
		}

	return success_flag;
}

/*
void UnsignedIntSpinner :: SetValue (uint32 u)
{
	int i = MapUIntToInt (u);
	setValue (i);
}
*/

int UnsignedIntSpinner :: valueFromText (const QString &text_r) const
{
	int i = UnsignedIntSpinner :: UIS_SPECIAL_VALUE;

	if (! ((text_r.isNull ()) || (text_r.isEmpty ())))
		{
			bool success_flag;
			int j = text_r.toInt (&success_flag);

			if (success_flag)
				{
					if (j >= 0)
						{
							i = j;
							uis_null_flag = false;
						}
					else
						{
							uis_null_flag = true;
						}
				}
		}
	else if (uis_required_flag)
		{
			uis_null_flag = true;
		}

	qDebug () << "UnsignedIntSpinner :: valueFromText:  s \"" << text_r << "\" value " << i;

	return i;
}


QString UnsignedIntSpinner :: textFromValue (int value) const
{
	QString s = nullptr;

	if (uis_null_flag)
		{
			s = QString ();
		}
	else
		{
			s = QString ();
			s.setNum (value);
		}


	qDebug () << "UnsignedIntSpinner :: textFromValue:  s \"" << s << "\" value " << value;

	return s;
}



bool UnsignedIntSpinner :: IsValueSet () const
{
	return !uis_null_flag;
}

/*
QValidator :: State UnsignedIntSpinner :: validate (QString &input_r, int &pos_r) const
{
	QValidator :: State state = QValidator :: Invalid;

	if (uis_required_flag)
		{
			state = QSpinBox :: validate (input_r, pos_r);
		}
	else
		{
			if ((input_r.isNull ()) || (input_r.isEmpty ()))
				{
					state = QValidator :: Acceptable;
					uis_null_flag = true;
				}
			else
				{
					state = QSpinBox :: validate (input_r, pos_r);
				}
		}

	qDebug () << "validating " << input_r << " state " << state;

	return state;
}
*/

void UnsignedIntSpinner :: setValue (int i)
{
	qDebug () << "setting spinner to " << i << Qt :: endl;

	uis_null_flag = false;

	QSpinBox :: setValue (i);
}


uint32 UnsignedIntSpinner :: GetValue () const
{
	int i = value ();

	qDebug () << "getting spinner " << i << Qt :: endl;

	return i;
}

