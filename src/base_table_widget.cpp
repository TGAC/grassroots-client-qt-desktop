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
#include "base_table_widget.h"

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

#include "json_parameter.h"

const char * const BaseTableWidget :: PTW_COLUMN_HEADERS_S = "COLUMN_HEADERS";




BaseTableWidget :: BaseTableWidget (Parameter * const param_p, QTParameterWidget * const parent_p)
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


BaseTableWidget ::	~BaseTableWidget ()
{
	delete ptw_scroller_p;
	ptw_scroller_p = nullptr;
}


void BaseTableWidget :: RemoveConnection ()
{
}


QWidget *BaseTableWidget :: GetQWidget ()
{
	return ptw_scroller_p;
}


bool BaseTableWidget :: SetColumnHeaders (Parameter *param_p)
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

													qDebug () << "setting header " <<  column_header_p -> text () << " for column " << i << " with data " << type_s << " = " << column_header_p -> data (Qt :: UserRole) <<  endl;

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


void BaseTableWidget :: ClearTable (bool triggered_flag)
{
	Clear ();
}


void BaseTableWidget :: Clear ()
{
	ptw_table_p -> clear ();
	SetColumnHeaders (bpw_param_p);
}


void BaseTableWidget :: ShowErrors (const json_t *errors_p)
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

						if (GetJSONInteger (value_p, TABLE_PARAM_ROW_S, &row))
							{
								const char *column_s = GetJSONString (value_p, TABLE_PARAM_COLUMN_S);

								if (column_s)
									{
										for (int j = num_columns - 1; j >= 0; -- j)
											{
												QTableWidgetItem *item_p = ptw_table_p -> horizontalHeaderItem (j);
												QString title = item_p -> text ();
												QByteArray title_ba = title.toLocal8Bit ();
												const char *title_s = title_ba.constData ();

												if (strcmp (title_s, column_s) == 0)
													{
														item_p = ptw_table_p -> item (row, j);

														if (!item_p)
															{
																item_p = new QTableWidgetItem ();
																ptw_table_p -> setItem (row, j, item_p);
															}

														if (item_p)
															{
																QBrush b (Qt :: red);
																const char *error_s = GetJSONString (value_p, JOB_ERROR_S);

																item_p -> setBackground (b);

																if (error_s)
																	{
																		item_p -> setToolTip (error_s);
																	}
															}

														/* force exit from loop */
														j = -1;
													}
											}

									}
							}

					}		/* json_array_foreach (errors_p, i, value_p) */

				}		/* if (json_is_array (errors_p)) */

		}		/* if (errors_p) */
}
