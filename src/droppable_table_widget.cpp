#include <QAction>
#include <QMenu>
#include <QMimeData>
#include <QDebug>
#include <QInputDialog>

#include "droppable_table_widget.h"
#include "string_utils.h"
#include "streams.h"
#include "json_util.h"
#include "parameter.h"
#include "base_table_widget.h"



DroppableTableWidget :: DroppableTableWidget (QWidget *parent_p, BaseTableWidget *param_table_widget_p, char row_delimiter, char column_delimter, const bool first_row_as_headers_flag)
: QTableWidget (parent_p), dtw_param_table_widget_p (param_table_widget_p), dtw_first_row_as_headers_flag (first_row_as_headers_flag)
{
	setAcceptDrops (true);
	SetRowDelimiter (row_delimiter);
	SetColumnDelimiter (column_delimter);
	dtw_unpack_text_content_flag = true;

	setSizePolicy (QSizePolicy :: MinimumExpanding, QSizePolicy :: MinimumExpanding);

	setContextMenuPolicy (Qt :: CustomContextMenu);

	connect (this, &DroppableTableWidget :: customContextMenuRequested, this, &DroppableTableWidget :: ShowPopupMenu);

	connect (this, &QTableWidget :: cellChanged, this, &DroppableTableWidget :: CheckCell);

}


void DroppableTableWidget :: CheckCell (int row, int column)
{
	QTableWidgetItem *column_header_p = horizontalHeaderItem (column);

	if (column_header_p)
		{
			QVariant v = column_header_p -> data (Qt::UserRole);

			if (v.isValid ())
				{
					QByteArray ba = v.toByteArray ();
					const char *value_s = ba.constData ();

					if (value_s)
						{
							QTableWidgetItem *cell_p = item (row, column);
							QString s (cell_p -> text ());

							if (strcmp (value_s, PA_TYPE_BOOLEAN_S) == 0)
								{

								}
							else if (strcmp (value_s, PA_TYPE_CHARACTER_S) == 0)
								{

								}
							else if (strcmp (value_s, PA_TYPE_INTEGER_S) == 0)
								{

								}
							else if (strcmp (value_s, PA_TYPE_NUMBER_S) == 0)
								{
									bool success_flag = false;

									double d = s.toDouble (&success_flag);

									if (!success_flag)
										{

										}
								}
							else if (strcmp (value_s, PA_TYPE_BOOLEAN_S) == 0)
								{

								}
						}
				}
		}
}



void DroppableTableWidget :: SetColumnDelimiter (char delimiter)
{
	dtw_column_delimiter = delimiter;
}


char DroppableTableWidget :: GetColumnDelimiter () const
{
	return dtw_column_delimiter;
}


void DroppableTableWidget :: SetRowDelimiter (char delimiter)
{
	dtw_row_delimiter = delimiter;
}


char DroppableTableWidget :: GetRowDelimiter () const
{
	return dtw_row_delimiter;
}




bool DroppableTableWidget :: SetColumnHeadersFromFirstRow ()
{
	bool success_flag = false;
	const int num_columns = columnCount ();

	for (int i = 0; i < num_columns; ++ i)
		{
			QTableWidgetItem *item_p = item (0, i);
			QString entry;

			if (item_p)
				{
					entry = item_p -> text ();
				}


			if ((entry.isNull ()) || (entry.isEmpty ()))
				{
					entry = "";
				}

			QTableWidgetItem *header_p = horizontalHeaderItem (i);

			if (header_p)
				{
					header_p -> setText (entry);
				}
			else
				{
					header_p = new QTableWidgetItem (entry);
					setHorizontalHeaderItem (i, header_p);
				}

			qDebug () << "Set col " << i << " header to \"" << entry << "\"" << Qt :: endl;
		}

	removeRow (0);

	return success_flag;
}




