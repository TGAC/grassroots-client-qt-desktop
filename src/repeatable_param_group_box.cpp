#include "repeatable_param_group_box.h"
#include "parameter_group.h"
#include "string_utils.h"


RepeatableParamGroupBox :: RepeatableParamGroupBox (ParameterGroup *group_p, QTParameterWidget *qt_param_widget_p)
 : QGroupBox (group_p -> pg_name_s),
	 rpgb_parameter_group_p (group_p),
   rpgb_qt_param_widget_parent_p (qt_param_widget_p)
{
  QLayout *layout_p = new QVBoxLayout;
  setLayout (layout_p);

	rpgb_parent_box_p = new QGroupBox (group_p -> pg_name_s, this);
  layout_p -> addWidget (rpgb_parent_box_p);

  layout_p = new QVBoxLayout;
  rpgb_parent_box_p -> setLayout (layout_p);

	rpgb_list_p = new QListWidget (this);

	layout_p -> addWidget (rpgb_list_p);

	QHBoxLayout *buttons_layout_p = new QHBoxLayout;
	QPushButton *row_button_p = new QPushButton (QIcon ("images/add"), "Add", this);
	//connect (add_row_button_p, &QPushButton :: clicked, this, &ParamGroupBox :: ParamGroupBoxAdded);
	buttons_layout_p -> addWidget (row_button_p);

	row_button_p = new QPushButton (QIcon ("images/remove"), "Remove", this);
	//connect (add_row_button_p, &QPushButton :: clicked, this, &ParamGroupBox :: ParamGroupBoxAdded);
	buttons_layout_p -> addWidget (row_button_p);
	layout_p -> addItem (buttons_layout_p);

  AddRepeatableGroup ();
}


RepeatableParamGroupBox :: ~RepeatableParamGroupBox ()
{
}


void RepeatableParamGroupBox ::AddParameterWidget (BaseParamWidget *widget_p)
{
	ParamGroupBox *current_row_p = rpgb_children.last ();

	current_row_p -> AddParameterWidget (widget_p);
}


QWidget *RepeatableParamGroupBox :: GetWidget ()
{
	return this;
}


void RepeatableParamGroupBox :: paintEvent (QPaintEvent *event_p)
{
	//ParamGroupBox :: paintEvent (event_p);
}


void RepeatableParamGroupBox :: AddRow (bool clicked_flag)
{
	AddRepeatableGroup ();
}


void RepeatableParamGroupBox :: AddRepeatableGroup ()
{
	char *group_name_s = GetRepeatableParameterGroupName (rpgb_parameter_group_p);

	if (group_name_s)
		{
			ParamGroupBox *new_row_p = 0;

			if (rpgb_children.size () > 0)
				{
					ParameterGroup *new_group_p = CreateAndAddParameterGroupChild (rpgb_parameter_group_p, group_name_s, true, true);

					if (new_group_p)
						{
							new_row_p = new ParamGroupBox (new_group_p, this -> rpgb_qt_param_widget_parent_p, true, true);
						}
					else
						{

						}

				}
			else
				{
					new_row_p = new ParamGroupBox (rpgb_parameter_group_p, this -> rpgb_qt_param_widget_parent_p, true, false);
				}

			if (new_row_p)
				{
					new_row_p -> setTitle (group_name_s);

					rpgb_children.append (new_row_p);
					rpgb_parent_box_p -> layout () -> addWidget (new_row_p);

					connect (new_row_p, &ParamGroupBox :: RemoveParamGroupBox, this, &RepeatableParamGroupBox :: ParamGroupBoxRemoved);
				}

			FreeCopiedString (group_name_s);
		}		/* if (group_name_s) */

}


void RepeatableParamGroupBox :: CheckVisibility (ParameterLevel level)
{
}


void RepeatableParamGroupBox :: ParamGroupBoxRemoved (ParamGroupBox *box_p)
{

}

