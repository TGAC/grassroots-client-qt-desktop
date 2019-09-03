#ifndef PROGRESS_WIDGET_DELEGATE_H
#define PROGRESS_WIDGET_DELEGATE_H

#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>

#include "progress_widget.h"


class ProgressWidgetDelegate : public QItemDelegate
{
  Q_OBJECT

public:
  explicit ProgressWidgetDelegate (QObject *parent_p, ProgressWidget *progress_p);

  // Create Editor when we construct ProgressWidgetDelegate
  virtual QWidget *createEditor (QWidget *parent_p, const QStyleOptionViewItem &option_r, const QModelIndex &index_r) const;

  // Then, we set the Editor
  virtual void setEditorData (QWidget *editor_p, const QModelIndex &index_r) const;

  // When we modify data, this model reflect the change
  virtual void setModelData (QWidget *editor_p, QAbstractItemModel *model_p, const QModelIndex &index_r) const;

  /*
   * Set the geometry for the list item's widget
   */
  virtual void updateEditorGeometry (QWidget *editor_p, const QStyleOptionViewItem &option_r, const QModelIndex &index_r) const;

signals:

public slots:

private:
  ProgressWidget *pwd_editor_p;
};


#endif // PROGRESS_WIDGET_DELEGATE_H
