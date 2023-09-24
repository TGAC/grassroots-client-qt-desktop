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

#include "param_char_edit.h"
#include "prefs_widget.h"

#include "string_utils.h"


ParamCharEdit :: ParamCharEdit (CharParameter * const param_p, QTParameterWidget * const parent_p, QLineEdit :: EchoMode echo)
:		BaseParamWidget (& (param_p -> cp_base_param), parent_p)
{
	pce_param_p = param_p;

	pce_text_box_p = new QLineEdit (parent_p);
	pce_text_box_p -> setEchoMode (echo);

	pce_text_box_p -> setMaxLength (1);

	if (bpw_param_p -> pa_refresh_service_flag)
		{
			QObject ::  connect (pce_text_box_p,  &QLineEdit :: textChanged, parent_p, &QTParameterWidget :: RefreshService);
		}

}


ParamCharEdit ::	~ParamCharEdit ()
{
	delete pce_text_box_p;
	pce_text_box_p = NULL;
}

void ParamCharEdit :: RemoveConnection ()
{
}


bool ParamCharEdit :: SetParameter (Parameter *param_p)
{
	bool success_flag = false;

	if (IsCharParameter (param_p))
		{
			if (BaseParamWidget :: SetParameter (param_p))
				{
					CharParameter *char_param_p = (CharParameter *) param_p;
					const char *value_s = GetCharParameterCurrentValue (char_param_p);

					if (value_s)
						{
							if (SetValueFromText (value_s))
								{
									pce_param_p = char_param_p;

									success_flag = true;

								}
						}
				}
		}

	return success_flag;
}



void ParamCharEdit :: SetDefaultValue ()
{
	const char *def_p = GetCharParameterDefaultValue (pce_param_p);

	if (def_p)
		{
			char value_s [2];

			*value_s = *def_p;
			* (value_s + 1) = '\0';

			pce_text_box_p -> setText (value_s);
		}
	else
		{
			pce_text_box_p -> setText (nullptr);
		}

}


QWidget *ParamCharEdit :: GetQWidget ()
{
	return pce_text_box_p;
}


bool ParamCharEdit :: SetValueFromText (const char *value_s)
{
	bool success_flag = false;

	if (value_s)
		{
			if (strlen (value_s) == 1)
				{
					QString s = pce_text_box_p -> text ();
					qDebug () << "old " << s;

					pce_text_box_p -> setText (value_s);

					s = pce_text_box_p -> text ();
					qDebug () << "new " << s;

					success_flag = true;
				}
		}
	else
		{
			pce_text_box_p -> setText (nullptr);
			success_flag = true;
		}

	return true;
}



bool ParamCharEdit :: SetValueFromJSON (const json_t * const value_p)
{
	bool success_flag = false;

	if (json_is_string (value_p))
		{
			const char *value_s = json_string_value (value_p);

			pce_text_box_p -> setText (value_s);
			success_flag = true;
		}

	return success_flag;
}



bool ParamCharEdit :: StoreParameterValue (bool refresh_flag)
{
	bool success_flag = false;
	QString s = pce_text_box_p -> text ();
	QByteArray ba = s.toLocal8Bit ();
	const char *value_s = ba.constData ();

    if ((!IsRequired ()) || refresh_flag || (!IsStringEmpty (value_s)))
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


bool ParamCharEdit :: UpdateConfigValue (const char * const value_s)
{
	bool success_flag = false;

	if (value_s)
		{
			if (strlen (value_s))
				{
					success_flag = SetCharParameterCurrentValue (pce_param_p, value_s);
				}
		}
	else
		{
			success_flag = SetCharParameterCurrentValue (pce_param_p, nullptr);
		}


	return success_flag;
}



