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



DroppableTableWidget :: DroppableTableWidget (QWidget *parent_p, ParamTableWidget *param_table_widget_p, char row_delimiter, char column_delimter, const bool first_row_as_headers_flag)
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

	connect (action_p, &QAction :: triggered, dtw_param_table_widget_p, &ParamTableWidget :: ClearTable);
	menu_p -> addAction (action_p);

	menu_p->exec (mapToGlobal (p));
}



char *DroppableTableWidget :: GetEntry (const char *start_s, const char *end_s)
{
	char *value_s = NULL;

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
							value_s = GetEntry (current_token_s, NULL);
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

//char *DroppableTableWidget :: OldGetValueAsText ()
//{
//	char *value_s = NULL;

//	if (!IsTableEmpty ())
//		{
//			ByteBuffer *buffer_p = AllocateByteBuffer (1024);

//			if (buffer_p)
//				{
//					const int num_rows = rowCount ();
//					const int num_cols = columnCount ();
//					bool success_flag = true;

//					for (int i = 0; i < num_rows; ++ i)
//						{
//							for (int j = 0; j < num_cols; ++ j)
//								{
//									QTableWidgetItem *item_p = item (i, j);

//									if (item_p)
//										{
//											QString s = item_p -> text ().trimmed ();
//											QByteArray ba = s.toLocal8Bit ();
//											const char *item_value_s = ba.constData ();

//											success_flag = AppendStringToByteBuffer (buffer_p, item_value_s);
//										}

//									if (success_flag)
//										{
//											success_flag = AppendToByteBuffer (buffer_p, &dtw_column_delimiter, 1);
//										}

//									if (!success_flag)
//										{
//											i = num_rows;
//											j = num_cols;
//										}
//								}

//							if (success_flag)
//								{
//									if (!AppendToByteBuffer (buffer_p, &dtw_row_delimiter, 1))
//										{
//											success_flag = false;
//											i = num_rows;
//										}
//								}
//						}

//					if (success_flag)
//						{
//							const char *data_s = GetByteBufferData (buffer_p);

//							value_s = EasyCopyToNewString (data_s);
//							qDebug () << value_s  << endl;
//						}

//					FreeByteBuffer (buffer_p);
//				}


//		}		/* if (!IsTableEmpty ()) */

//	return value_s;
//}



char *DroppableTableWidget :: GetValueAsText ()
{
	char *value_s = NULL;



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
															QTableWidgetItem *column_header_p = 0;
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

					if (success_flag)
						{
							char *data_s = json_dumps (rows_p, JSON_INDENT (2));

							if (data_s)
								{
									value_s = EasyCopyToNewString (data_s);
									free (data_s);
								}

							qDebug () << value_s  << endl;
						}


				}		/* if (rows_p) */


		}		/* if (!IsTableEmpty ()) */

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


