#include "repeatable_param_group_box.h"
#include "parameter_group.h"


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

  rpgb_add_row_btn_p = new QPushButton (QIcon ("images/add"), "Add Group", this);
  connect (rpgb_add_row_btn_p, &QPushButton :: clicked, this, &RepeatableParamGroupBox :: AddRow);

  layout_p -> addWidget (rpgb_add_row_btn_p);

  AddRow (false);
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
	ParamGroupBox *first_row_p = rpgb_children.first ();

	ParamGroupBox *new_row_p = first_row_p -> Clone (rpgb_parameter_group_p);

	if (new_row_p)
		{
			rpgb_children.append (new_row_p);

			rpgb_parent_box_p -> layout () -> addWidget (new_row_p);
		}
	else
		{

		}
}


void RepeatableParamGroupBox :: CheckVisibility (ParameterLevel level)
{
}



