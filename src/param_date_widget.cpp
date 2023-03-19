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
#include <QHBoxLayout>
#include <QDateTime>

#include "param_date_widget.h"

#include "progress_window.h"
#include "prefs_widget.h"

#include "string_utils.h"
#include "time_util.h"


ParamDateWidget :: ParamDateWidget (TimeParameter * const param_p, QTParameterWidget * const parent_p)
: BaseParamWidget (& (param_p -> tp_base_param), parent_p)
{
	pdw_checkbox_p = new QCheckBox (parent_p);
	pdw_calendar_p = new QDateTimeEdit (QDateTime :: currentDateTime (), parent_p);
	pdw_param_p = param_p;

	QHBoxLayout *layout_p = new QHBoxLayout;

	layout_p -> addWidget (pdw_checkbox_p);
	layout_p -> addWidget (pdw_calendar_p);

	pdw_root_widget_p = new QWidget (parent_p);
	pdw_root_widget_p -> setLayout (layout_p);

	pdw_checkbox_p -> setEnabled (true);
	pdw_checkbox_p -> setChecked (true);

	connect (pdw_checkbox_p, &QAbstractButton :: clicked, pdw_calendar_p, &QWidget :: setEnabled);

	if (bpw_param_p -> pa_refresh_service_flag)
		{
			QObject ::  connect (pdw_calendar_p,  &QDateTimeEdit :: dateTimeChanged, parent_p, &QTParameterWidget :: RefreshService);
		}

}


ParamDateWidget ::	~ParamDateWidget ()
{
	delete pdw_root_widget_p;
}



bool ParamDateWidget :: StoreParameterValue (bool refresh_flag)
{
	bool success_flag = false;

	if (pdw_checkbox_p -> isChecked ())
		{
			QDateTime dt = pdw_calendar_p -> dateTime ();
			struct tm *time_p = AllocateTime ();

			if (time_p)
				{
					memset (time_p, 0, sizeof (struct tm));

					if (pdw_checkbox_p -> isChecked ())
						{
						  QDate d = dt.date ();
						  QTime t = dt.time ();

						  time_p -> tm_year = d.year () - 1900;
						  time_p -> tm_mon = d.month () - 1;
						  time_p -> tm_mday = d.day ();

						  time_p -> tm_hour = t.hour ();
						  time_p -> tm_min = t.minute ();
						  time_p -> tm_sec = t.second ();
						}

					success_flag = SetTimeParameterCurrentValue (pdw_param_p, time_p);
					qDebug () << "Setting " << pdw_param_p -> tp_base_param.pa_name_s << " to "
							<< time_p -> tm_mday <<  " " << time_p -> tm_mon <<  " "<< time_p -> tm_year;

					FreeTime (time_p);
				}
		}
	else
		{
			success_flag = SetTimeParameterCurrentValue (pdw_param_p, nullptr);
			qDebug () << "Setting " << pdw_param_p -> tp_base_param.pa_name_s << " to null";

		}

	return success_flag;
}


void ParamDateWidget :: SetDefaultValue ()
{
	const struct tm *time_p = GetTimeParameterDefaultValue (pdw_param_p);
	bool enabled_flag = true;

	if (time_p)
		{			
			if (time_p -> tm_year != 0)
				{
					QDate d (1900 + (time_p -> tm_year), 1 + (time_p -> tm_mon), time_p -> tm_mday);

					pdw_calendar_p -> setDate (d);
				}
		}
	else
		{
			enabled_flag = false;
		}

	pdw_checkbox_p -> setChecked (enabled_flag);
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

					pdw_calendar_p -> setDate (d);


					pdw_checkbox_p -> setChecked (true);

					success_flag = true;
				}
		}
	else
		{
			pdw_checkbox_p -> setChecked (false);
			success_flag = true;
		}

	return success_flag;
}


bool ParamDateWidget :: SetValueFromJSON (const json_t * const value_p)
{
	bool success_flag = false;

	if ((!value_p) || (json_is_null (value_p)))
		{
			pdw_checkbox_p -> setChecked (false);
			success_flag = true;
		}
	else if (json_is_string (value_p))
		{
			success_flag = SetValueFromText (json_string_value (value_p));
		}

	return success_flag;
}


QWidget *ParamDateWidget :: GetQWidget ()
{
	return pdw_root_widget_p;
}
