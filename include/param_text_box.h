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
#ifndef PARAM_TEXT_BOX_H
#define PARAM_TEXT_BOX_H

#include <QWidget>


#include "string_parameter.h"

#include "base_param_widget.h"
#include "droppable_text_box.h"


class ParamTextBox : public BaseParamWidget
{
	Q_OBJECT


public:
	ParamTextBox (StringParameter * const param_p, QTParameterWidget * const parent_p);
	virtual ~ParamTextBox ();

	virtual void RemoveConnection ();

	virtual void SetDefaultValue ();

	virtual bool SetValueFromText (const char *value_s);

	virtual bool SetValueFromJSON (const json_t * const value_p);

	virtual bool StoreParameterValue (bool refresh_flag);

	virtual bool SetParameter (Parameter *param_p);


protected:
	DroppableTextBox *ptb_text_box_p;
	StringParameter *ptb_param_p;

	virtual QWidget *GetQWidget ();

	virtual bool CreateDroppableTextBox (QTParameterWidget *parent_p);

};




#endif // PARAM_TEXT_BOX_H
