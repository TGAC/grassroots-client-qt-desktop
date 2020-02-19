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
#include "base_param_widget.h"
#include "qt_parameter_widget.h"

#include "QtGui"

#include "string_utils.h"


BaseParamWidget	:: BaseParamWidget (Parameter * const param_p, QTParameterWidget * const parent_p)
	: bpw_param_p (param_p),
		bpw_parent_p (parent_p),
		bpw_error_flag (false)
{
	bpw_param_name_s = EasyCopyToNewString (param_p -> pa_name_s);
	const char *label_s = GetUIName (param_p);


	if (param_p -> pa_required_flag)
		{
			QString s (label_s);
			s.append (" *");
			bpw_label_p = new QLabel (s);
		}
	else
		{
			bpw_label_p = new QLabel (label_s);
		}

	if (param_p -> pa_description_s)
		{
			bpw_label_p -> setToolTip (param_p -> pa_description_s);
		}
}


BaseParamWidget *BaseParamWidget :: Clone (ParameterGroup *group_p) const
{
	Parameter *dest_param_p = CloneParameter (bpw_param_p);

	if (dest_param_p)
		{
			BaseParamWidget *dest_widget_p = nullptr;

			if (group_p)
				{
					dest_param_p -> pa_group_p = group_p;
				}

			dest_widget_p = bpw_parent_p -> CreateWidgetForParameter (dest_param_p, true);

			if (dest_widget_p)
				{
					return dest_widget_p;
				}
			else
				{
					FreeParameter (dest_param_p);
				}
		}

	return nullptr;
}


BaseParamWidget :: ~BaseParamWidget ()
{
	if (bpw_param_name_s)
		{
			FreeCopiedString (bpw_param_name_s);
		}
}


const char *BaseParamWidget :: GetParameterName () const
{
	return bpw_param_name_s;
}


void BaseParamWidget :: RemoveConnection ()
{

}



QLabel *BaseParamWidget :: GetLabel () const
{
	return bpw_label_p;
}


bool BaseParamWidget :: MeetsLevel (const ParameterLevel ui_level) const
{
	return CompareParameterLevels (bpw_param_p -> pa_level, ui_level);
}


void BaseParamWidget :: SetVisible (const bool visible_flag)
{
	QWidget *widget_p = GetQWidget ();

	if (widget_p)
		{
			widget_p -> setVisible (visible_flag);
		}

	bpw_label_p -> setVisible (visible_flag);
}


void BaseParamWidget :: SetErrorFlag (const bool error_flag)
{
	if (bpw_error_flag != error_flag)
		{
			const char *style_s = nullptr;

			if (error_flag)
				{
					style_s = "QLabel {color: rgb(170,0,0);}";
				}
			else
				{
					style_s = "QLabel {color: rgb(0,0,0);}";
				}

			bpw_label_p -> setStyleSheet (style_s);

			bpw_error_flag = error_flag;
		}
}

bool BaseParamWidget :: GetErrorFlag () const
{
	return bpw_error_flag;
}


void BaseParamWidget :: CheckLevelDisplay (const ParameterLevel ui_level, const QWidget * const parent_widget_p)
{
	QWidget *this_widget_p = GetQWidget ();

	if (this_widget_p)
		{
			if (!CompareParameterLevels (bpw_param_p -> pa_level, ui_level))
				{
					//if (this_widget_p -> isVisibleTo (parent_widget_p))
					//	{
							this_widget_p -> hide ();

							if (bpw_label_p)
								{
									bpw_label_p -> hide ();
								}
					//	}
				}
			else
				{
					//if (! (this_widget_p -> isVisibleTo (parent_widget_p)))
					//	{
							this_widget_p -> show ();

							if (bpw_label_p)
								{
									bpw_label_p -> show ();
								}
					//	}
				}
		}
}


QWidget *BaseParamWidget :: GetUIQWidget ()
{
	QWidget *w_p = GetQWidget ();

	if (w_p)
		{
			if (bpw_param_p -> pa_description_s)
				{
					w_p -> setToolTip (bpw_param_p -> pa_description_s);
				}
		}

	return w_p;
}



void BaseParamWidget :: ShowErrors (const json_t *errors_p)
{

}
