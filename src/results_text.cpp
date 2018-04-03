#include "results_text.h"

#include "results_page.h"


ResultsText :: ResultsText (ResultsPage *parent_p, const json_t *results_list_json_p, const char * const service_name_)
 : ResultsProvider (parent_p)
{


}


ResultsText :: ~ResultsText ()
{
}




bool ResultsText :: AddItemFromJSON (const char * const name_s, const json_t *json_p)
{
  return false;

}
