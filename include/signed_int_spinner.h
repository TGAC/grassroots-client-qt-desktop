#ifndef SIGNED_INT_SPINNER_H
#define SIGNED_INT_SPINNER_H

#include <QSpinBox>
#include <QString>

#include "typedefs.h"
#include "parameter.h"


class SignedIntSpinner : public QSpinBox
{
	Q_OBJECT

public:
	static int SIS_UNSET_VALUE;

	SignedIntSpinner (Parameter *param_p, QWidget *parent_p = nullptr);

	virtual ~SignedIntSpinner ();

	QValidator::State validate (QString &input_r, int &pos_r) const override;

	bool IsValueSet () const;

	bool ClearValue ();

protected:
	Parameter *sis_param_p;
	mutable bool sis_null_flag;

	virtual int valueFromText (const QString &text_r) const override;

	virtual QString textFromValue (int value) const override;

};


#endif // SIGNED_INT_SPINNER_H
