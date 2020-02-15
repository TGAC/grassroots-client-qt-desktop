
#include "string_table_widget.h"

#include "string_utils.h"




StringTableWidget :: StringTableWidget (StringParameter * const param_p, QTParameterWidget * const parent_p)
	: BaseTableWidget (& (param_p -> sp_base_param), parent_p)
{
	stw_param_p = param_p;
}


StringTableWidget :: ~StringTableWidget ()
{

}


void StringTableWidget :: SetDefaultValue ()
{
	const char *def_value_p = GetStringParameterDefaultValue (stw_param_p);

	if (def_value_p)
		{

		}
	else
		{
			ClearTable ();
		}
}


bool StringTableWidget :: SetValueFromText (const char *value_s)
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


bool StringTableWidget :: SetValueFromJSON (const json_t * const value_p)
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

		return success_flag;}


bool StringTableWidget :: StoreParameterValue ()
{
	bool success_flag = false;
	char *table_data_p = ptw_table_p -> GetValueAsText ();

	if (table_data_p)
		{
			success_flag = SetStringParameterCurrentValue (stw_param_p, table_data_p);

			FreeCopiedString (table_data_p);
		}

	return success_flag;
}


void StringTableWidget :: Clear ()
{
	BaseTableWidget :: Clear ();

	SetStringParameterCurrentValue (stw_param_p, nullptr);
}
