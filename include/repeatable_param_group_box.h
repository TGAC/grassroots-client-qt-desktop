#ifndef REPEATABLE_PARAM_GROUP_BOX_HPP
#define REPEATABLE_PARAM_GROUP_BOX_HPP

#include <QPushButton>
#include <QGroupBox>
#include <QListWidget>

#include "param_group_box.h"

class QTParameterWidget;

class RepeatableParamGroupBox : public ParamGroupBox
{
	Q_OBJECT

public:
	RepeatableParamGroupBox (ParameterGroup *group_p, QTParameterWidget *qt_param_widget_p, bool removable_flag, bool add_params_flag);

	virtual ~RepeatableParamGroupBox ();

protected:
	void init (bool add_params_flag) override;

private:
	QListWidget *rpgb_entries_p;
	Parameter *rpgb_label_param_p;

private slots:
	void AddEntry ();
	void RemoveEntry ();

};

#endif // REPEATABLE_PARAM_GROUP_BOX_HPP
