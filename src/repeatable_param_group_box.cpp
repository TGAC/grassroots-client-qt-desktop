#include <QDebug>


#include "repeatable_param_group_box.h"
#include "parameter_group.h"
#include "string_utils.h"
#include "qt_parameter_widget.h"


RepeatableParamGroupBox :: RepeatableParamGroupBox (ParameterGroup *group_p, QTParameterWidget *qt_param_widget_p,  bool removable_flag, bool add_params_flag)
 : ParamGroupBox (group_p, qt_param_widget_p, removable_flag, add_params_flag)
{
	rpgb_label_param_p = nullptr;

	const char *label_s = group_p -> pg_repeatable_param_label_s;

	if (label_s)
		{
			QList <BaseParamWidget *> :: const_iterator itr = pgb_children.cbegin ();

			while ((itr != pgb_children.end ()) && (rpgb_label_param_p == nullptr))
				{
					BaseParamWidget *widget_p = *itr;
					const char *name_s = widget_p -> GetParameterName ();

					if (strcmp (name_s, label_s) == 0)
						{
							rpgb_label_param_p = widget_p;
						}
					else
						{
							++ itr;
						}
				}

		}
}


RepeatableParamGroupBox :: ~RepeatableParamGroupBox ()
{
}



void RepeatableParamGroupBox :: init (bool add_params_flag)
{
	QVBoxLayout *layout_p = new QVBoxLayout ();

	qDebug () << "RepeatableParamGroupBox :: init for " << pgb_parameter_group_p -> pg_name_s << Qt :: endl;

	rpgb_entries_p = new QListWidget (this);
	layout_p -> addWidget (rpgb_entries_p);

	QHBoxLayout *buttons_layout_p = new QHBoxLayout;
	QPushButton *row_button_p = new QPushButton (QIcon ("images/add"), "Add", this);
	//connect (add_row_button_p, &QPushButton :: clicked, this, &ParamGroupBox :: ParamGroupBoxAdded);
	buttons_layout_p -> addWidget (row_button_p);

	row_button_p = new QPushButton (QIcon ("images/remove"), "Remove", this);
	//connect (add_row_button_p, &QPushButton :: clicked, this, &ParamGroupBox :: ParamGroupBoxAdded);
	buttons_layout_p -> addWidget (row_button_p);

	layout_p -> addItem (buttons_layout_p);

	layout_p -> addItem (pgb_layout_p);

	setLayout (layout_p);
}


void RepeatableParamGroupBox :: AddEntry ()
{
	json_t *group_json_p = GetParameterGroupAsJSON (pgb_parameter_group_p);
	char *label_s;


	if (0 /* rpgb_label_param_p */)
		{

		}
	else
		{
			int i = rpgb_entries_p -> count ();

		}

	QListWidgetItem *item_p = new QListWidgetItem ("blah", rpgb_entries_p);

	char *value_s = json_dumps (group_json_p, 0);
	item_p -> setData (Qt::UserRole, value_s);

	rpgb_entries_p -> addItem (item_p);
}

void RepeatableParamGroupBox :: RemoveEntry ()
{

}


