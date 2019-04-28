#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tts_game.h"
#include "tools.h"
#include "orbittransformcontroller.h"
#include "tts_custom_model.h"
#include "tts_customimage.h"

#include <QTextStream>
#include <QDirIterator>

QString MainWindow::modPath="";

GameLoaderThread::GameLoaderThread(QListWidget* m_listToLoad)
    :m_listToLoad(m_listToLoad)
{}

void GameLoaderThread::run() {
    QString result("OK");
    for (int i = 0; i < m_listToLoad->count(); i++) {
        emit readingGameNumber(i);
        dynamic_cast <TTS_Game*>(m_listToLoad->item(i))->loadGameFromFile();
        }
    emit resultReady(result);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->progressBar->setVisible(false);
    ui->widgetTexture->setVisible(false);

    init3DView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::searchGames(void)
{

    //modPath="E:/Coding/github/TTS_Download_Manager/Mods";
    modPath="G:/Jeux/SteamLibrary/SteamApps/common/Tabletop Simulator/Tabletop Simulator_Data/Mods";
    //modPath="C:/Users/E9868092/Downloads/perso/TTS/Mods";

    QString workshopPath=modPath+"/Workshop";
   /* QString modelPath=modPath+"/Models";
    QString imgPath=modPath+"/Images";
*/


    QStringList listFilter;
    listFilter << "*.json";

    QDirIterator dirIterator(workshopPath, listFilter ,QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);

    QStringList fileList;
    while(dirIterator.hasNext())
    {
        fileList << dirIterator.next();
    }

    gameCount=fileList.length();
    ui->lGameCount->setText("0/"+QString::number(gameCount));

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(gameCount);
    ui->progressBar->setVisible(true);

    for(int i=0 ; (i < gameCount) ; i++)
    {
        ui->progressBar->setValue(i);
        TTS_Game *game=new TTS_Game(fileList.at(i),i+1);
        ui->listWidget->addItem(game);
        //QTextStream(stdout) << fileList.at(i)<<endl;
    }

    //ui->progressBar->setVisible(false);

    connect(ui->listWidget, &QListWidget::currentItemChanged, this,&MainWindow::gameClicked);
    connect(ui->treeWidget, &QTreeWidget::currentItemChanged, this,&MainWindow::itemClicked);

    GameLoaderThread *gameLoaderThread = new GameLoaderThread(ui->listWidget);
        connect(gameLoaderThread, &GameLoaderThread::readingGameNumber, this, &MainWindow::updateProgressBar);
        connect(gameLoaderThread, &GameLoaderThread::finished, gameLoaderThread, &QObject::deleteLater);
        gameLoaderThread->start();
}

void MainWindow::updateProgressBar(const int &val)
{
    ui->progressBar->setValue(val);
    if(val == ui->progressBar->maximum())
         ui->progressBar->setVisible(false);
}

void MainWindow::init3DView(void)
{
    view = new Qt3DExtras::Qt3DWindow();

    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
    QWidget *container = QWidget::createWindowContainer(view);
    //container->setParent(ui->widget);
    QSize screenSize = view->screen()->size();
    container->setMinimumSize(QSize(200, 100));
    container->setMaximumSize(screenSize);

    QHBoxLayout *hLayout = new QHBoxLayout(ui->widget3D);
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setAlignment(Qt::AlignTop);
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);

    Qt3DInput::QInputAspect *input = new Qt3DInput::QInputAspect;
    view->registerAspect(input);

    rootEntity = new Qt3DCore::QEntity;

    Qt3DRender::QCamera *camera = view->camera();
    camera->lens()->setPerspectiveProjection(40.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(0, 0, 20.0f));
    camera->setViewCenter(QVector3D(0, 0, 0));

    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(0.8f);
    lightEntity->addComponent(light);

    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(QVector3D(60, 0, 40.0f));
    lightEntity->addComponent(lightTransform);

    Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    camController->setCamera(camera);

    customModelEntity = new Qt3DCore::QEntity(rootEntity);
    customModelMesh = new Qt3DRender::QMesh;
    customModelMesh->setSource(QUrl(QLatin1String("qrc:/empty.obj")));

    customModelTextureLoader= new Qt3DRender::QTextureLoader;
    customModelTextureLoader->setSource(QUrl(QLatin1String("qrc:/empty.png")));

    customModelTextureMaterial = new Qt3DExtras::QTextureMaterial;


    Qt3DCore::QTransform *sphereTransform = new Qt3DCore::QTransform;
    OrbitTransformController *controller = new OrbitTransformController(sphereTransform);
    controller->setTarget(sphereTransform);
    controller->setRadius(0.0f);

    QPropertyAnimation *sphereRotateTransformAnimation = new QPropertyAnimation(sphereTransform);
    sphereRotateTransformAnimation->setTargetObject(controller);
    sphereRotateTransformAnimation->setPropertyName("angle");
    sphereRotateTransformAnimation->setStartValue(QVariant::fromValue(0));
    sphereRotateTransformAnimation->setEndValue(QVariant::fromValue(360));
    sphereRotateTransformAnimation->setDuration(10000);
    sphereRotateTransformAnimation->setLoopCount(-1);
    sphereRotateTransformAnimation->start();

    customModelEntity->addComponent(customModelMesh);
    customModelEntity->addComponent(sphereTransform);
    customModelEntity->addComponent(customModelTextureMaterial);

    view->setRootEntity(rootEntity);
    view->show();

    ui->widget3D->setVisible(false);
}

