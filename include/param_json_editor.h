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
*/
#ifndef PARAM_JSON_EDITOR_H
#define PARAM_JSON_EDITOR_H

#include "base_param_widget.h"
#include "json_parameter.h"

#include "droppable_text_box.h"

class DroppableJSONBox : public DroppableTextBox
{
public:
	DroppableJSONBox (QTParameterWidget *parent_p);
	virtual bool SetFromJSON (const json_t * const value_p);
	virtual bool SetFromText (const char * const data_s);

};


class ParamJSONEditor : public BaseParamWidget
{
	Q_OBJECT

public:
	ParamJSONEditor (JSONParameter * const param_p, QTParameterWidget * const parent_p);
	virtual ~ParamJSONEditor ();

	virtual void SetDefaultValue ();

	virtual bool SetValueFromText (const char *value_s);


	virtual bool SetValueFromJSON (const json_t * const value_p);


	virtual bool StoreParameterValue ();

protected:
	JSONParameter *pje_param_p;
	DroppableJSONBox *pje_text_box_p;

	virtual QWidget *GetQWidget ();

	virtual bool CreateDroppableTextBox (QTParameterWidget *parent_p);

};

#endif // PARAM_JSON_EDITOR_H

