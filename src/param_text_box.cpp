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
#include <stdio.h>

#include <QDebug>
#include <QFontMetrics>
#include <QMimeData>

#include "param_text_box.h"
#include "prefs_widget.h"

#include "string_utils.h"
#include "byte_buffer.h"


ParamTextBox :: ParamTextBox (StringParameter * const param_p, QTParameterWidget * const parent_p)
:		BaseParamWidget (& (param_p -> sp_base_param), parent_p)
{
	ptb_param_p = param_p;
	CreateDroppableTextBox (parent_p);
}



bool ParamTextBox :: CreateDroppableTextBox (QTParameterWidget *parent_p)
{
	ptb_text_box_p = new DroppableTextBox (parent_p);

	if (bpw_param_p -> pa_refresh_service_flag)
		{
			QObject ::  connect (ptb_text_box_p,  &DroppableTextBox :: textChanged, parent_p, &QTParameterWidget :: RefreshService);
		}

	return true;
}



ParamTextBox ::	~ParamTextBox ()
{
	delete ptb_text_box_p;
	ptb_text_box_p = NULL;
}





void ParamTextBox :: RemoveConnection ()
{
}


void ParamTextBox :: SetDefaultValue ()
{
	const char *value_s = GetStringParameterDefaultValue (ptb_param_p);

	ptb_text_box_p -> setPlainText (value_s);
}



QWidget *ParamTextBox :: GetQWidget ()
{
	return ptb_text_box_p;
}



bool ParamTextBox :: StoreParameterValue ()
{
	QString s = ptb_text_box_p -> toPlainText ();
	QByteArray ba = s.toLocal8Bit ();
	const char *value_s = ba.constData ();

	bool success_flag = SetStringParameterCurrentValue (ptb_param_p, value_s);

	qDebug () << "Setting " << bpw_param_p -> pa_name_s << " to " << value_s << " " << success_flag;

	return success_flag;
}



bool ParamTextBox :: SetValueFromText (const char *value_s)
{
	QString s = ptb_text_box_p -> toPlainText ();
	qDebug () << "old " << s;

	ptb_text_box_p -> setPlainText (value_s);

	s = ptb_text_box_p -> toPlainText ();
	qDebug () << "new " << s;

	return true;
}



bool ParamTextBox :: SetValueFromJSON (const json_t * const value_p)
{
	bool success_flag = false;

	if (json_is_string (value_p))
		{
			const char *value_s = json_string_value (value_p);

			ptb_text_box_p -> setPlainText (value_s);
			success_flag = true;
		}

	return success_flag;
}



