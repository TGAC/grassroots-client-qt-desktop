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
#include "param_table_widget.h"

#include <stdio.h>

#include <QDebug>
#include <QFont>
#include <QMimeData>
#include <QTableWidgetItem>
#include <QMenu>
#include <QAction>
#include "prefs_widget.h"

#include "string_utils.h"
#include "byte_buffer.h"


const char * const ParamTableWidget :: PTW_COLUMN_HEADERS_S = "COLUMN_HEADERS";




ParamTableWidget :: ParamTableWidget (StringParameter * const param_p, QTParameterWidget * const parent_p)
:		BaseTableWidget (& (param_p -> sp_base_param), parent_p)
{
}


ParamTableWidget ::	~ParamTableWidget ()
{
	delete ptw_scroller_p;
	ptw_scroller_p = nullptr;
}




