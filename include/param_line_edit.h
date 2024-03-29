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
#ifndef PARAMETER_LINE_EDIT_H
#define PARAMETER_LINE_EDIT_H


#include <QWidget>
#include <QLineEdit>

#include "string_parameter.h"
#include "string_array_parameter.h"

#include "base_param_widget.h"



class ParamLineEdit : public BaseParamWidget
{
  Q_OBJECT


private slots:
	bool UpdateConfigValue (const char * const value_s);

public:
	ParamLineEdit (StringParameter * const param_p, QTParameterWidget * const parent_p, QLineEdit :: EchoMode echo = QLineEdit :: Normal);
	ParamLineEdit (StringArrayParameter * const param_p, QTParameterWidget * const parent_p, QLineEdit :: EchoMode echo = QLineEdit :: Normal);
	virtual ~ParamLineEdit ();

  virtual void RemoveConnection ();

  virtual void SetDefaultValue ();

  virtual bool SetValueFromText (const char *value_s);


	virtual bool SetValueFromJSON (const json_t * const value_p);

  void SetMaxLength (int l);

	virtual bool StoreParameterValue (bool refresh_flag);

	virtual bool SetParameter (Parameter *param_p);

	virtual bool SetFromParameterValue (Parameter *param_p);

protected:
  QLineEdit *ple_text_box_p;
	StringParameter *ple_param_p;

  virtual QWidget *GetQWidget ();
};



#endif		/* #ifndef PARAMETER_LINE_EDIT_H */
