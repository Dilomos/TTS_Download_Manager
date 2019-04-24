#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTreeWidgetItem>
#include <QGraphicsScene>

#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DInput>
#include <Qt3DLogic>
#include <Qt3DExtras>
#include <Qt3DAnimation>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static QString modPath;
    void searchGames(void);

private:
    Ui::MainWindow *ui;
    int gameCount;
    Qt3DExtras::Qt3DWindow *view;

    Qt3DCore::QEntity *rootEntity;

    Qt3DCore::QEntity *customModelEntity;

    Qt3DRender::QMesh *customModelMesh;

    Qt3DRender::QTextureLoader *customModelTextureLoader;
    Qt3DExtras::QTextureMaterial *customModelTextureMaterial;

    void init3DView(void);


public slots:
    void gameClicked(QListWidgetItem * item,QListWidgetItem * prevItem);
    void itemClicked(QTreeWidgetItem * item,QTreeWidgetItem * prevItem);

};

#endif // MAINWINDOW_H
