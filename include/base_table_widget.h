#ifndef BASE_TABLE_WIDGET_H
#define BASE_TABLE_WIDGET_H

#include <QScrollArea>

#include "base_param_widget.h"
#include "droppable_table_widget.h"


class BaseTableWidget : public BaseParamWidget
{
	Q_OBJECT

public:
	static const char * const PTW_COLUMN_HEADERS_S;

	BaseTableWidget (Parameter * const param_p, QTParameterWidget * const parent_p);
	virtual ~BaseTableWidget ();

	virtual void RemoveConnection ();

	virtual void SetDefaultValue ();

	virtual bool SetValueFromText (const char *value_s);

	virtual bool SetValueFromJSON (const json_t * const value_p);

	virtual void ShowErrors (const json_t *errors_p);

	virtual void Clear ();

	bool SetColumnHeaders (Parameter *param_p);

public slots:
	void ClearTable (bool triggered_flag = false);

protected:
	QScrollArea *ptw_scroller_p;
	DroppableTableWidget *ptw_table_p;
	char ptw_column_delimiter;
	char ptw_row_delimiter;

	virtual QWidget *GetQWidget ();
};


#endif // BASE_TABLE_WIDGET_H
