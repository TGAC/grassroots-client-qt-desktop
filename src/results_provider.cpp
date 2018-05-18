
#include "results_list.h"
#include "results_text.h"


ResultsProvider *ResultsProvider :: GetResultsProvider (ResultsPage *parent_p, const json_t *results_json_p, const char * const service_name_s)
{
  ResultsProvider *provider_p = 0;

  if (results_json_p)
    {
      if (json_is_array (results_json_p))
        {
          provider_p = new ResultsList (parent_p, results_json_p, service_name_s);
        }
      else if (json_is_string (results_json_p))
        {
          provider_p = new ResultsText (parent_p, results_json_p, service_name_s);
        }

    }   /* if (results_json_p) */

  return provider_p;
}




ResultsProvider :: ResultsProvider (ResultsPage *parent_p)
{
  rp_parent_p = parent_p;
}


ResultsProvider :: ~ResultsProvider ()
{}

