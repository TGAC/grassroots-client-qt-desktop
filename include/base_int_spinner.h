#ifndef BASE_INT_SPINNER_H
#define BASE_INT_SPINNER_H

#include <QSpinBox>
#include <QString>

#include "typedefs.h"
#include "parameter.h"


class BaseSpinner : public QAbstractSpinBox
{
	Q_OBJECT

public:
	BaseSpinner (Parameter *param_p, QWidget *parent_p = nullptr);

	virtual ~BaseSpinner ();

	bool IsValueSet () const;

	bool ClearValue ();

protected:
	Parameter *bis_param_p;
	mutable bool bis_null_flag;

};


#endif // BASE_INT_SPINNER_H
