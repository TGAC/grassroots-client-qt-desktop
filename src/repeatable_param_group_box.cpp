#include "repeatable_param_group_box.h"


RepeatableParamGroupBox :: RepeatableParamGroupBox (const char * const name_s, const bool visible_flag)
 : QGroupBox (name_s)
{
  rpgb_add_row_btn_p = new QPushButton (QIcon ("images/add"), "Add Group", this);

  QLayout *layout_p = layout ();
  layout_p -> addWidget (rpgb_add_row_btn_p);

  connect (rpgb_add_row_btn_p, &QPushButton :: clicked, this, &RepeatableParamGroupBox :: AddRow);
}


RepeatableParamGroupBox :: ~RepeatableParamGroupBox ()
{
}


void RepeatableParamGroupBox ::AddParameterWidget (BaseParamWidget *widget_p)
{
	//ParamGroupBox :: AddParameterWidget (widget_p);
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

}


void RepeatableParamGroupBox :: CheckVisibility (ParameterLevel level)
{
}



