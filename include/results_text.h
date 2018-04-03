#ifndef RESULTS_TEXT_H
#define RESULTS_TEXT_H


#include <QLabel>

#include "jansson.h"
#include "results_provider.h"


class ResultsPage;

class ResultsText : public ResultsProvider
{
	Q_OBJECT


public:

	/*********************/
	/***** FUNCTIONS *****/
	/*********************/

	ResultsText (ResultsPage *parent_p, const json_t *results_list_json_p, const char * const service_name_);

	virtual ~ResultsText ();

	virtual bool AddItemFromJSON (const char * const name_s, const json_t *json_p);


private:

	/*********************/
	/***** VARIABLES *****/
	/*********************/
	QLabel *rt_label_p;

};



#endif // RESULTS_TEXT_H