void DroppableTableWidget :: dropEvent (QDropEvent *event_p)
{
	QList <QUrl> urls = event_p -> mimeData () -> urls ();

	if (! (urls.isEmpty ()))
		{
			QString filename = urls.first ().toLocalFile ();

			if (! ((filename.isEmpty ()) || (filename.isNull ())))
				{
					QByteArray ba = filename.toLocal8Bit ();
					const char * const filename_s = ba.constData ();

					//qDebug () << "dropped " << filename;

					LoadText (filename_s);

					//qDebug () << "rows " << rowCount () << " cols " << columnCount ();

				}		/* if (! (filename.isEmpty ())) */

		}		/* if (! (urls.isEmpty ())) */
}

void DroppableTableWidget :: dragEnterEvent (QDragEnterEvent *event_p)
{
	event_p -> acceptProposedAction ();
	event_p -> accept ();
}

void DroppableTableWidget :: dragMoveEvent (QDragMoveEvent *event_p)
{
	event_p -> accept ();
}


bool DroppableTableWidget :: dropMimeData (int row, int column, const QMimeData *data_p, Qt :: DropAction action)
{
	return true;
}


void DroppableTableWidget :: ShowPopupMenu (const QPoint &p)
{
	QMenu *menu_p = new QMenu (this);

	QAction *action_p = new QAction (tr ("Clear Table"), this);
	connect (action_p, &QAction :: triggered, dtw_param_table_widget_p, &BaseTableWidget :: ClearTable);
	menu_p -> addAction (action_p);

	if (dtw_param_table_widget_p -> AreColumnsAddable ())
		{
			action_p = new QAction (tr ("Add Column"), this);
			connect (action_p, &QAction :: triggered, this, &DroppableTableWidget :: AddColumn);
			menu_p -> addAction (action_p);
		}

	action_p = new QAction (tr ("Set Columns From First Row"), this);
	connect (action_p, &QAction :: triggered, this, &DroppableTableWidget :: SetColumnHeadersFromFirstRow);
	menu_p -> addAction (action_p);

	menu_p->exec (mapToGlobal (p));
}


void DroppableTableWidget :: AddColumn (bool triggered_flag)
{

	/*
	 * Get column name to add
	 */
  bool ok_flag;
	QString column = QInputDialog::getText (this, tr ("Add Column"), tr ("Name:"), QLineEdit :: Normal, "", &ok_flag);

	if (ok_flag && (!column.isEmpty ()))
		{
			QByteArray ba = column.toLocal8Bit ();
			const char * const column_s = ba.constData ();

			if (!dtw_param_table_widget_p -> AddColumnHeader (column_s))
				{

				}

		}
}



char *DroppableTableWidget :: GetEntry (const char *start_s, const char *end_s)
{
	char *value_s = nullptr;

	if (!end_s)
		{
			size_t l = strlen (start_s);
			end_s = start_s + l;
		}

	if (dtw_unpack_text_content_flag)
		{
			const char *first_value_p = start_s;
			bool found_flag = false;

			while ((!found_flag) && (first_value_p < end_s))
				{
					if (*first_value_p == '\"')
						{
							found_flag = true;
						}

					++ first_value_p;
				}

			if (found_flag)
				{
					if (end_s)
						{
							const char *last_value_p = end_s;

							found_flag = false;

							while ((!found_flag) && (last_value_p > first_value_p))
								{
									if (*last_value_p == '\"')
										{
											found_flag = true;
										}
									else
										{
											-- last_value_p;
										}
								}

							if (found_flag)
								{
									value_s = CopyToNewString (first_value_p, last_value_p - first_value_p, false);
								}
						}
					else
						{
							value_s = EasyCopyToNewString (first_value_p);
						}
				}
			else
				{
					if (end_s != start_s)
						{
							value_s = CopyToNewString (start_s, end_s - start_s, false);
						}
				}
		}
	else
		{
			value_s = CopyToNewString (start_s, end_s - start_s, false);
		}

	return value_s;
}




