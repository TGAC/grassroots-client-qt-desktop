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

	virtual void Init (bool add_params_flag);

	json_t *GetParametersAsJSON ();

	json_t *GetParametersAsGroupJSON ();

protected:

private:
	QListWidget *rpgb_entries_p;
	Parameter *rpgb_label_param_p;
	QPushButton *rpgb_remove_entry_button_p;

private slots:
	void AddEntry ();
	void RemoveEntry ();
	void SelectedListEntryChanged ();

};

#endif // REPEATABLE_PARAM_GROUP_BOX_HPP
