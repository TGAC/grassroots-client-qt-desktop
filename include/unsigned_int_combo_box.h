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
#ifndef UNSIGNED_INT_PARAM_COMBO_BOX_H
#define UNSIGNED_INT_PARAM_COMBO_BOX_H

#include <QWidget>

#include "unsigned_int_parameter.h"
#include "base_combo_box.h"



class UnsignedIntComboBox : public BaseComboBox
{
	Q_OBJECT


public:
	UnsignedIntComboBox (UnsignedIntParameter * const param_p, QTParameterWidget * const parent_p);
	virtual ~UnsignedIntComboBox ();

	bool AddOption (const uint32 value, const char *description_s);

	virtual void SetDefaultValue ();

	virtual bool SetValueFromText (const char *value_s);

	virtual bool SetValueFromJSON (const json_t * const value_p);

	virtual bool StoreParameterValue (bool refresh_flag);

	virtual bool SetFromParameterValue (Parameter *param_p);

private:
	UnsignedIntParameter *uicb_param_p;

	bool SetValue (const uint32 value);

};


#endif // UNSIGNED_INT_PARAM_COMBO_BOX_H
