#ifndef LABELS_WIDGET_H
#define LABELS_WIDGET_H

#include <QWidget>
#include <QLabel>


class LabelsWidget : public QWidget
{
public:
	LabelsWidget (const char *label_s, QWidget *parent_p = nullptr);
	~LabelsWidget ();

	void SetError (const QString &message_r);
	void ClearError ();

private:
	QLabel *lw_icon_p;
	QLabel *lw_title_p;
};

#endif // LABELS_WIDGET_H
