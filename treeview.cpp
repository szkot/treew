#include "treeview.h"
#include "widget.h"
#include <QDebug>
#include <QMimeData>
#include <QRegExp>
#include <QFileSystemModel>

TreeView::TreeView()
{
    setDragEnabled(true);
    setAcceptDrops(true);
    viewport()->setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::InternalMove);
}

void TreeView::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void TreeView::dropEvent(QDropEvent *event)
{
    QModelIndex curIndex = currentIndex();

    QFileSystemModel *fileModel =
            static_cast<QFileSystemModel *>(model());

    const QString source = fileModel->filePath(curIndex);
    QModelIndex destIndex = indexAt(event->pos());
    const QString dest = fileModel->filePath(destIndex);

    if( Widget::copy(source, dest) )
        fileModel->remove(curIndex);

    event->accept();
}

void TreeView::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

