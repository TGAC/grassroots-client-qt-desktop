#include "base_combo_box.h"



BaseComboBox :: BaseComboBox (Parameter * const param_p, QTParameterWidget * const parent_p)
	: BaseParamWidget (param_p, parent_p)
{
	bcb_combo_box_p = new QComboBox;
}

BaseComboBox :: ~BaseComboBox ()
{
	delete bcb_combo_box_p;
}


QWidget *BaseComboBox :: GetQWidget ()
{
	return bcb_combo_box_p;
}

