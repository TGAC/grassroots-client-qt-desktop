#ifndef BASE_PARAM_SPIN_BOX_H
#define BASE_PARAM_SPIN_BOX_H



#include <QWidget>
#include <QSpinBox>

#include "parameter.h"
#include "base_param_widget.h"


class ClearableSpinBox : public QSpinBox
{
	Q_OBJECT

public:
	ClearableSpinBox (QWidget *parent_p = nullptr);

protected:
	void fixup(QString &input) const;
};


class BaseParamSpinBox : public BaseParamWidget
{
	Q_OBJECT

public:
	BaseParamSpinBox (Parameter * const param_p, QTParameterWidget * const parent_p);
	virtual ~BaseParamSpinBox ();

protected:
	virtual QWidget *GetQWidget ();

	QSpinBox *bpsb_spin_box_p;
};


#endif // BASE_PARAM_SPIN_BOX_H
