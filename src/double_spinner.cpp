
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
	double d = 0.0f;

	if (! ((text_r.isNull ()) || (text_r.isEmpty ())))
		{
			bool success_flag = false;

			d = text_r.toDouble (&success_flag);
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


/*
 *

QVariant QSpinBoxPrivate::validateAndInterpret(QString &input, int &pos,
																							 QValidator::State &state) const
{
		if (cachedText == input && !input.isEmpty()) {
				state = cachedState;
				QSBDEBUG() << "cachedText was '" << cachedText << "' state was "
									 << state << " and value was " << cachedValue;

				return cachedValue;
		}
		const int max = maximum.toInt();
		const int min = minimum.toInt();

		QString copy = stripped(input, &pos);
		QSBDEBUG() << "input" << input << "copy" << copy;
		state = QValidator::Acceptable;
		int num = min;

		if (max != min && (copy.isEmpty()
											 || (min < 0 && copy == QLatin1String("-"))
											 || (max >= 0 && copy == QLatin1String("+")))) {
				state = QValidator::Intermediate;
				QSBDEBUG() << __FILE__ << __LINE__<< "num is set to" << num;
		} else if (copy.startsWith(QLatin1Char('-')) && min >= 0) {
				state = QValidator::Invalid; // special-case -0 will be interpreted as 0 and thus not be invalid with a range from 0-100
		} else {
				bool ok = false;
				if (displayIntegerBase != 10) {
						num = copy.toInt(&ok, displayIntegerBase);
				} else {
						num = locale.toInt(copy, &ok);
						if (!ok && (max >= 1000 || min <= -1000)) {
								const QString sep(locale.groupSeparator());
								const QString doubleSep = sep + sep;
								if (copy.contains(sep) && !copy.contains(doubleSep)) {
										QString copy2 = copy;
										copy2.remove(sep);
										num = locale.toInt(copy2, &ok);
								}
						}
				}
				QSBDEBUG() << __FILE__ << __LINE__<< "num is set to" << num;
				if (!ok) {
						state = QValidator::Invalid;
				} else if (num >= min && num <= max) {
						state = QValidator::Acceptable;
				} else if (max == min) {
						state = QValidator::Invalid;
				} else {
						if ((num >= 0 && num > max) || (num < 0 && num < min)) {
								state = QValidator::Invalid;
								QSBDEBUG() << __FILE__ << __LINE__<< "state is set to Invalid";
						} else {
								state = QValidator::Intermediate;
								QSBDEBUG() << __FILE__ << __LINE__<< "state is set to Intermediate";
						}
				}
		}
		if (state != QValidator::Acceptable)
				num = max > 0 ? min : max;
		input = prefix + copy + suffix;
		cachedText = input;
		cachedState = state;
		cachedValue = QVariant((int)num);

		QSBDEBUG() << "cachedText is set to '" << cachedText << "' state is set to "
							 << state << " and value is set to " << cachedValue;
		return cachedValue;
}

*/

QValidator :: State DoubleSpinner :: validate (QString &input_r, int &pos_r) const
{
	QValidator :: State state = QValidator :: Invalid;

	if (! (ds_param_p -> pa_required_flag))
		{
			if (!input_r.isNull ())
				{
					QString s = input_r.trimmed ();

					if (s.isEmpty ())
						{
							state = QValidator :: Acceptable;
						}
				}
		}
	else
		{
			state = QDoubleSpinBox :: validate (input_r, pos_r);
		}

	return state;
}
