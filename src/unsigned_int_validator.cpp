#include "unsigned_int_validator.h"


UnsignedIntValidator :: UnsignedIntValidator (QWidget *parent_p)
	: QValidator (nullptr)
{

}

UnsignedIntValidator :: ~UnsignedIntValidator ()
{

}


QValidator :: State UnsignedIntValidator :: validate (QString &input_r, int &pos_r) const
{
	QValidator :: State state = QValidator :: Invalid;
	QString s = input_r.trimmed ();

	if (s.isEmpty ())
		{
			state = QValidator :: Intermediate;
		}
	else
		{
			bool uint_flag;
			uint u = input_r.toUInt (&uint_flag);

			if (uint_flag)
				{
					state = QValidator :: Acceptable;
				}
			else
				{
					state = QValidator :: Invalid;
				}
		}

	return state;
}
