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
#ifndef PARAM_TABLE_WIDGET_H
#define PARAM_TABLE_WIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QPoint>
#include <QScrollArea>

#include "parameter.h"

#include "base_param_widget.h"


class ParamTableWidget;

class DroppableTableWidget : public QTableWidget
{
	Q_OBJECT

public slots:
	void ShowPopupMenu (const QPoint &p);

	void CheckCell (int row, int column);

public:
	DroppableTableWidget (QWidget *parent_p, ParamTableWidget *param_table_widget_p, char row_delimiter, char column_delimter, const bool first_row_as_headers_flag);

	void SetRow (const int row, const char *data_s);

	void SetColumnDelimiter (char delimiter);
	char GetColumnDelimiter () const;

	void SetRowDelimiter (char delimiter);
	char GetRowDelimiter () const;


	char *GetValueAsText ();
	json_t *GetValueAsJSON ();
	bool SetValueFromText (const char *data_s);

protected:
	virtual void dragEnterEvent (QDragEnterEvent *event_p);

	virtual void dropEvent (QDropEvent *event_p);
	virtual void dragMoveEvent (QDragMoveEvent *event_p);

	void LoadText (const char *filename_s);
	char *GetEntry (const char *start_s, const char *end_s);

	bool IsTableEmpty () const;

	virtual bool dropMimeData (int row, int column, const QMimeData * data, Qt::DropAction action);

private:
	ParamTableWidget *dtw_param_table_widget_p;
	char dtw_row_delimiter;
	char dtw_column_delimiter;
	bool dtw_unpack_text_content_flag;
	bool dtw_first_row_as_headers_flag;
};


class ParamTableWidget : public BaseParamWidget
{
	Q_OBJECT

public:
	static const char * const PTW_COLUMN_HEADERS_S;

	ParamTableWidget (Parameter * const param_p, QTParameterWidget * const parent_p);
	virtual ~ParamTableWidget ();

	virtual void RemoveConnection ();

	virtual void SetDefaultValue ();

	virtual bool SetValueFromText (const char *value_s);

	virtual bool SetValueFromJSON (const json_t * const value_p);


	virtual bool StoreParameterValue ();

	virtual void ShowErrors (const json_t *errors_p);

	bool SetColumnHeaders (Parameter *param_p);

public slots:
	void ClearTable (bool triggered_flag = false);

protected:
	QScrollArea *ptw_scroller_p;
	DroppableTableWidget *ptw_table_p;
	char ptw_column_delimiter;
	char ptw_row_delimiter;

	virtual QWidget *GetQWidget ();
};


#endif // PARAM_TABLE_WIDGET_H

