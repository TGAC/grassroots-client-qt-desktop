#ifndef DOUBLE_SPINNER_H
#define DOUBLE_SPINNER_H

#include <QDoubleSpinBox>
#include <QString>

#include "typedefs.h"
#include "parameter.h"


class DoubleSpinner : public QDoubleSpinBox
{
	Q_OBJECT

public:

	DoubleSpinner (Parameter *param_p, QWidget *parent_p = nullptr);

	virtual ~DoubleSpinner ();

//	QValidator::State validate (QString &input_r, int &pos_r) const override;

	void setValue (double d);

	bool IsValueSet () const;

	bool ClearValue ();

protected:
	static double DS_SPECIAL_VALUE;
	static const char *DS_SPECIAL_VALUE_TEXT_S;
	Parameter *ds_param_p;
	mutable bool ds_null_flag;


	virtual double valueFromText (const QString &text_r) const override;

	virtual QString textFromValue (double value) const override;

};

#endif // DOUBLE_SPINNER_H
