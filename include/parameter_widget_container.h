#ifndef PARAMETER_WIDGET_CONTAINER_H
#define PARAMETER_WIDGET_CONTAINER_H

#include "base_param_widget.h"

class ParameterWidgetContainer
{
public:
  virtual ~ParameterWidgetContainer ();

  virtual void AddParameterWidget (BaseParamWidget *widget_p) = 0;

  virtual QWidget *GetWidget () = 0;

  virtual void CheckVisibility (ParameterLevel level) = 0;
};


#endif // PARAMETER_WIDGET_CONTAINER_H

