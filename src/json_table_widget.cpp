

#include "json_table_widget.h"



JSONTableWidget :: JSONTableWidget (JSONParameter * const param_p, QTParameterWidget * const parent_p)
	: BaseTableWidget (& (param_p -> jp_base_param), parent_p)
{
	jtw_param_p = param_p;
}


JSONTableWidget :: ~JSONTableWidget ()
{

}


void JSONTableWidget :: SetDefaultValue ()
{
	const json_t *def_value_p = GetJSONParameterDefaultValue (jtw_param_p);

	if (def_value_p)
		{

		}
	else
		{
			ClearTable ();
		}
}


bool JSONTableWidget :: SetValueFromText (const char *value_s)
{

}


bool JSONTableWidget :: SetValueFromJSON (const json_t * const value_p)
{

}


bool JSONTableWidget :: StoreParameterValue ()
{
	bool success_flag = false;
	json_t *table_json_p = ptw_table_p -> GetValueAsJSON ();

	if (table_json_p)
		{
			success_flag = SetJSONParameterCurrentValue (jtw_param_p, table_json_p);

			json_decref (table_json_p);
		}

	return success_flag;
}


void JSONTableWidget :: Clear ()
{
	BaseTableWidget :: Clear ();

	SetJSONParameterCurrentValue (jtw_param_p, nullptr);
}
