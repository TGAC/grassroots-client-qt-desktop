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

/**
 * @file
 * @brief
 */
#ifndef PARAM_TABLE_WIDGET_H
#define PARAM_TABLE_WIDGET_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QPoint>
#include <QScrollArea>

#include "string_parameter.h"

#include "base_table_widget.h"


class StringTableWidget : public BaseTableWidget
{
	Q_OBJECT

public:
	static const char * const PTW_COLUMN_HEADERS_S;

	StringTableWidget (StringParameter * const param_p, QTParameterWidget * const parent_p);
	virtual ~StringTableWidget ();

	virtual void SetDefaultValue ();

	virtual bool SetValueFromText (const char *value_s);

	virtual bool SetValueFromJSON (const json_t * const value_p);

	virtual bool StoreParameterValue (bool refresh_flag);

	virtual void Clear ();

protected:
	StringParameter *stw_param_p;
};


#endif // PARAM_TABLE_WIDGET_H

