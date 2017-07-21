#ifndef REPEATABLE_PARAM_GROUP_BOX_HPP
#define REPEATABLE_PARAM_GROUP_BOX_HPP

#include <QPushButton>
#include <QGroupBox>

#include "param_group_box.h"

class RepeatableParamGroupBox : public QGroupBox, public ParameterWidgetContainer
{
	Q_OBJECT

public:
	RepeatableParamGroupBox (ParameterGroup *group_p, QTParameterWidget *qt_param_widget_p);

	virtual ~RepeatableParamGroupBox ();

	virtual void AddParameterWidget (BaseParamWidget *widget_p);

	virtual QWidget *GetWidget ();

	virtual void CheckVisibility (ParameterLevel level);

public slots:
	void ParamGroupBoxRemoved (ParamGroupBox *box_p);


protected:
	void paintEvent (QPaintEvent *event_p);

private:
	QPushButton *rpgb_add_row_btn_p;
	QList <ParamGroupBox *> rpgb_children;
	ParameterGroup *rpgb_parameter_group_p;
	QGroupBox *rpgb_parent_box_p;
	QTParameterWidget *rpgb_qt_param_widget_parent_p;

private slots:
	void AddRow (bool clicked_flag = false);
};

#endif // REPEATABLE_PARAM_GROUP_BOX_HPP
