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
#ifndef UINT_PARAMETER_SPIN_BOX_H
#define UINT_PARAMETER_SPIN_BOX_H


#include <QWidget>
#include <QSpinBox>

#include "unsigned_int_parameter.h"
#include "base_param_widget.h"
#include "unsigned_int_spinner.h"


class UnsignedIntParamSpinBox : public BaseParamWidget
{
	Q_OBJECT


public:
	UnsignedIntParamSpinBox (UnsignedIntParameter * const param_p, QTParameterWidget * const parent_p);
	virtual ~UnsignedIntParamSpinBox ();


	virtual bool SetValueFromText (const char *value_s);
	virtual bool SetValueFromJSON (const json_t * const value_p);

	virtual void SetDefaultValue ();
	virtual bool StoreParameterValue ();

protected:
	static uint32 UIPSB_DEFAULT_MAX;

	virtual QWidget *GetQWidget ();

private:	
	UnsignedIntSpinner *uipsb_spin_box_p;
	UnsignedIntParameter *uips_param_p;
};


#endif		/* #ifndef UINT_PARAMETER_SPIN_BOX_H */