void DroppableTableWidget :: LoadText (const char *filename_s)
{
	FILE *in_f = fopen (filename_s, "r");

	qDebug () << "rows " << rowCount () << " cols " << columnCount ();

	if (in_f)
		{
			bool loop_flag = true;
			char *buffer_s = NULL;
			int row = 0;

			setSortingEnabled (false);

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


ParamTableWidget :: ParamTableWidget (Parameter * const param_p, QTParameterWidget * const parent_p)
:		BaseParamWidget (param_p, parent_p)
{
	bool first_row_headers_flag = false;

	const char *value_s = GetParameterKeyValue (param_p, PA_TABLE_COLUMN_DELIMITER_S);

	if (value_s)
		{
			ptw_column_delimiter = *value_s;
		}
	else
		{
			ptw_column_delimiter = *PA_TABLE_DEFAULT_COLUMN_DELIMITER_S;
		}

	value_s = GetParameterKeyValue (param_p, PA_TABLE_ROW_DELIMITER_S);

	if (value_s)
		{
			ptw_row_delimiter = *value_s;
		}
	else
		{
			ptw_row_delimiter = *PA_TABLE_DEFAULT_ROW_DELIMITER_S;
		}

	value_s = GetParameterKeyValue (param_p, PA_TABLE_COLUMN_HEADERS_PLACEMENT_S);

	if (value_s)
		{
			if (strcmp (value_s, PA_TABLE_COLUMN_HEADERS_PLACEMENT_FIRST_ROW_S) == 0)
				{
					first_row_headers_flag = true;
				}
		}

	ptw_table_p = new DroppableTableWidget (parent_p, this, ptw_row_delimiter, ptw_column_delimiter, first_row_headers_flag);

	ptw_scroller_p = new QScrollArea (parent_p);
	ptw_scroller_p -> setWidgetResizable (true);
	ptw_scroller_p -> setWidget (ptw_table_p);



	if (!SetColumnHeaders (param_p))
		{
		}

	if (param_p -> pa_refresh_service_flag)
		{
			QObject ::  connect (ptw_table_p,  &QTableWidget :: itemChanged, parent_p, &QTParameterWidget :: RefreshService);
		}
}


ParamTableWidget ::	~ParamTableWidget ()
{
	delete ptw_scroller_p;
	ptw_scroller_p = NULL;
}


void ParamTableWidget :: RemoveConnection ()
{
}


void ParamTableWidget :: SetDefaultValue ()
{
	const char *value_s = bpw_param_p -> pa_default.st_string_value_s;

	if (value_s)
		{
			SetValueFromText (value_s);
		}
}


QWidget *ParamTableWidget :: GetQWidget ()
{
	return ptw_scroller_p;
}


bool ParamTableWidget :: StoreParameterValue ()
{
	bool success_flag  = false;
	char *value_s = ptw_table_p ->  GetValueAsText ();

	if (value_s)
		{
			SetParameterValue (bpw_param_p, value_s, true);

			success_flag = true;
			FreeCopiedString (value_s);
		}

	return success_flag;
}

void ParamTableWidget :: ClearTable (bool triggered_flag)
{
	ptw_table_p -> clear ();
	SetColumnHeaders (bpw_param_p);

	SetParameterValue (bpw_param_p, NULL, true);
}


bool ParamTableWidget :: SetColumnHeaders (Parameter *param_p)
{
	bool success_flag = false;
	const char *value_s = GetParameterKeyValue (param_p, PA_TABLE_COLUMN_HEADINGS_S);

	if (value_s)
		{
			json_error_t json_err;
			json_t *value_p = json_loads (value_s, 0, &json_err);

			if (value_p)
				{
					if (json_is_array (value_p))
						{
							QVector <QTableWidgetItem *> column_headers;
							const size_t num_columns = json_array_size (value_p);
							size_t i;

							ptw_table_p -> setColumnCount (num_columns);

							for (i = 0; i < num_columns; ++ i)
								{
									json_t *column_info_p = json_array_get (value_p, i);

									const char *name_s = GetJSONString (column_info_p, PARAM_NAME_S);

									if (name_s)
										{
											QTableWidgetItem *column_header_p = new QTableWidgetItem (name_s);

											qDebug () << "name_s: " << name_s << endl;

											if (column_header_p)
												{
													const char *type_s = GetJSONString (column_info_p, PARAM_TYPE_S);

													if (type_s)
														{
															QVariant v (type_s);
															column_header_p -> setData (Qt::UserRole, v);
														}

													qDebug () << "setting header " <<  column_header_p -> text () << " for column " << i << endl;

													ptw_table_p -> setHorizontalHeaderItem (i, column_header_p);

												}
											else
												{
													qDebug () << "failed to create column header for header_s: " << name_s << endl;
												}
										}

								}

						}

					json_decref (value_p);
				}


			success_flag = true;
			ptw_table_p -> setRowCount (5);

		}		/* if (value_s) */
	else
		{
			qDebug () << "no headers: " << endl;
		}

	return success_flag;
}


bool ParamTableWidget :: SetValueFromText (const char *value_s)
{
	bool success_flag  = true;

	if (value_s)
		{
			const char *current_row_s = value_s;
			const char *next_row_s  = strchr (current_row_s, '\n');
			int row = 0;

			while (next_row_s)
				{
					char *row_s = CopyToNewString (current_row_s, next_row_s - current_row_s, false);

					if (row_s)
						{
							ptw_table_p -> SetRow (row, row_s);
							FreeCopiedString (row_s);
						}

					current_row_s = next_row_s + 1;

					if (*current_row_s != '\0')
						{
							next_row_s = strchr (current_row_s, '\n');
							++ row;
						}
					else
						{
							current_row_s = NULL;
							next_row_s = NULL;
						}

				}		/* while (next_row_s) */

			if (current_row_s)
				{
					char *row_s = EasyCopyToNewString (current_row_s);

					if (row_s)
						{
							ptw_table_p -> SetRow (row, row_s);
							FreeCopiedString (row_s);
						}
				}
		}

	return success_flag;
}


bool ParamTableWidget :: SetValueFromJSON (const json_t * const value_p)
{
	bool success_flag = false;

	if (json_is_array (value_p))
		{
			size_t num_rows = json_array_size (value_p);
			const int num_cols = ptw_table_p -> columnCount ();

			for (size_t i = 0; i < num_rows; ++ i)
				{
					json_t *row_data_p = json_array_get (value_p, i);

					for (int j = 0; j < num_cols; ++ j)
						{
							QString s = ptw_table_p -> horizontalHeaderItem (j) -> text ();
							QByteArray ba = s.toLocal8Bit ();
							const char *key_s = ba.constData ();
							json_t *value_p = json_object_get (row_data_p, key_s);

							if (json_is_string (value_p))
								{
									const char *value_s = json_string_value (value_p);

									QTableWidgetItem *item_p = ptw_table_p -> item (i, j);

									if (!item_p)
										{
											item_p = new QTableWidgetItem (value_s);

											if (j >= num_cols)
												{
													ptw_table_p -> setColumnCount (num_cols + 1);
												}
										}
								}

						}
				}

			success_flag = true;
		}
	else if (json_is_string (value_p))
		{
			const char *value_s = json_string_value (value_p);

			success_flag = SetValueFromText (value_s);
		}

	return success_flag;
}


void ParamTableWidget :: ShowErrors (const json_t *errors_p)
{
	if (errors_p)
		{
			if (json_is_array (errors_p))
				{
					size_t i;
					json_t *value_p;
					const int num_columns = ptw_table_p -> columnCount ();

					json_array_foreach (errors_p, i, value_p)
					{
						int row;

						if (GetJSONInteger (value_p, "row", &row))
							{
								const char *error_s = GetJSONString (value_p, "error");
								QBrush b (Qt :: red);

								for (int i = num_columns - 1; i >= 0; -- i)
									{
										QTableWidgetItem *item_p = ptw_table_p -> item (row, i);

										if (item_p)
											{
												item_p -> setBackground (b);
												item_p -> setToolTip (error_s);
											}
									}
							}

					}		/* json_array_foreach (errors_p, i, value_p) */

				}		/* if (json_is_array (errors_p)) */

		}		/* if (errors_p) */
}