void DroppableTableWidget :: SetRow (const int row, const char *data_s)
{
	const char *current_token_s = data_s;
	const char *next_token_s;
	int col = 0;
	bool loop_flag = true;

	if (row >= rowCount ())
		{
			setRowCount (row + 1);
		}

	while (loop_flag)
		{
			char *value_s = nullptr;

			if (*current_token_s != dtw_column_delimiter)
				{
					next_token_s = strchr (current_token_s, dtw_column_delimiter);

					if (next_token_s)
						{
							value_s = GetEntry (current_token_s, next_token_s);

							if (!value_s)
								{
									value_s = CopyToNewString (current_token_s, next_token_s - current_token_s, false);
								}

							current_token_s = next_token_s + 1;
						}
					else
						{
							value_s = GetEntry (current_token_s, nullptr);
							loop_flag = false;
						}
				}
			else
				{
					++ current_token_s;
				}

			QTableWidgetItem *item_p = item (row, col);

			if (item_p)
				{
					item_p -> setText (value_s);
				}
			else
				{
					item_p = new QTableWidgetItem (value_s);

					if (col >= columnCount ())
						{
							setColumnCount (col + 1);
						}


					setItem (row, col, item_p);
				}

			//qDebug () << "num rows " << rowCount () << " num cols " << columnCount ();

			++ col;

			FreeCopiedString (value_s);
		}		/* while (loop_flag) */
}


bool DroppableTableWidget :: IsTableEmpty () const
{
	const int num_rows = rowCount ();
	const int num_cols = columnCount ();

	for (int i = 0; i < num_rows; ++ i)
		{
			for (int j = 0; j < num_cols; ++ j)
				{
					QTableWidgetItem *item_p = item (i, j);

					if (item_p)
						{
							QString s = item_p -> text ().trimmed ();

							if (s.length () > 0)
								{
									return false;
								}
						}
				}
		}

	return true;
}



json_t *DroppableTableWidget :: GetValueAsJSON (bool *success_flag_p)
{
	if (!IsTableEmpty ())
		{
			json_t *rows_p = json_array ();

			if (rows_p)
				{
					const int num_rows = rowCount ();
					const int num_cols = columnCount ();
					bool success_flag = true;
					int i = 0;

					if (dtw_first_row_as_headers_flag)
						{
							++ i;
						}

					for ( ; i < num_rows; ++ i)
						{
							json_t *row_p = json_object ();

							if (row_p)
								{
									if (json_array_append_new (rows_p, row_p) == 0)
										{
											for (int j = 0; j < num_cols; ++ j)
												{
													QTableWidgetItem *item_p = item (i, j);

													if (item_p)
														{
															QTableWidgetItem *column_header_p = nullptr;
															QString column_heading;
															column_heading.clear ();

															if (dtw_first_row_as_headers_flag)
																{
																	column_header_p = item (0, j);
																}
															else
																{
																	column_header_p = horizontalHeaderItem (j);
																}


															if (column_header_p)
																{
																	QVariant column_data = column_header_p -> data (Qt :: DisplayRole);

																	if ((column_data.isValid ()) && (!column_data.isNull ()))
																		{
																			column_heading = column_data.toString ().trimmed ();
																		}
																}


															if (column_heading.size () == 0)
																{
																	column_heading = QString :: number (j);
																}

															//qDebug () << "getting column " <<  column_heading << " for column " << j << endl;


															QByteArray column_heading_ba = column_heading.toLocal8Bit ();
															const char *column_heading_s = column_heading_ba.constData ();

															QString value_s = item_p -> text ().trimmed ();
															QByteArray ba = value_s.toLocal8Bit ();
															const char *item_value_s = ba.constData ();

															if (!SetJSONString (row_p, column_heading_s, item_value_s))
																{
																	success_flag = false;
																}
														}

													if (!success_flag)
														{
															i = num_rows;
															j = num_cols;
														}
												}

										}		/* if (json_array_append_new (rows_p, row_p) == 0) */
									else
										{
											json_decref (row_p);
											success_flag = false;
										}

								}		/* if (row_p) */

						}		/* for (int i = 0; i < num_rows; ++ i) */

					if (success_flag_p)
						{
							*success_flag_p = success_flag;
						}

					if (success_flag)
						{
							size_t num_rows = json_array_size (rows_p);

							/* remove any trailing empty rows */
							if (num_rows > 0)
								{
									size_t i = num_rows;
									bool loop_flag = true;

									while (loop_flag)
										{
											json_t *row_p = json_array_get (rows_p, i);

											if (json_object_size (row_p) > 0)
												{
													loop_flag = false;
												}
											else
												{
													json_array_remove (rows_p, i);
													json_decref (row_p);

													if (i == 0)
														{
															loop_flag = false;
														}
													else
														{
															-- i;
														}
												}

										}


								}		/* if (num_rows > 0) */

							return rows_p;
						}

					json_decref (rows_p);
				}		/* if (rows_p) */


		}		/* if (!IsTableEmpty ()) */
	else
		{
			/* nothing to do */
			if (success_flag_p)
				{
					*success_flag_p = true;
				}
		}

	return NULL;
}


