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
#include "param_group_box.h"
#include "qt_parameter_widget.h"


ParamGroupBox :: ParamGroupBox (const char * const name_s, const bool visible_flag, QTParameterWidget *qt_param_widget_p)
 : QGroupBox (name_s),
   pgb_parent_p (qt_param_widget_p)
{
  pgb_layout_p = new QFormLayout;

	setStyleSheet("QGroupBox { font-weight: bold; } ");

	setCheckable (true);
	setChecked (visible_flag);
	setLayout (pgb_layout_p);
	setAlignment (Qt :: AlignLeft);

	connect (this, &ParamGroupBox :: clicked, this, &ParamGroupBox :: ToggleCollapsed);
}


ParamGroupBox *ParamGroupBox :: Clone (const char * const name_s)
{
	ParamGroupBox *cloned_box_p = new ParamGroupBox (name_s, true, pgb_parent_p);

	for (i = pgb_children.count () - 1; i >= 0; -- i)
		{
			BaseParamWidget *src_widget_p = pgb_children.at (i) -> bpw_param_p;
			BaseParamWidget *dest_widget_p = pgb_parent_p -> CreateWidgetForParameter (src_widget_p -> GetParameter ());

			if (dest_widget_p)
				{
					cloned_box_p -> AddParameterWidget (param_widget_p);
				}
		}

	return cloned_box_p;
}



ParamGroupBox :: ~ParamGroupBox ()
{

}

void ParamGroupBox :: AddParameterWidget (BaseParamWidget *param_widget_p)
{
	QWidget *widget_p = param_widget_p -> GetUIQWidget ();
	QLabel *label_p = param_widget_p -> GetLabel ();

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
