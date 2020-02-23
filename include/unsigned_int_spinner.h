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
	UnsignedIntSpinner (bool required_flag, QWidget *parent_p = nullptr);

	virtual ~UnsignedIntSpinner ();


	bool IsValueSet () const;

	bool ClearValue ();

	void SetValue (uint32 i);

	uint32 GetValue () const;

	void SetMinimum (uint32 min);

	void SetMaximum (uint32 max);

protected:
	bool uis_required_flag;
	mutable bool uis_null_flag;
	QValidator *uis_validator_p;

	virtual int valueFromText (const QString &text_r) const override;

	virtual QString textFromValue (int value) const override;

	virtual QValidator::State validate (QString &input_r, int &pos_r) const override;

	uint32 MapIntToUInt (int32 i) const;

	int32 MapUIntToInt (uint32 i) const;

};


#endif // UUNNSIGNED_INT_SPINNER_H
