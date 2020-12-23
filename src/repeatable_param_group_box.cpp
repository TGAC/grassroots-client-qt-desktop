#include <QDebug>


#include "repeatable_param_group_box.h"
#include "parameter_group.h"
#include "string_utils.h"
#include "qt_parameter_widget.h"
#include "qt_client_data.h"


RepeatableParamGroupBox :: RepeatableParamGroupBox (ParameterGroup *group_p, QTParameterWidget *qt_param_widget_p,  bool removable_flag, bool add_params_flag)
 : ParamGroupBox (group_p, qt_param_widget_p, removable_flag, add_params_flag)
{
	rpgb_entries_p = new QListWidget (this);
	rpgb_entries_p -> setSelectionMode (QAbstractItemView :: SingleSelection);

	rpgb_label_param_p = nullptr;

	if (group_p -> pg_repeatable_param_p)
		{
			rpgb_label_param_p = group_p -> pg_repeatable_param_p;
		}

}


RepeatableParamGroupBox :: ~RepeatableParamGroupBox ()
{
}



void RepeatableParamGroupBox :: init (bool add_params_flag)
{
	QVBoxLayout *layout_p = new QVBoxLayout ();

	qDebug () << "RepeatableParamGroupBox :: init for " << pgb_parameter_group_p -> pg_name_s << Qt :: endl;


	layout_p -> addWidget (rpgb_entries_p);

	QHBoxLayout *buttons_layout_p = new QHBoxLayout;
	QPushButton *row_button_p = new QPushButton (QIcon ("images/add"), "Add", this);
	connect (row_button_p, &QPushButton :: clicked, this, &RepeatableParamGroupBox :: AddEntry);
	buttons_layout_p -> addWidget (row_button_p);

	rpgb_remove_entry_button_p = new QPushButton (QIcon ("images/remove"), "Remove", this);
	connect (rpgb_remove_entry_button_p, &QPushButton :: clicked, this, &RepeatableParamGroupBox :: RemoveEntry);
	buttons_layout_p -> addWidget (rpgb_remove_entry_button_p);

	layout_p -> addItem (buttons_layout_p);

	layout_p -> addItem (pgb_layout_p);


	connect (rpgb_entries_p, &QListWidget :: itemSelectionChanged, this, &RepeatableParamGroupBox :: SelectedListEntryChanged);

	setLayout (layout_p);
}


void RepeatableParamGroupBox :: AddEntry ()
{
	StoreValues ();

	const SchemaVersion *sv_p = pgb_parent_p -> GetClientData () -> qcd_base_data.cd_schema_p;
	json_t *group_json_p = GetParameterGroupAsJSON (pgb_parameter_group_p, true, false, sv_p);
	QString label;
	bool set_label_flag = false;
	Parameter *param_p = pgb_parameter_group_p -> pg_repeatable_param_p;

	if (param_p)
		{
			bool alloc_flag = false;
			char *value_s = GetParameterValueAsString (param_p, &alloc_flag);

			if (value_s)
				{
					label = value_s;
					set_label_flag = true;
				}

			if (alloc_flag)
				{
					FreeCopiedString (value_s);
				}
		}

	if (!set_label_flag)
		{
			int i = rpgb_entries_p -> count ();
			label = QString :: number (i);
		}

	QListWidgetItem *item_p = new QListWidgetItem (label, rpgb_entries_p);

	char *value_s = json_dumps (group_json_p, 0);
	item_p -> setData (Qt :: UserRole, value_s);


	qDebug () << "adding " << label << ": " << item_p -> data (Qt :: UserRole) << Qt :: endl;


	rpgb_entries_p -> addItem (item_p);
}

void RepeatableParamGroupBox :: RemoveEntry ()
{
	QList <QListWidgetItem *> l =	rpgb_entries_p -> selectedItems ();
	const int count = l.size ();

	if (count == 1)
		{
			QListWidgetItem *item_p = l.at (0);

			qDebug () << "removing " << item_p -> text () << ": " << item_p -> data (Qt :: UserRole) << Qt :: endl;

			rpgb_entries_p -> removeItemWidget (item_p);
			delete item_p;
		}
}


void RepeatableParamGroupBox :: SelectedListEntryChanged ()
{
	QList <QListWidgetItem *> l =	rpgb_entries_p -> selectedItems ();
	const int count = l.size ();

	if (count == 1)
		{
			QListWidgetItem *item_p = l.at (0);
			qDebug () << "selected " << item_p -> text () << ": " << item_p -> data (Qt :: UserRole) << Qt :: endl;
		}

	if (count > 0)
		{
			rpgb_remove_entry_button_p -> setEnabled (true);
		}
	else
		{
			rpgb_remove_entry_button_p -> setEnabled (false);
		}
}
