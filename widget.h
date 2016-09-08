#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileSystemModel>
#include "treeview.h"
#include <QtWidgets>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    static bool copy(const QString &source,
              const QString &destination);

private slots:
    void on_context_Menu_requested(const QPoint &pos);
    void newFolder();
    void removeItem();
    void cutItem();
    void pasteItem();

private:
    QFileSystemModel *fileModel;
    QMenu *contextMenu;
    QAction *removeItemAction;
    QAction *cutItemAction;
    QAction *pasteItemAction;
    QVBoxLayout *layout;
    QModelIndex *clipboard;

    TreeView treeView;
    QPushButton newFolderButton;
    QHBoxLayout HLayout;

    QLineEdit newFolderNameEdit;

    void setUpGui();
};

#endif // WIDGET_H