char *DroppableTableWidget :: GetValueAsText (bool *success_flag_p)
{
	char *value_s = NULL;
	json_t *table_json_p = GetValueAsJSON (success_flag_p);

	if (table_json_p)
		{
			char *data_s = json_dumps (table_json_p, JSON_INDENT (2));

			if (data_s)
				{
					value_s = EasyCopyToNewString (data_s);

					free (data_s);
				}
			else
				{
					PrintJSONToErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, table_json_p, "Failed to get JSON as string");
				}

			json_decref (table_json_p);
		}

	return value_s;
}





bool DroppableTableWidget :: SetValueFromText (const char *data_s)
{
	bool success_flag = false;

	if (data_s)
		{
			const char *start_row_p = data_s;
			const char *end_row_p = strchr (start_row_p, dtw_row_delimiter);
			int x = 0;

			/*
			 * loop over each row
			 */
			while (start_row_p && end_row_p)
				{
					/*
					 * loop over each column
					 */
					int y = 0;
					const char *start_column_p = start_row_p;
					const char *end_column_p = strchr (start_column_p, dtw_column_delimiter);

					while (end_column_p)
						{


							start_column_p = end_column_p + 1;
							if (*start_column_p != '\0')
								{
									end_column_p = strchr (start_column_p, dtw_column_delimiter);
								}

							++ y;
						}		/* while (end_column_p) */



					start_row_p = end_row_p + 1;
					if (*start_row_p != '\0')
						{
							end_row_p = strchr (start_row_p, dtw_row_delimiter);
						}

					++ x;
				} /* while (start_row_p && end_row_p) */

		}		/* if (data_s) */

	return success_flag;
}


int DroppableTableWidget :: LoadJSON (const char *filename_s)
{
	int res = 0;
	json_error_t err;
	json_t *data_p = json_load_file (filename_s, 0, &err);

	if (data_p)
		{
			if (json_is_array (data_p))
				{
					size_t num_rows = json_array_size (data_p);


				}

			json_decref (data_p);
		}

	return res;
}


void DroppableTableWidget :: LoadText (const char *filename_s)
{
	FILE *in_f = fopen (filename_s, "r");

	qDebug () << "rows " << rowCount () << " cols " << columnCount ();

	if (in_f)
		{
			bool loop_flag = true;
			char *buffer_s = nullptr;
			int row = 0;

			setSortingEnabled (false);

			/*
			if (dtw_first_row_as_headers_flag)
				{
					if (GetLineFromFile (in_f, &buffer_s))
						{
							SetColumnHeaders (buffer_s);
						}
				}
			 */


			while (loop_flag)
				{
					if (GetLineFromFile (in_f, &buffer_s))
						{
							if (*buffer_s)
								{
									SetRow (row, buffer_s);

									++ row;
								}
							else
								{
									loop_flag = false;
								}
						}
					else
						{
							loop_flag = false;
						}
				}

			setSortingEnabled (true);

			fclose (in_f);
		}

	//qDebug () << "rows " << rowCount () << " cols " << columnCount ();
}
