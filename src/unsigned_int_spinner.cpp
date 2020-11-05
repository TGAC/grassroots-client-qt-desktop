
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

	setMinimum (UIS_SPECIAL_VALUE);
	setMaximum (INT_MAX);

	setSpecialValueText (UIS_SPECIAL_VALUE_TEXT_S);
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
			QString nullstr;

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
	int i = 0;

	if (! ((text_r.isNull ()) || (text_r.isEmpty ())))
		{
			bool success_flag;
			uint32 u = text_r.toUInt (&success_flag);

			if (success_flag)
				{
					i = MapUIntToInt (u);
				}

			uis_null_flag = !success_flag;
		}
	else if (uis_required_flag)
		{
			uis_null_flag = true;
		}

	return i;
}


QString UnsignedIntSpinner :: textFromValue (int value) const
{
	if (uis_null_flag)
		{
			return QString ();
		}
	else
		{
			uint32 u = MapIntToUInt (value);
			QString s;

			s.setNum (u);
			return s;
		}
}



bool UnsignedIntSpinner :: IsValueSet () const
{
	return !uis_null_flag;
}


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


uint32 UnsignedIntSpinner :: MapIntToUInt (int32 i) const
{
	uint32 *u_p = reinterpret_cast <uint32 *> (&i);
	return *u_p;
}


int32 UnsignedIntSpinner :: MapUIntToInt (uint32 u) const
{
	int32 *i_p = reinterpret_cast <int32 *> (&u);
	return *i_p;
}

/*
void UnsignedIntSpinner :: setValue (uint32 value)
{
	int i = MapUIntToInt (value);

	qDebug () << "setting spinner to " << i << " from " << value << Qt :: endl;

	QSpinBox :: setValue (i);
}
*/

uint32 UnsignedIntSpinner :: GetValue () const
{
	int i = value ();
	uint32 u = MapIntToUInt (i);

	qDebug () << "getting spinner " << u << " from " << i << Qt :: endl;

	return u;
}

