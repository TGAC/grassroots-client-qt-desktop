#ifndef PROGRESS_ITEM_DELEGATE_H
#define PROGRESS_ITEM_DELEGATE_H

#include <QStyledItemDelegate>

//! [0]
class ProgressItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:

    void paint (QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QSize sizeHint (const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
    QWidget *createEditor (QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void setEditorData (QWidget *editor, const QModelIndex &index) const override;
    void setModelData (QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;
};

#endif // PROGRESS_ITEM_DELEGATE_H
