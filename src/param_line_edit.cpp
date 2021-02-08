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
#include <QDebug>

#include "param_line_edit.h"
#include "prefs_widget.h"

#include "string_utils.h"


ParamLineEdit :: ParamLineEdit (StringParameter * const param_p, QTParameterWidget * const parent_p, QLineEdit :: EchoMode echo)
:		BaseParamWidget (& (param_p -> sp_base_param), parent_p)
{
	ple_param_p = param_p;

	ple_text_box_p = new QLineEdit (parent_p);
	ple_text_box_p -> setEchoMode (echo);

	if (bpw_param_p -> pa_refresh_service_flag)
		{
			QObject ::  connect (ple_text_box_p,  &QLineEdit :: textChanged, parent_p, &QTParameterWidget :: RefreshService);
		}

}


ParamLineEdit ::	~ParamLineEdit ()
{
	delete ple_text_box_p;
	ple_text_box_p = NULL;
}

void ParamLineEdit :: RemoveConnection ()
{
}


void ParamLineEdit :: SetDefaultValue ()
{
	const char *value_s = GetStringParameterDefaultValue (ple_param_p);

	ple_text_box_p -> setText (value_s);
}


QWidget *ParamLineEdit :: GetQWidget ()
{
	return ple_text_box_p;
}

void ParamLineEdit :: SetMaxLength (int l)
{
	ple_text_box_p -> setMaxLength (l);
}


bool ParamLineEdit :: SetValueFromText (const char *value_s)
{
	QString s = ple_text_box_p -> text ();
	qDebug () << "old " << s;

	ple_text_box_p -> setText (value_s);

	s = ple_text_box_p -> text ();
	qDebug () << "new " << s;

	return true;
}



bool ParamLineEdit :: SetValueFromJSON (const json_t * const value_p)
{
	bool success_flag = false;

	if (json_is_string (value_p))
		{
			const char *value_s = json_string_value (value_p);

			ple_text_box_p -> setText (value_s);
			success_flag = true;
		}
	else if (json_is_null (value_p))
		{
			ple_text_box_p -> setText (nullptr);
			success_flag = true;
		}
	else
		{
			PrintJSONToErrors (STM_LEVEL_WARNING, __FILE__, __LINE__, value_p, "Failed to set value for ParamLineEdit %s", bpw_param_name_s);
		}

	return success_flag;
}



bool ParamLineEdit :: StoreParameterValue (bool refresh_flag)
{
	bool success_flag = false;
	QString s = ple_text_box_p -> text ();
	QByteArray ba = s.toLocal8Bit ();
	const char *value_s = ba.constData ();

	if ((!(bpw_param_p -> pa_required_flag)) || refresh_flag || (!IsStringEmpty (value_s)))
		{
			if (GetErrorFlag ())
				{
					SetErrorMessage (nullptr);
				}

			success_flag = UpdateConfigValue (value_s);
		}
	else
		{
			SetErrorMessage (BaseParamWidget ::  BPW_REQUIRED_S);
		}

	return success_flag;
}


bool ParamLineEdit :: UpdateConfigValue (const char * const value_s)
{
	bool success_flag = SetStringParameterCurrentValue (ple_param_p, value_s);

	qDebug () << "Setting " << bpw_param_p -> pa_name_s << " to " << value_s;

	return success_flag;
}



