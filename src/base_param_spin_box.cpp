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
#include <QDebug>

#include "base_param_spin_box.h"
#include "prefs_widget.h"
#include "math_utils.h"

#include "signed_int_parameter.h"
#include "unsigned_int_parameter.h"



ClearableSpinBox :: ClearableSpinBox (QWidget *parent_p)
	: QSpinBox (parent_p)
{}


void ClearableSpinBox :: fixup (QString &input) const
{

}


BaseParamSpinBox :: BaseParamSpinBox (Parameter * const param_p, QTParameterWidget * const parent_p)
:	BaseParamWidget (param_p, parent_p)
{
	bpsb_spin_box_p = new QSpinBox (parent_p);

	if (param_p -> pa_refresh_service_flag)
		{
			QObject ::  connect (bpsb_spin_box_p, static_cast <void (QSpinBox :: *) (int)>(&QSpinBox::valueChanged), parent_p, &QTParameterWidget :: RefreshService);
		}
}


BaseParamSpinBox ::	~BaseParamSpinBox ()
{
	delete bpsb_spin_box_p;
}


QWidget *BaseParamSpinBox :: GetQWidget ()
{
	return bpsb_spin_box_p;
}