void MainWindow::gameClicked(QListWidgetItem * item,QListWidgetItem * prevItem)
{
    dynamic_cast <TTS_Game*>(item)->loadGameFromFile();

    ui->label->setPixmap(QPixmap(dynamic_cast <TTS_Game*>(item)->getPixPath()));
    ui->statusBar->showMessage(dynamic_cast <TTS_Game*>(item)->getFileName());
    ui->lGameCount->setText(QString::number(dynamic_cast <TTS_Game*>(item)->getIndex())+"/"+QString::number(gameCount));


    ui->treeWidget->takeTopLevelItem(0);
    ui->treeWidget->addTopLevelItem(dynamic_cast <QTreeWidgetItem*>(item));
    ui->treeWidget->expandAll();
    ui->treeWidget->header()->resizeSections(QHeaderView::ResizeToContents);
}

void MainWindow::itemClicked(QTreeWidgetItem * item,QTreeWidgetItem * prevItem)
{
    if(item)
    {
        ui->widgetTexture->setVisible(false);
        ui->widget3D->setVisible(false);
        if(dynamic_cast <TTS_TreeWidgetItem*>(item)->isDisplayable())
        {
            if(dynamic_cast <TTS_TreeWidgetItem*>(item)->is3DModel())
            {
                ui->widget3D->setVisible(true);

                QString modelPath=dynamic_cast <TTS_Custom_Model*>(item)->getLocalModel();
                QString texturePath=dynamic_cast <TTS_Custom_Model*>(item)->getLocalTexture();

                customModelEntity->removeComponent(customModelMesh);
                customModelEntity->removeComponent(customModelTextureMaterial);

                if(modelPath !=  "")
                    customModelMesh->setSource(QUrl::fromLocalFile(modelPath));
                else
                    customModelMesh->setSource(QUrl(QLatin1String("qrc:/empty.obj")));

                if(texturePath !=  "")
                    customModelTextureLoader->setSource(QUrl::fromLocalFile(texturePath));
                else
                    customModelTextureLoader->setSource(QUrl(QLatin1String("qrc:/empty.png")));

                customModelTextureMaterial->setTexture(customModelTextureLoader);

                customModelEntity->addComponent(customModelMesh);
                customModelEntity->addComponent(customModelTextureMaterial); 
            }

            if(dynamic_cast <TTS_TreeWidgetItem*>(item)->isTexture())
            {
                ui->widgetTexture->setVisible(true);

                QString frontImgPath=dynamic_cast <TTS_CustomImage*>(item)->getLocalImageFront();
                QString backImgPath=dynamic_cast <TTS_CustomImage*>(item)->getLocalImageBack();

                if(frontImgPath != "")
                {
                    ui->frontImgLabel->setVisible(true);
                    ui->le_ImgFrontLocal->setVisible(true);
                    ui->le_ImgFrontOnline->setVisible(true);

                    QPixmap p(frontImgPath);
                    int wlabel = ui->frontImgLabel->width();
                    int hlabel = ui->frontImgLabel->height();

                    ui->le_ImgFrontLocal->setText(frontImgPath);
                    ui->le_ImgFrontOnline->setText(dynamic_cast <TTS_CustomImage*>(item)->getOnlineImageFront());

                    if(wlabel<p.width() || hlabel<p.height())
                    {
                        ui->frontImgLabel->setPixmap(p.scaled(wlabel,hlabel,Qt::KeepAspectRatio));
                    }
                    else
                    {
                        ui->frontImgLabel->setPixmap(p);
                    }
                }
                else
                {
                    ui->frontImgLabel->setVisible(false);
                    ui->le_ImgFrontLocal->setVisible(false);
                    ui->le_ImgFrontOnline->setVisible(false);
                }

                if(backImgPath != "")
                {
                    ui->backImgLabel->setVisible(true);
                    ui->le_ImgBackLocal->setVisible(true);
                    ui->le_ImgBackOnline->setVisible(true);

                    QPixmap p(backImgPath);
                    int wlabel = ui->frontImgLabel->width();
                    int hlabel = ui->frontImgLabel->height();

                    ui->le_ImgBackLocal->setText(backImgPath);
                    ui->le_ImgBackOnline->setText(dynamic_cast <TTS_CustomImage*>(item)->getOnlineImageBack());

                    if(wlabel<p.width() || hlabel<p.height())
                    {
                        ui->backImgLabel->setPixmap(p.scaled(wlabel,hlabel,Qt::KeepAspectRatio));
                    }
                    else
                    {
                        ui->backImgLabel->setPixmap(p);
                    }

                }
                else
                {
                    ui->backImgLabel->setVisible(false);
                    ui->le_ImgBackLocal->setVisible(false);
                    ui->le_ImgBackOnline->setVisible(false);
                }
            }
        }
    }
}
