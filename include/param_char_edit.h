#ifndef PARAM_CHAR_EDIT_H
#define PARAM_CHAR_EDIT_H


#include <QWidget>
#include <QLineEdit>

#include "char_parameter.h"

#include "base_param_widget.h"



class ParamCharEdit : public BaseParamWidget
{
	Q_OBJECT


private slots:
	bool UpdateConfigValue (const char * const value_s);

public:
	ParamCharEdit (CharParameter * const param_p, QTParameterWidget * const parent_p, QLineEdit :: EchoMode echo = QLineEdit :: Normal);
	virtual ~ParamCharEdit ();

	virtual void RemoveConnection ();

	virtual void SetDefaultValue ();

	virtual bool SetValueFromText (const char *value_s);


	virtual bool SetValueFromJSON (const json_t * const value_p);

	virtual bool StoreParameterValue (bool refresh_flag);

protected:
	QLineEdit *pce_text_box_p;
	CharParameter *pce_param_p;

	virtual QWidget *GetQWidget ();
};

#endif // PARAM_CHAR_EDIT_H
