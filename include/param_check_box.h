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
#ifndef PARAMETER_CHECK_BOX_H
#define PARAMETER_CHECK_BOX_H


#include <QWidget>
#include <QCheckBox>

#include "base_param_widget.h"

#include "boolean_parameter.h"

class ParamCheckBox : public BaseParamWidget
{
	Q_OBJECT


private slots:
//	bool UpdateConfig (int state);

public:
	ParamCheckBox (BooleanParameter * const param_p, QTParameterWidget * const parent_p0);
	virtual ~ParamCheckBox ();

	virtual void SetDefaultValue ();

	virtual bool SetValueFromText (const char *value_s);

	virtual bool SetValueFromJSON (const json_t * const value_p);

	virtual bool StoreParameterValue (bool refresh_flag);

protected:
	virtual QWidget *GetQWidget ();


private:
	QCheckBox *pcb_check_box_p;
	BooleanParameter *pcb_param_p;
};


#endif		/* #ifndef PARAMETER_CHECK_BOX_H */
