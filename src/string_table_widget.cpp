
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

}


bool StringTableWidget :: SetValueFromJSON (const json_t * const value_p)
{

}


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
