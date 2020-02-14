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


bool BaseTableWidget :: SetValueFromText (const char *value_s)
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
							current_row_s = nullptr;
							next_row_s = nullptr;
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


bool BaseTableWidget :: SetValueFromJSON (const json_t * const value_p)
{
	bool success_flag = false;

	if (json_is_array (value_p))
		{
			size_t num_rows = json_array_size (value_p);
			const int num_cols = ptw_table_p -> columnCount ();

			/*
			 * Make sure that the table is of sufficient size
			 */
			if (num_rows >= (size_t) (ptw_table_p -> rowCount ()))
				{
					ptw_table_p -> setRowCount (num_rows + 1);
				}

			for (size_t i = 0; i < num_rows; ++ i)
				{
					json_t *row_data_p = json_array_get (value_p, i);

					for (int j = 0; j < num_cols; ++ j)
						{
							QString s = ptw_table_p -> horizontalHeaderItem (j) -> text ();
							QByteArray ba = s.toLocal8Bit ();
							const char *key_s = ba.constData ();
							json_t *json_value_p = json_object_get (row_data_p, key_s);
							QVariant v =  ptw_table_p -> horizontalHeaderItem (j) -> data (Qt::UserRole);
							QString column_type = v.toString();
							QByteArray column_type_ba = column_type.toLocal8Bit ();
							const char *column_type_s = column_type_ba.constData ();
							QTableWidgetItem *item_p = ptw_table_p -> item (i, j);
							ParameterType param_type = PT_NUM_TYPES;

							if (column_type_s)
								{
									if (!GetGrassrootsTypeFromString (column_type_s, &param_type))
										{
											PrintErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, "Failed to get datatype for column \"%s\" fpr column type \"%s\"\n", key_s, column_type_s);
										}
								}

							if (!item_p)
								{
									item_p = new QTableWidgetItem ();

									if (j >= num_cols)
										{
											ptw_table_p -> setColumnCount (num_cols + 1);
										}

									ptw_table_p -> setItem (i, j, item_p);
								}

							if (item_p)
								{
									QVariant *v_p = nullptr;

									switch (param_type)
										{
										case PT_BOOLEAN:
											if (json_is_boolean (json_value_p))
												{
													v_p = new QVariant (json_is_true (json_value_p) ? true : false);
												}
											break;

										case PT_CHAR:
											if (json_is_string (json_value_p))
												{
													const char *value_s = json_string_value (json_value_p);

													if (value_s && (strlen (value_s) == 1))
														{
															v_p = new QVariant (*value_s);
														}
												}
											break;

										case PT_SIGNED_INT:
										case PT_NEGATIVE_INT:
										case PT_UNSIGNED_INT:
											if (json_is_integer (json_value_p))
												{
													v_p = new QVariant (json_integer_value (json_value_p));
												}
											break;


										case PT_SIGNED_REAL:
										case PT_UNSIGNED_REAL:
											if (json_is_real (json_value_p))
												{
													v_p = new QVariant (json_real_value (json_value_p));
												}
											else if (json_is_integer (json_value_p))
												{
													v_p = new QVariant (static_cast <double> (json_integer_value (json_value_p)));
												}
											break;


										case PT_DIRECTORY:
										case PT_FILE_TO_READ:
										case PT_FILE_TO_WRITE:
											{
											}
											break;

										case PT_TABLE:
										case PT_STRING:
										case PT_LARGE_STRING:
										case PT_PASSWORD:
										case PT_KEYWORD:
										case PT_FASTA:
											if (json_is_string (json_value_p))
												{
													v_p = new QVariant (json_string_value (json_value_p));
												}
											break;

										case PT_JSON:
										case PT_JSON_TABLE:
											{
											}
											break;

										case PT_TIME:
											break;

										case PT_NUM_TYPES:
											break;
										}

									if (v_p)
										{
											item_p -> setData (Qt::DisplayRole, *v_p);
											delete v_p;
										}
									else
										{
											PrintErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, "No value to set\n");
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
