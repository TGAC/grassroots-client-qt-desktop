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

#include "param_date_widget.h"

#include "progress_window.h"
#include "prefs_widget.h"

#include "string_utils.h"
#include "time_util.h"


ParamDateWidget :: ParamDateWidget (Parameter * const param_p, QTParameterWidget * const parent_p)
: BaseParamWidget (param_p, parent_p)
{
	pdw_calendar_p = new QCalendarWidget (parent_p);
}


ParamDateWidget ::	~ParamDateWidget ()
{
	delete pdw_calendar_p;
}



bool ParamDateWidget :: StoreParameterValue ()
{
	bool success_flag = false;
	QDate d = pdw_calendar_p -> selectedDate ();
	struct tm *time_p = AllocateTime ();

	if (time_p)
		{
			memset (time_p, 0, sizeof (struct tm));

			time_p -> tm_year = d.year () - 1900;
			time_p -> tm_mon = d.month () - 1;
			time_p -> tm_mday = d.day ();

			success_flag = SetParameterValue (bpw_param_p, time_p, true);

			FreeTime (time_p);
		}


	return success_flag;
}


void ParamDateWidget :: SetDefaultValue ()
{
	struct tm *time_p = bpw_param_p -> pa_default.st_time_p;

	if (time_p)
		{
			QDate d (1900 + (time_p -> tm_year), 1 + (time_p -> tm_mon), time_p -> tm_mday);

			pdw_calendar_p -> setSelectedDate (d);
		}
}


bool ParamDateWidget :: SetValueFromText (const char *value_s)
{
	bool success_flag = false;

	if (value_s)
		{
			struct tm time_val;

			memset (&time_val, 0, sizeof (struct tm));

			if (SetTimeFromString (&time_val, value_s))
				{
					QDate d (1900 + (time_val.tm_year), 1 + (time_val.tm_mon), time_val.tm_mday);

					pdw_calendar_p -> setSelectedDate (d);
					success_flag = true;
				}
		}

	return success_flag;
}


bool ParamDateWidget :: SetValueFromJSON (const json_t * const value_p)
{
	bool success_flag = false;

	if (json_is_string (value_p))
		{
			success_flag = SetValueFromText (json_string_value (value_p));
		}

	return success_flag;
}


QWidget *ParamDateWidget :: GetQWidget ()
{
	return pdw_calendar_p;
}
