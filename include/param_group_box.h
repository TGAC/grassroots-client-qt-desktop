/*
** Copyright 2014-2016 The Earlham Institute
** 
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
** 
**     http://www.apache.org/licenses/LICENSE-2.0
** 
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

/**
 * @file
 * @brief
 */
#ifndef PARAM_GROUP_BOX_H
#define PARAM_GROUP_BOX_H

#include <QFormLayout>
#include <QGroupBox>
#include <QObject>
#include <QListWidget>

#include "base_param_widget.h"
#include "parameter_widget_container.h"

class QTParameterWidget;

class ParamGroupBox : public QGroupBox, public ParameterWidgetContainer
{
	Q_OBJECT

signals:
	void RemoveParamGroupBox (ParamGroupBox *box_p);

public:
	ParamGroupBox (ParameterGroup *group_p, QTParameterWidget *qt_param_widget_p, bool removable_flag, bool add_params_flag);

	virtual ~ParamGroupBox ();


	ParamGroupBox *Clone (ParameterGroup *group_p);

	virtual void AddParameterWidget (BaseParamWidget *widget_p);

	virtual QWidget *GetWidget ();

	virtual void CheckVisibility (ParameterLevel level);

	void AddParamGroupWidgets (bool add_params_flag);

	virtual void init (bool add_params_flag);

	const QList <BaseParamWidget *> *GetChildren ();

protected:
	QList <BaseParamWidget *> pgb_children;
	QFormLayout *pgb_layout_p;
	QTParameterWidget *pgb_parent_p;
	ParameterGroup *pgb_parameter_group_p;
	bool pgb_removable_flag;

	void StoreValues ();

	void paintEvent (QPaintEvent *event_p);


private slots:
	void ToggleCollapsed (bool checked);
	void ParamGroupBoxRemoved ();
};


#endif // PARAM_GROUP_BOX_H
