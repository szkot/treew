#include "widget.h"
#include <QFile>

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      clipboard(0)
{
    QWidget::showNormal();
    fileModel = new QFileSystemModel(this);
    fileModel->setRootPath(QDir::currentPath());
    treeView.setModel(fileModel);

    const QSize availableSize =
            QApplication::desktop()->availableGeometry(&treeView).size();
    treeView.resize(availableSize / 3);
    treeView.setColumnWidth(0, treeView.width() / 2);
    treeView.setContextMenuPolicy(Qt::CustomContextMenu);

    setUpGui();

    connect(&treeView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(on_context_Menu_requested(QPoint)));
    connect(&newFolderButton, SIGNAL(clicked(bool)),
            this, SLOT(newFolder()));
    connect(removeItemAction, SIGNAL(triggered(bool)),
            this, SLOT(removeItem()));
    connect(cutItemAction, SIGNAL(triggered(bool)),
            this, SLOT(cutItem()));
    connect(pasteItemAction, SIGNAL(triggered(bool)),
            this, SLOT(pasteItem()));
}

void Widget::setUpGui()
{
    newFolderNameEdit.setText("nowy folder");
    newFolderButton.setText("nowy folder");

    removeItemAction = new QAction("remove", this);
    cutItemAction = new QAction("cut", this);
    pasteItemAction = new QAction("paste", this);

    contextMenu = new QMenu(this);
    contextMenu->addAction(removeItemAction);
    contextMenu->addAction(cutItemAction);
    contextMenu->addAction(pasteItemAction);

    layout = new QVBoxLayout(this);

    layout->addWidget(&treeView);
    HLayout.addWidget(&newFolderButton);
    HLayout.addWidget(&newFolderNameEdit);
    layout->addLayout(&HLayout);
}

void Widget::newFolder()
{
    QModelIndex index = treeView.currentIndex();
    QString name = newFolderNameEdit.text();
    fileModel->mkdir(index, name);
    treeView.expand(index);
}

void Widget::removeItem()
{
    QModelIndex index = treeView.currentIndex();
    fileModel->remove(index);
}

void Widget::cutItem()
{
    clipboard = new QModelIndex(treeView.currentIndex());
}

void Widget::pasteItem()
{
    if(!clipboard)
        return;

    QModelIndex index = treeView.currentIndex();

    QString sourcePath = fileModel->filePath(*clipboard);
    QString destinationPath = fileModel->filePath(index);

    QString bug = fileModel->filePath(clipboard->parent());
    qDebug() << bug;

    if( copy(sourcePath, destinationPath) )
    {
        fileModel->remove(*clipboard);
        clipboard = 0;
    }
}

bool Widget::copy(const QString &source,
                  const QString &destination)
{
    QFileInfo sourceInfo(source);

    if(sourceInfo.isDir())
    {
        QDir destDir(destination);
        if (!destDir.mkdir((QFileInfo(source).fileName())))
            return false;

        QDir sourceDir(source);
        QStringList fileNames = sourceDir.entryList(QDir::NoDotAndDotDot | QDir::AllEntries);

        foreach(const QString &fileName, fileNames)
        {
            const QString newSrcPath = source + '/' + fileName;
            const QString newDestPath = destination + '/' + sourceDir.dirName();
            if(!copy(newSrcPath, newDestPath))
                return false;
        }
    }

    else
    {
        const QString dest = destination + QLatin1String("/") + QFileInfo(source).fileName();
        qDebug() << dest;
        if(!QFile::copy(source, dest))
            return false;
    }

    return true;
}

void Widget::on_context_Menu_requested(const QPoint &pos)
{
    QModelIndex index = treeView.indexAt(pos);
    if(index.isValid())
    {
    contextMenu->exec(mapToGlobal(pos));
    }
}

Widget::~Widget()
{

}


