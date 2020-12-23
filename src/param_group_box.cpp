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
#include <QFormLayout>
#include <QPushButton>

#include "param_group_box.h"
#include "qt_parameter_widget.h"
#include "parameter_group.h"
#include "repeatable_param_group_box.h"


ParamGroupBox :: ParamGroupBox (ParameterGroup *group_p, QTParameterWidget *qt_param_widget_p, bool removable_flag, bool add_params_flag)
 : QGroupBox (group_p -> pg_name_s),
	 pgb_parent_p (qt_param_widget_p),
	 pgb_parameter_group_p (group_p),
	 pgb_removable_flag (removable_flag)
{
	setStyleSheet("QGroupBox { font-weight: bold; } ");

	setCheckable (true);
	setChecked (group_p -> pg_visible_flag);
	setAlignment (Qt :: AlignLeft);

	pgb_layout_p = new QFormLayout;

	AddParamGroupWidgets (add_params_flag);
}


ParamGroupBox *ParamGroupBox :: Clone (ParameterGroup *group_p)
{
	ParamGroupBox *cloned_box_p = new ParamGroupBox (group_p, pgb_parent_p, pgb_removable_flag, true);

	AddParamGroupWidgets (true);

	return cloned_box_p;
}



ParamGroupBox :: ~ParamGroupBox ()
{

}

void ParamGroupBox :: init (bool add_params_flag)
{
	qDebug () << "ParamGroupBox :: init for " << pgb_parameter_group_p -> pg_name_s << Qt :: endl;

	setLayout (pgb_layout_p);
}



void ParamGroupBox :: AddParamGroupWidgets (bool add_params_flag)
{
	if (pgb_parameter_group_p -> pg_params_p)
		{
			ParameterNode *node_p = (ParameterNode *) (pgb_parameter_group_p -> pg_params_p -> ll_head_p);

			while (node_p)
				{
					Parameter *param_p = node_p -> pn_parameter_p;
					BaseParamWidget *widget_p = pgb_parent_p -> CreateWidgetForParameter (param_p, add_params_flag);

					if (widget_p)
						{
							AddParameterWidget (widget_p);
							pgb_parent_p -> ParameterWidgetAdded (param_p, widget_p);

							node_p = (ParameterNode *) (node_p -> pn_node.ln_next_p);
						}
					else
						{
							PrintErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, "Failed to add widget for \"%s\"", param_p -> pa_name_s);
						}
				}
		}
}

void ParamGroupBox :: StoreValues ()
{
	QList <BaseParamWidget *> :: Iterator i = pgb_children.begin ();

	for ( ; i != pgb_children.end (); ++ i)
		{
			BaseParamWidget *widget_p = *i;

			widget_p -> StoreParameterValue (false);
		}

}

void ParamGroupBox :: AddParameterWidget (BaseParamWidget *param_widget_p)
{
	QWidget *widget_p = param_widget_p -> GetUIQWidget ();
	QWidget *label_p = param_widget_p -> GetLabel ();

	pgb_layout_p -> addRow (label_p, widget_p);
	pgb_children.append (param_widget_p);

	if (! (isChecked ()))
		{
			param_widget_p -> SetVisible (false);
		}
}


QWidget *ParamGroupBox :: GetWidget ()
{
	return this;
}


void ParamGroupBox :: paintEvent (QPaintEvent *event_p)
{
	QGroupBox :: paintEvent (event_p);
}


void ParamGroupBox :: ToggleCollapsed (bool checked)
{
	if (checked)
		{
			for (int j = pgb_children.count () - 1; j >= 0; -- j)
				{
					BaseParamWidget *widget_p = pgb_children.at (j);

					widget_p -> SetVisible (true);
				}

		}
	else
		{
			for (int j = pgb_children.count () - 1; j >= 0; -- j)
				{
					BaseParamWidget *widget_p = pgb_children.at (j);

					widget_p -> SetVisible (false);
				}
		}
}


void ParamGroupBox :: CheckVisibility (ParameterLevel level)
{
	if (isHidden ())
		{
			bool any_visible_children_flag = false;

			for (int j = pgb_children.count () - 1; j >= 0; -- j)
				{
					BaseParamWidget *widget_p = pgb_children.at (j);

					if (widget_p -> MeetsLevel (level))
						{
							any_visible_children_flag = true;
							j = 0;
						}
				}

			if (any_visible_children_flag)
				{
					show ();
				}
		}
	else
		{
			bool any_visible_children_flag = false;

			for (int j = pgb_children.count () - 1; j >= 0; -- j)
				{
					BaseParamWidget *widget_p = pgb_children.at (j);

					if (widget_p -> MeetsLevel (level))
						{
							any_visible_children_flag = true;
							j = 0;
						}
				}

			if (!any_visible_children_flag)
				{
					hide ();
				}
		}
}


void ParamGroupBox :: ParamGroupBoxRemoved ()
{
	emit RemoveParamGroupBox (this);
}
