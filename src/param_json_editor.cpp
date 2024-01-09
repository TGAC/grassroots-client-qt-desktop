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
#include <QDialog>

#include "param_json_editor.h"

#include "jansson.h"

#include "json_util.h"
#include "string_utils.h"
#include "qt_client_data.h"


DroppableJSONBox :: DroppableJSONBox (QTParameterWidget *parent_p)
	: DroppableTextBox (parent_p)
{}


bool DroppableJSONBox :: SetFromText (const char * const data_s)
{
	bool success_flag = false;
	json_error_t error;
	json_t *data_p = json_loads (data_s, 0, &error);

	if (data_p)
		{
			clear ();
			insertPlainText (data_s);
			success_flag = true;

			json_decref (data_p);
		}

	return success_flag;
}


bool DroppableJSONBox :: SetFromJSON (const json_t * const value_p)
{
	bool success_flag = false;

	if (json_is_null (value_p))
		{
			clear ();
			success_flag = true;
		}
	else
		{
			char *data_s = json_dumps (value_p, 0);

			clear ();

			if (data_s)
				{
					insertPlainText (data_s);
					success_flag = true;

					free (data_s);
				}
		}

	return success_flag;
}


ParamJSONEditor :: ParamJSONEditor (JSONParameter * const param_p, QTParameterWidget * const parent_p)
:	BaseParamWidget (& (param_p -> jp_base_param), parent_p)
{
	pje_param_p = param_p;
	pje_text_box_p = new DroppableJSONBox (parent_p);
}



bool ParamJSONEditor :: CreateDroppableTextBox (QTParameterWidget *parent_p)
{
	pje_text_box_p = new DroppableJSONBox (parent_p);

	return true;
}



ParamJSONEditor :: ~ParamJSONEditor ()
{

}


QWidget *ParamJSONEditor :: GetQWidget ()
{
	return pje_text_box_p;
}


void ParamJSONEditor :: SetDefaultValue ()
{
	const json_t *value_p = GetJSONParameterDefaultValue (pje_param_p );

	if (!IsJSONEmpty (value_p))
		{
			char *value_s = json_dumps (value_p, JSON_INDENT (2) | JSON_PRESERVE_ORDER);

			if (value_s)
				{
					pje_text_box_p -> setPlainText (value_s);
					free (value_s);
				}
		}
}



bool ParamJSONEditor :: StoreParameterValue (bool refresh_flag)
{
	bool success_flag = false;
	QString s = pje_text_box_p -> toPlainText ();
	QByteArray ba = s.toLocal8Bit ();
	const char *value_s = ba.constData ();

	if (!IsStringEmpty (value_s))
		{
			json_error_t error;
			json_t *data_p = json_loads (value_s, 0, &error);

			if (data_p)
				{
					success_flag = SetJSONParameterCurrentValue (pje_param_p, data_p);
					json_decref (data_p);
				}
			else
				{
					success_flag = SetJSONParameterCurrentValue (pje_param_p, nullptr);
				}
		}
	else
		{
			success_flag = SetJSONParameterCurrentValue (pje_param_p, nullptr);
		}

	if (bpw_parent_p -> GetClientData () -> qcd_verbose_flag)
		{
			qDebug () << "Setting " << bpw_param_p -> pa_name_s << " to " << value_s << " " << success_flag;
		}

	if (!success_flag)
		{

		}

	return success_flag;
}



bool ParamJSONEditor :: SetValueFromText (const char *value_s)
{
	return pje_text_box_p -> SetFromText (value_s);
}


bool ParamJSONEditor :: SetFromParameterValue (Parameter *param_p)
{
	bool success_flag = false;

	if (IsJSONParameter (param_p))
		{
			JSONParameter *json_param_p = reinterpret_cast <JSONParameter *> (param_p);
			const json_t *value_p = GetJSONParameterCurrentValue (json_param_p);

			success_flag = SetValueFromJSON (value_p);
		}

	return success_flag;
}


bool ParamJSONEditor :: SetValueFromJSON (const json_t * const param_value_p)
{
	bool success_flag = false;
	bool verbose_flag = bpw_parent_p -> GetClientData () -> qcd_verbose_flag;

	//	const json_t *param_value_p = json_object_get (value_p, PARAM_CURRENT_VALUE_S);

	if ((!param_value_p) || (json_is_null (param_value_p)))
		{
			if (verbose_flag)
				{
					PrintLog (STM_LEVEL_INFO, __FILE__, __LINE__, "ParamJSONEditor :: SetValueFromJSON on widget \"%s\" with NULL\n", GetParameterName ());
				}

			success_flag = (static_cast <DroppableJSONBox *> (pje_text_box_p)) -> SetFromJSON (json_null ());
		}
	else
		{
			if (verbose_flag)
				{
					PrintJSONToLog (STM_LEVEL_INFO, __FILE__, __LINE__, param_value_p, "ParamJSONEditor :: SetValueFromJSON on widget \"%s\"\n", GetParameterName ());
				}

			success_flag = (static_cast <DroppableJSONBox *> (pje_text_box_p)) -> SetFromJSON (param_value_p);
		}

	return success_flag;
}

