/*
 * string_list_widget.h
 *
 *  Created on: 11 Jan 2024
 *      Author: billy
 */

#ifndef CLIENTS_DESKTOP_CLIENT_INCLUDE_STRING_LIST_WIDGET_H_
#define CLIENTS_DESKTOP_CLIENT_INCLUDE_STRING_LIST_WIDGET_H_

#include <QListWidget>
#include <QWidget>

#include "string_array_parameter.h"
#include "qt_parameter_widget.h"



class StringListWidget : public BaseParamWidget
{
	Q_OBJECT


public:
	StringListWidget (StringArrayParameter * const param_p, QTParameterWidget * const parent_p);
	virtual ~StringListWidget ();

	virtual void SetDefaultValue ();

	virtual bool SetValueFromText (const char *value_s);

	virtual bool SetValueFromJSON (const json_t * const value_p);

	virtual bool StoreParameterValue (bool refresh_flag);

	virtual bool SetParameter (Parameter *param_p);

	virtual bool SetFromParameterValue (Parameter *param_p);

	void RemoveConnection ();

protected:
	virtual QWidget *GetQWidget ();


private:
	StringArrayParameter *slw_param_p;

  QListWidget *slw_list_p;

	void AddOptions (Parameter *param_p);

};



#endif /* CLIENTS_DESKTOP_CLIENT_INCLUDE_STRING_LIST_WIDGET_H_ */
