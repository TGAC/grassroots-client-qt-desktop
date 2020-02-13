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
	static double DS_UNSET_VALUE;

	DoubleSpinner (Parameter *param_p, QWidget *parent_p = nullptr);

	virtual ~DoubleSpinner ();

	QValidator::State validate (QString &input_r, int &pos_r) const;


protected:
	Parameter *ds_param_p;
	bool ds_value_set_flag;

	virtual double valueFromText (const QString &text_r) const override;

	virtual QString textFromValue (double value) const override;

};

#endif // DOUBLE_SPINNER_H