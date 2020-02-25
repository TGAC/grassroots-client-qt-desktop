#ifndef JSON_TABLE_WIDGET_H
#define JSON_TABLE_WIDGET_H

#include "base_table_widget.h"

#include "json_parameter.h"


class JSONTableWidget : public BaseTableWidget
{
	Q_OBJECT

public:
	JSONTableWidget (JSONParameter * const param_p, QTParameterWidget * const parent_p);
	virtual ~JSONTableWidget ();

	virtual void SetDefaultValue ();

	virtual bool SetValueFromText (const char *value_s);

	virtual bool SetValueFromJSON (const json_t * const value_p);

	virtual bool StoreParameterValue (bool refresh_flag);

	virtual void Clear ();

protected:
	JSONParameter *jtw_param_p;
};

#endif // JSON_TABLE_WIDGET_H
