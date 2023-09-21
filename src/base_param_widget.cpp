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
#undef _CRTDBG_MAP_ALLOC
#include "base_param_widget.h"
#include "qt_parameter_widget.h"

#include "QtGui"

#include "string_utils.h"


const char * const BaseParamWidget ::  BPW_REQUIRED_S = "This field is required";


BaseParamWidget	:: BaseParamWidget (Parameter * const param_p, QTParameterWidget * const parent_p)
: bpw_param_p (nullptr),
	bpw_parent_p (parent_p),
	bpw_error_flag (false),
	bpw_param_name_s (nullptr),
	bpw_label_p (nullptr)
{
	SetParameter (param_p);
}


BaseParamWidget :: ~BaseParamWidget ()
{
	if (bpw_param_name_s)
		{
			FreeCopiedString (bpw_param_name_s);
		}
}


bool BaseParamWidget :: SetParameter (Parameter *param_p)
{
	bool success_flag = true;
	bpw_param_name_s = EasyCopyToNewString (param_p -> pa_name_s);
	const char *label_s = GetUIName (param_p);

	if (param_p -> pa_required_flag)
		{
			char *title_s = ConcatenateStrings (label_s, " *");

			if (title_s)
				{
					if (bpw_label_p)
						{
							bpw_label_p -> SetText (title_s);
						}
					else
						{
							bpw_label_p = new LabelsWidget (title_s, bpw_parent_p);
						}

					FreeCopiedString (title_s);
				}
		}
	else
		{
			if (bpw_label_p)
				{
					bpw_label_p -> SetText (label_s);
				}
			else
				{
					bpw_label_p = new LabelsWidget (label_s,  bpw_parent_p);
				}
		}

	if (param_p -> pa_description_s)
		{
			bpw_label_p -> setToolTip (param_p -> pa_description_s);
		}

	bpw_param_p = param_p;

	return success_flag;
}


const char *BaseParamWidget :: GetParameterName () const
{
	return bpw_param_name_s;
}


void BaseParamWidget :: RemoveConnection ()
{

}


bool BaseParamWidget :: SetParameter (Parameter *param_p)
{
	if (bpw_param_name_s)
		{
			if (strcmp (bpw_param_name_s, param_p -> pa_name_s) != 0)
				{
					char *new_param_name_s = EasyCopyToNewString (param_p -> pa_name_s);
					
					if (new_param_name_s)
						{
							FreeCopiedString (bpw_param_name_s);
							bpw_param_name_s = new_param_name_s;
						}
				}	
		}
	else
		{
			
		}
		
	


	
	const char *label_s = GetUIName (param_p);


	if (param_p -> pa_required_flag)
		{
			char *title_s = ConcatenateStrings (label_s, " *");

			if (title_s)
				{
					bpw_label_p = new LabelsWidget (title_s,  parent_p);
					FreeCopiedString (title_s);
				}
		}
	else
		{
			bpw_label_p = new LabelsWidget (label_s, parent_p);
		}

	if (param_p -> pa_description_s)
		{
			bpw_label_p -> setToolTip (param_p -> pa_description_s);
		}

}


bool BaseParamWidget :: IsComboBoxWidget () const
{
	return false;
}


bool BaseParamWidget :: IsRequired ()
{
	bool required_flag = false;
	ParameterGroup *group_p = bpw_param_p -> pa_group_p;

	if (group_p)
		{
			if (! (group_p -> pg_repeatable_flag))
				{
					required_flag = bpw_param_p -> pa_required_flag;
				}
		}

	return required_flag;
}


QWidget *BaseParamWidget :: GetLabel () const
{
	return bpw_label_p;
}


const Parameter *BaseParamWidget :: GetParameter () const
{
	return bpw_param_p;
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

			if (!error_flag)
				{
					bpw_label_p -> ClearError ();
				}
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



void BaseParamWidget :: SetErrorMessage (const char *message_s)
{
	if (message_s)
		{
			bpw_label_p -> SetError (message_s);
			bpw_error_flag = true;
		}
	else
		{
			bpw_label_p -> ClearError ();
			bpw_error_flag = false;
		}
}


void BaseParamWidget :: SetErrors (const json_t *errors_p)
{
	QString s;
	const json_t *entry_p;
	size_t i;

	s.append ("<ul>");
	json_array_foreach (errors_p, i, entry_p)
	{
		if (json_is_string (entry_p))
			{
				const char *value_s = json_string_value (entry_p);

				s.append ("<li>");
				s.append (value_s);
				s.append ("</li>");
			}
	}

	s.append ("</ul>");

	bpw_label_p -> SetError (s);
}
