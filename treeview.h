#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QTreeView>
#include <QMouseEvent>

class TreeView : public QTreeView
{
public:
    TreeView();
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
private:
};

#endif // TREEVIEW_H
