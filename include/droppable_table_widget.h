#ifndef DROPPABLE_TABLE_WIDGET_H
#define DROPPABLE_TABLE_WIDGET_H

#include <QTableWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QPoint>
#include <QScrollArea>

#include "jansson.h"

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

#endif // DROPPABLE_TABLE_WIDGET_H
