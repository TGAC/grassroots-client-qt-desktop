#ifndef UNSIGNED_INT_SPINNER_H
#define UNSIGNED_INT_SPINNER_H

#include <QSpinBox>
#include <QString>

#include "typedefs.h"
#include "parameter.h"


class UnsignedIntSpinner : public QSpinBox
{
	Q_OBJECT

public:
	UnsignedIntSpinner (Parameter *param_p, QWidget *parent_p = nullptr);

	virtual ~UnsignedIntSpinner ();

	QValidator::State validate (QString &input_r, int &pos_r) const override;

	bool IsValueSet () const;

	bool ClearValue ();

protected:
	Parameter *sis_param_p;
	mutable bool sis_null_flag;
	QValidator *uis_validator_p;

	virtual int valueFromText (const QString &text_r) const override;

	virtual QString textFromValue (int value) const override;

};


#endif // UUNNSIGNED_INT_SPINNER_H
