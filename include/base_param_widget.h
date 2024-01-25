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
#ifndef BASE_PARAM_WIDGET_H
#define BASE_PARAM_WIDGET_H

#include <QWidget>


#include "labels_widget.h"

#include "parameter.h"
#include "jansson.h"


class QTParameterWidget;


class BaseParamWidget : public QObject
{
	Q_OBJECT

public:
	BaseParamWidget (Parameter * const param_p, QTParameterWidget * const parent_p);

	virtual ~BaseParamWidget ();

	QWidget *GetUIQWidget ();

	QWidget *GetLabel () const;

	void CheckLevelDisplay (const ParameterLevel ui_level, const QWidget * const parent_widget_p);

	bool MeetsLevel (const ParameterLevel ui_level) const;

	const char *GetParameterName () const;

	virtual void RemoveConnection ();

	void SetVisible (const bool visible_flag);

	virtual void SetErrors (const json_t *errors_p);

	virtual void SetErrorMessage (const char *message_s);

	virtual bool IsComboBoxWidget () const;

	virtual bool IsRequired ();



	void SetErrorFlag (const bool error_flag);
	bool GetErrorFlag () const;

	const Parameter *GetParameter () const;

	virtual bool SetParameter (Parameter *param_p);


	virtual void SetDefaultValue () = 0;

	virtual bool SetValueFromText (const char *value_s) = 0;

	virtual bool SetValueFromJSON (const json_t * const value_p) = 0;

	virtual bool StoreParameterValue (bool refresh_flag) = 0;

	virtual bool SetFromParameterValue (Parameter *param_p) = 0;

protected:
	Parameter *bpw_param_p;
	QTParameterWidget * const bpw_parent_p;
	LabelsWidget *bpw_label_p;
	char *bpw_param_name_s;

	static const char * const BPW_REQUIRED_S;

	virtual QWidget *GetQWidget () = 0;

private:
	bool bpw_error_flag;

};

#endif		/* #ifndef BASE_PARAM_WIDGET_H */
