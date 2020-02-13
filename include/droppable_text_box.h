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

#ifndef DROPPABLE_TEXT_BOX_H
#define DROPPABLE_TEXT_BOX_H

#include <QPlainTextEdit>
#include <QDragEnterEvent>
#include <QDropEvent>


#include "qt_parameter_widget.h"


class DroppableTextBox : public QPlainTextEdit
{
public:
	DroppableTextBox (QTParameterWidget *parent_p);
	~DroppableTextBox ();

	void LoadText (const char *filename_s);

	virtual bool SetFromText (const char * const data_s);


protected:
	virtual void dragEnterEvent (QDragEnterEvent *event_p);

	virtual void dropEvent (QDropEvent *event_p);

};


#endif // DROPPABLE_TEXT_BOX_H
