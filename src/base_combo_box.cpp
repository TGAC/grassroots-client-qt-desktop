#include "base_combo_box.h"
#include "qt_parameter_widget.h"


BaseComboBox :: BaseComboBox (Parameter * const param_p, QTParameterWidget * const parent_p)
	: BaseParamWidget (param_p, parent_p)
{
	bcb_combo_box_p = new QComboBox;

	if (param_p -> pa_refresh_service_flag)
		{
			QObject ::  connect (bcb_combo_box_p,  &QComboBox :: currentTextChanged, parent_p, &QTParameterWidget :: RefreshService);
		}
}

BaseComboBox :: ~BaseComboBox ()
{
	delete bcb_combo_box_p;
}


QWidget *BaseComboBox :: GetQWidget ()
{
	return bcb_combo_box_p;
}


bool BaseComboBox :: IsComboBoxWidget () const
{
	return true;
}
