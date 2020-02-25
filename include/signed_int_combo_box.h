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
#ifndef SIGNED_INT_PARAM_COMBO_BOX_H
#define SIGNED_INT_PARAM_COMBO_BOX_H

#include <QWidget>

#include "signed_int_parameter.h"
#include "base_combo_box.h"



class SignedIntComboBox : public BaseComboBox
{
	Q_OBJECT


public:
	SignedIntComboBox (SignedIntParameter * const param_p, QTParameterWidget * const parent_p);
	virtual ~SignedIntComboBox ();

	virtual void SetDefaultValue ();

	virtual bool SetValueFromText (const char *value_s);

	virtual bool SetValueFromJSON (const json_t * const value_p);

	virtual bool StoreParameterValue (bool refresh_flag);


	bool AddOption (const int32 value, const char *description_s);

private:
	SignedIntParameter *sicb_param_p;

	bool SetValue (const int32 value);
};


#endif // SIGNED_INT_PARAM_COMBO_BOX_H
