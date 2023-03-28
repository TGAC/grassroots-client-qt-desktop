#ifndef PARAM_DATE_WIDGET_H
#define PARAM_DATE_WIDGET_H


#include <QWidget>
#include <QDateTimeEdit>
#include <QCheckBox>

#include "parameter.h"
#include "base_param_widget.h"
#include "time_parameter.h"
#include "time_array_parameter.h"

class ParamDateWidget : public BaseParamWidget
{
	Q_OBJECT


private slots:
//	bool UpdateConfig (int state);

public:
	ParamDateWidget (TimeParameter * const param_p, QTParameterWidget * const parent_p0);
	ParamDateWidget (TimeArrayParameter * const param_p, QTParameterWidget * const parent_p0);

	virtual ~ParamDateWidget ();

	virtual void SetDefaultValue ();

	virtual bool SetValueFromText (const char *value_s);

	virtual bool SetValueFromJSON (const json_t * const value_p);

	virtual bool StoreParameterValue (bool refresh_flag);

protected:
	virtual QWidget *GetQWidget ();


private:
	TimeParameter *pdw_param_p;
	QWidget *pdw_root_widget_p;
	QDateTimeEdit  *pdw_calendar_p;
	QCheckBox *pdw_checkbox_p;
};


#endif // PARAM_DATE_WIDGET_H
