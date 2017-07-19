#ifndef REPEATABLE_PARAM_GROUP_BOX_HPP
#define REPEATABLE_PARAM_GROUP_BOX_HPP

#include <QPushButton>
#include <QGroupBox>

#include "param_group_box.h"


class RepeatableParamGroupBox : public QGroupBox, public ParameterWidgetContainer
{
	Q_OBJECT

public:
	RepeatableParamGroupBox (const char * const name_s, const bool visible_flag);

	virtual ~RepeatableParamGroupBox ();

	virtual void AddParameterWidget (BaseParamWidget *widget_p);

	virtual QWidget *GetWidget ();

	virtual void CheckVisibility (ParameterLevel level);

protected:
	void paintEvent (QPaintEvent *event_p);

private:
	QPushButton *rpgb_add_row_btn_p;
	QList <ParamGroupBox *> rpgb_children;

private slots:
	void AddRow (bool clicked_flag = false);

};

#endif // REPEATABLE_PARAM_GROUP_BOX_HPP
