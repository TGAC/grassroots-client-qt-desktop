#include "labels_widget.h"

#include <QHBoxLayout>

LabelsWidget :: LabelsWidget (const char *label_s, QWidget *parent_p)
	: QWidget (parent_p)
{
	lw_title_p = new QLabel (label_s);
	lw_icon_p = new QLabel ();

	/*
	 * The icon used to show an error is 24x24 pixels
	 * so to stop the label moving around, hard-code the
	 * size
	 */
	lw_icon_p -> setMinimumSize (24, 24);
	lw_icon_p -> setMaximumSize (24, 24);

	QHBoxLayout *layout_p = new QHBoxLayout ();

	layout_p -> addWidget (lw_icon_p);
	layout_p -> addWidget (lw_title_p);

	layout_p -> setContentsMargins (0, 0, 0, 0);

	setLayout (layout_p);
}

LabelsWidget :: ~LabelsWidget ()
{

}

void LabelsWidget :: SetText (const char *text_s)
{
	lw_title_p -> setText (text_s);
}


void LabelsWidget :: SetError (const QString &message_r)
{
	lw_icon_p -> setPixmap (QPixmap ("images/warning"));
	setToolTip (message_r);
}


void LabelsWidget :: ClearError ()
{
	lw_icon_p -> setPixmap (QPixmap ());
	setToolTip (QString ());
}
