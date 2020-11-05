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


	//virtual QValidator::State validate (QString &input_r, int &pos_r) const override;

	void setValue (int i);

//	void setValue (uint32 u);

/*
	void setMinimum (uint32 min);

	void setMaximum (uint32 max);

	int minimum () const;

	int maximum () const;
*/

	uint32 GetValue () const;


protected:
	static int UIS_SPECIAL_VALUE;
	static const char *UIS_SPECIAL_VALUE_TEXT_S;
	bool uis_required_flag;
	mutable bool uis_null_flag;
	QValidator *uis_validator_p;

	virtual int valueFromText (const QString &text_r) const override;

	virtual QString textFromValue (int value) const override;

};


#endif // UUNNSIGNED_INT_SPINNER_H
