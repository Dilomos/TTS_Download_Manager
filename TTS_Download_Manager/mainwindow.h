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
#include <QThread>
#include <QProgressBar>

namespace Ui {
class MainWindow;
}

class QLabel;

class GameLoaderThread : public QThread
{
    Q_OBJECT

public:
    explicit GameLoaderThread(QListWidget* m_listToLoad,int id,int startIndex,int lastIndex);
protected:
    void run() override;

signals:
    void readingGameNumber(const int &num,int threadID);

private:
  QListWidget* m_listToLoad;

  int m_id;
  int m_startIndex;
  int m_lastIndex;


};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static QString modPath;
    static QString exportPath;
    void searchGames(void);

private:
    Ui::MainWindow *ui;
    int gameCount;
    int gameWithMissingFilesCount;
    Qt3DExtras::Qt3DWindow *view;

    Qt3DCore::QEntity *rootEntity;

    Qt3DCore::QEntity *customModelEntity;

    Qt3DRender::QMesh *customModelMesh;

    Qt3DRender::QTextureLoader *customModelTextureLoader;
    Qt3DExtras::QTextureMaterial *customModelTextureMaterial;

    void getParameterFromFile();
    void init3DView(void);

    QVector<int> m_threadVal;
    int m_threadFinishedCount;

    void saveParametersToFile(void);


public slots:
    void gameClicked(QListWidgetItem * item,QListWidgetItem * prevItem);
    void itemClicked(QTreeWidgetItem * item,QTreeWidgetItem * prevItem);

    void openFileExternally(void);

    void updateProgressBar(const int &val,int threadID);
    void hideProgressBar(void);

    void setModsFolder(bool checked);
    void setExportFolder(bool checked);

};

#endif // MAINWINDOW_H
