
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

	const json_t *param_value_p = json_object_get (value_p, PARAM_CURRENT_VALUE_S);

	if (param_value_p)
		{
			if (json_is_string (param_value_p))
				{
					const char *value_s = json_string_value (value_p);
					success_flag = SetValueFromText (value_s);
				}
		}

	return success_flag;
}


bool StringTableWidget :: SetFromParameterValue (Parameter *param_p)
{
	bool success_flag = false;

	if (IsStringParameter (param_p))
		{
			StringParameter *str_param_p = reinterpret_cast <StringParameter *> (param_p);
			const char *value_s = GetStringParameterCurrentValue (str_param_p);

			success_flag = SetValueFromText (value_s);

			SetWidgetEnabled (! (param_p -> pa_read_only_flag));

		}

	return success_flag;
}


bool StringTableWidget :: StoreParameterValue (bool refresh_flag)
{
	bool success_flag = false;
	char *table_data_p = ptw_table_p -> GetValueAsText (&success_flag);

	if (success_flag && table_data_p)
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
