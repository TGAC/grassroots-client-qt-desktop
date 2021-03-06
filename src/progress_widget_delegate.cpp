
#include "progress_widget_delegate.h"



ProgressWidgetDelegate :: ProgressWidgetDelegate (QObject *parent_p, ProgressWidget *progress_p)
{
	pwd_editor_p = progress_p;
}


QWidget *ProgressWidgetDelegate :: createEditor (QWidget *parent_p, const QStyleOptionViewItem &option_r, const QModelIndex &index_r) const
{
  return pwd_editor_p;
}


void ProgressWidgetDelegate :: setEditorData (QWidget *editor_p, const QModelIndex &index_r) const
{
  QVariant v = index_r.model () -> data (index_r, Qt :: EditRole);

}


void ProgressWidgetDelegate :: setModelData (QWidget *editor_p, QAbstractItemModel *model_p, const QModelIndex &index_r) const
{
  QVariant v = index_r.model () -> data (index_r, Qt :: EditRole);
  v.data();
}


void ProgressWidgetDelegate :: updateEditorGeometry (QWidget *editor_p, const QStyleOptionViewItem &option_r, const QModelIndex &index_r) const
{
  pwd_editor_p -> setGeometry (option_r.rect);
}
