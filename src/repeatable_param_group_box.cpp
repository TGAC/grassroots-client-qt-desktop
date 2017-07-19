#include "repeatable_param_group_box.h"


RepeatableParamGroupBox :: RepeatableParamGroupBox (const char * const name_s, const bool visible_flag, QTParameterWidget *qt_param_widget_p)
 : QGroupBox (name_s),
   rpgb_name_s (name_s),
   rpgb_visible_flag (visible_flag),
   rpgb_qt_param_widget_parent_p (qt_param_widget_p)
{
  QLayout *layout_p = new QVBoxLayout;
  setLayout (layout_p);

  rpgb_parent_box_p = new QGroupBox (name_s, this);
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
  int i = rpgb_children.size ();
  QString s = QString::number(i);
  const QByteArray &ba_r = s.toLocal8Bit ();
  const char *name_s = ba_r.constData ();

  ParamGroupBox *first_row_p = rpgb_children.first ();

  ParamGroupBox *new_row_p = new ParamGroupBox (name_s, rpgb_visible_flag, rpgb_qt_param_widget_parent_p);
  rpgb_children.append (new_row_p);

  rpgb_parent_box_p -> layout () -> addWidget (new_row_p);
}


void RepeatableParamGroupBox :: CheckVisibility (ParameterLevel level)
{
}



