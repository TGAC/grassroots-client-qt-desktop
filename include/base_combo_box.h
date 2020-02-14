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
#ifndef PARAMETER_COMBO_BOX_H
#define PARAMETER_COMBO_BOX_H


#include <QWidget>
#include <QComboBox>
#include <QGroupBox>

#include "parameter.h"
#include "base_param_widget.h"



class BaseComboBox : public BaseParamWidget
{
	Q_OBJECT

public:
  BaseComboBox (Parameter * const param_p, QTParameterWidget * const parent_p);
  virtual ~BaseComboBox ();

protected:
	QComboBox *bcb_combo_box_p;

	virtual QWidget *GetQWidget ();

private:
  QGroupBox *bcb_group_p;
};


#endif		/* #ifndef PARAMETER_COMBO_BOX_H */
