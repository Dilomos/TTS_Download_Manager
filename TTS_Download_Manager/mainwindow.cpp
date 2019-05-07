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
    for (int i = 0; i < m_listToLoad->count(); i++) {
        emit readingGameNumber(i);
        dynamic_cast <TTS_Game*>(m_listToLoad->item(i))->loadGameFromFile();
        }
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
        QString file = dirIterator.next();
        if(dirIterator.fileName() != "WorkshopFileInfos.json")
            fileList << file;
    }

    gameCount=fileList.length();

    ui->lGameCount->setText(QString::number(gameCount)+" Json Mods");

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

    connect(ui->listWidget, &QListWidget::currentItemChanged, this,&MainWindow::gameClicked);
    connect(ui->treeWidget, &QTreeWidget::currentItemChanged, this,&MainWindow::itemClicked);

    GameLoaderThread *gameLoaderThread = new GameLoaderThread(ui->listWidget);
    connect(gameLoaderThread, &GameLoaderThread::readingGameNumber, this, &MainWindow::updateProgressBar);
    connect(gameLoaderThread, &GameLoaderThread::finished, this, &MainWindow::hideProgressBar);
    connect(gameLoaderThread, &GameLoaderThread::finished, gameLoaderThread, &QObject::deleteLater);
    gameLoaderThread->start();
}

void MainWindow::updateProgressBar(const int &val)
{
    ui->progressBar->setValue(val);
}

void MainWindow::hideProgressBar(void)
{
    ui->progressBar->setVisible(false);
    ui->listWidget->sortItems();

    QListWidgetItem *itemToStop=nullptr;
    gameWithMissingFilesCount=0;
    for (int i = (ui->listWidget->count()-1); i >= 0;)
    {
        if(ui->listWidget->item(i) == itemToStop)
            break;

        if(dynamic_cast <TTS_Game*>(ui->listWidget->item(i))->isSomethingMissing())
        {
            if(itemToStop == nullptr)
                itemToStop = ui->listWidget->item(i);

            ui->listWidget->insertItem(0,ui->listWidget->takeItem(i));
            gameWithMissingFilesCount++;
        }
        else
        {
            i--;
        }
    }
    QString gameCountLabelTmp=QString::number(gameCount)+" Json Mods";
    if(gameWithMissingFilesCount != 0)
        gameCountLabelTmp+=" / "+QString::number(gameWithMissingFilesCount)+" Mods with missing files";
    ui->lGameCount->setText(gameCountLabelTmp);

    ui->listWidget->setCurrentRow(0);
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

    //QHBoxLayout *hLayout = new QHBoxLayout(ui->widget3D);
    //QVBoxLayout *vLayout = new QVBoxLayout(ui->widget3D);
    ui->verticalLayout_9->setAlignment(Qt::AlignTop);
    ui->verticalLayout_9->insertWidget(0,container, 1);
    //hLayout->addLayout(vLayout);

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
    if(item != nullptr)
    {
        ui->widgetTexture->setVisible(false);
        ui->widget3D->setVisible(false);

        dynamic_cast <TTS_Game*>(item)->loadGameFromFile();

        QString fileCountLabelTmp=QString::number(dynamic_cast <TTS_Game*>(item)->getFileCount())+" files";
        if(gameWithMissingFilesCount != 0)
            fileCountLabelTmp+=" / "+QString::number(dynamic_cast <TTS_Game*>(item)->getMissingFileCount())+" missing files";
        ui->lFileCount->setText(fileCountLabelTmp);

        ui->label->setPixmap(QPixmap(dynamic_cast <TTS_Game*>(item)->getPixPath()));
        ui->le_gameName->setText(dynamic_cast <TTS_Game*>(item)->getFileName());
        //ui->statusBar->showMessage(dynamic_cast <TTS_Game*>(item)->getFileName());
        //ui->lGameCount->setText(QString::number(dynamic_cast <TTS_Game*>(item)->getIndex())+"/"+QString::number(gameCount));


        ui->treeWidget->takeTopLevelItem(0);
        ui->treeWidget->addTopLevelItem(dynamic_cast <QTreeWidgetItem*>(item));
        ui->treeWidget->expandAll();
        ui->treeWidget->header()->resizeSections(QHeaderView::ResizeToContents);
    }
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

                ui->l_model->setVisible(true);
                ui->l_diffuse->setVisible(true);
                ui->l_normal->setVisible(true);
                ui->l_collider->setVisible(true);

                ui->l_ModelLocal->setVisible(true);
                ui->l_ModelOnline->setVisible(true);
                ui->l_NormalLocal->setVisible(true);
                ui->l_NormalOnline->setVisible(true);
                ui->l_DiffuseLocal->setVisible(true);
                ui->l_DiffuseOnline->setVisible(true);
                ui->l_ColliderLocal->setVisible(true);
                ui->l_ColliderOnline->setVisible(true);

                ui->le_ModelLocal->setVisible(true);
                ui->le_ModelOnline->setVisible(true);
                ui->le_NormalLocal->setVisible(true);
                ui->le_NormalOnline->setVisible(true);
                ui->le_DiffuseLocal->setVisible(true);
                ui->le_DiffuseOnline->setVisible(true);
                ui->le_ColliderLocal->setVisible(true);
                ui->le_ColliderOnline->setVisible(true);

                ui->le_ModelOnline->setStyleSheet("QLineEdit { background: rgb(255, 255, 255); selection-background-color: rgb(255, 255, 255); }");
                ui->le_NormalOnline->setStyleSheet("QLineEdit { background: rgb(255, 255, 255); selection-background-color: rgb(255, 255, 255); }");
                ui->le_DiffuseOnline->setStyleSheet("QLineEdit { background: rgb(255, 255, 255); selection-background-color: rgb(255, 255, 255); }");
                ui->le_ColliderOnline->setStyleSheet("QLineEdit { background: rgb(255, 255, 255); selection-background-color: rgb(255, 255, 255); }");


                customModelEntity->removeComponent(customModelMesh);
                customModelEntity->removeComponent(customModelTextureMaterial);


                ui->le_ModelLocal->setText(dynamic_cast <TTS_Custom_Model*>(item)->getLocalModel());
                ui->le_ModelOnline->setText(dynamic_cast <TTS_Custom_Model*>(item)->getOnlineModel());

                if(!dynamic_cast <TTS_Custom_Model*>(item)->isModelMissing())
                {
                    customModelMesh->setSource(QUrl::fromLocalFile(dynamic_cast <TTS_Custom_Model*>(item)->getLocalModel()));
                }
                else
                {
                    customModelMesh->setSource(QUrl(QLatin1String("qrc:/empty.obj")));
                    if(dynamic_cast <TTS_Custom_Model*>(item)->getOnlineModel() == "")
                    {
                        ui->l_model->setVisible(false);
                        ui->le_ModelLocal->setVisible(false);
                        ui->le_ModelOnline->setVisible(false);
                        ui->l_ModelLocal->setVisible(false);
                        ui->l_ModelOnline->setVisible(false);
                    }
                    else {
                        ui->le_ModelLocal->setStyleSheet("QLineEdit { background: rgb(255, 0, 0); selection-background-color: rgb(255, 0, 0); }");
                    }
                }

                ui->le_DiffuseLocal->setText(dynamic_cast <TTS_Custom_Model*>(item)->getLocalTexture());
                ui->le_DiffuseOnline->setText(dynamic_cast <TTS_Custom_Model*>(item)->getOnlineTexture());
                if(!dynamic_cast <TTS_Custom_Model*>(item)->isTextureMissing())
                {
                    customModelTextureLoader->setSource(QUrl::fromLocalFile(dynamic_cast <TTS_Custom_Model*>(item)->getLocalTexture()));

                }
                else
                {
                    customModelTextureLoader->setSource(QUrl(QLatin1String("qrc:/empty.png")));

                    if(dynamic_cast <TTS_Custom_Model*>(item)->getOnlineTexture() == "")
                    {
                        ui->l_diffuse->setVisible(false);
                        ui->le_DiffuseLocal->setVisible(false);
                        ui->le_DiffuseOnline->setVisible(false);
                        ui->l_DiffuseLocal->setVisible(false);
                        ui->l_DiffuseOnline->setVisible(false);
                    }
                    else {

                        ui->le_DiffuseLocal->setStyleSheet("QLineEdit { background: rgb(255, 0, 0); selection-background-color: rgb(255, 0, 0); }");
                    }
                }

                ui->le_NormalLocal->setText(dynamic_cast <TTS_Custom_Model*>(item)->getLocalNormal());
                ui->le_NormalOnline->setText(dynamic_cast <TTS_Custom_Model*>(item)->getOnlineNormal());
                if(dynamic_cast <TTS_Custom_Model*>(item)->isNormalMissing())
                {
                    if(dynamic_cast <TTS_Custom_Model*>(item)->getOnlineNormal() == "")
                    {
                        ui->l_normal->setVisible(false);
                        ui->le_NormalLocal->setVisible(false);
                        ui->le_NormalOnline->setVisible(false);
                        ui->l_NormalLocal->setVisible(false);
                        ui->l_NormalOnline->setVisible(false);
                    }
                    else {

                        ui->le_NormalLocal->setStyleSheet("QLineEdit { background: rgb(255, 0, 0); selection-background-color: rgb(255, 0, 0); }");
                    }
                }

                ui->le_ColliderLocal->setText(dynamic_cast <TTS_Custom_Model*>(item)->getLocalCollider());
                ui->le_ColliderOnline->setText(dynamic_cast <TTS_Custom_Model*>(item)->getOnlineCollider());

                if(dynamic_cast <TTS_Custom_Model*>(item)->isColliderMissing())
                {
                    if(dynamic_cast <TTS_Custom_Model*>(item)->getOnlineCollider() == "")
                    {
                        ui->l_collider->setVisible(false);
                        ui->le_ColliderLocal->setVisible(false);
                        ui->le_ColliderOnline->setVisible(false);
                        ui->l_ColliderLocal->setVisible(false);
                        ui->l_ColliderOnline->setVisible(false);
                    }
                    else {

                        ui->le_ColliderLocal->setStyleSheet("QLineEdit { background: rgb(255, 0, 0); selection-background-color: rgb(255, 0, 0); }");
                    }
                }

                customModelTextureMaterial->setTexture(customModelTextureLoader);

                customModelEntity->addComponent(customModelMesh);
                customModelEntity->addComponent(customModelTextureMaterial); 
            }

            if(dynamic_cast <TTS_TreeWidgetItem*>(item)->isTexture())
            {
                ui->widgetTexture->setVisible(true);

                QString frontImgPath=dynamic_cast <TTS_AbstractCustomImage*>(item)->getLocalImageFront();
                QString backImgPath=dynamic_cast <TTS_AbstractCustomImage*>(item)->getLocalImageBack();

                ui->frontImgLabel->setVisible(true);
                if(dynamic_cast <TTS_AbstractCustomImage*>(item)->getOnlineImageFront() == "")
                {
                    ui->frontImgLabel->setVisible(false);
                }
                ui->le_ImgFrontLocal->setVisible(true);
                ui->le_ImgFrontOnline->setVisible(true);
                ui->l_imgLocalFront->setVisible(true);
                ui->l_imgOnlineFront->setVisible(true);


                ui->backImgLabel->setVisible(true);
                if(dynamic_cast <TTS_AbstractCustomImage*>(item)->getOnlineImageBack() == "")
                {
                    ui->backImgLabel->setVisible(false);
                }
                ui->le_ImgBackLocal->setVisible(true);
                ui->le_ImgBackOnline->setVisible(true);
                ui->l_imgLocalBack->setVisible(true);
                ui->l_imgOnlineBack->setVisible(true);


                ui->le_ImgBackLocal->setStyleSheet("QLineEdit { background: rgb(255, 255, 255); selection-background-color: rgb(255, 255, 255); }");

                ui->le_ImgFrontLocal->setText(frontImgPath);
                ui->le_ImgFrontOnline->setText(dynamic_cast <TTS_AbstractCustomImage*>(item)->getOnlineImageFront());
                if(!dynamic_cast <TTS_AbstractCustomImage*>(item)->isFrontMissing())
                {
                    if(dynamic_cast <TTS_AbstractCustomImage*>(item)->getOnlineImageFront() != "")
                    {
                        //QPixmap p(frontImgPath);
                        QImageReader reader(frontImgPath);
                        reader.setDecideFormatFromContent(true);
                        QPixmap p=QPixmap::fromImage(reader.read());

                        int wlabel = ui->frontImgLabel->width();
                        int hlabel = ui->frontImgLabel->height();

                        if(wlabel<p.width() || hlabel<p.height())
                        {
                            ui->frontImgLabel->setPixmap(p.scaled(wlabel-20,hlabel-20,Qt::KeepAspectRatio));
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
                        ui->l_imgLocalFront->setVisible(false);
                        ui->l_imgOnlineFront->setVisible(false);
                    }
                }
                else
                {
                    //ui->frontImgLabel->setVisible(false);
                    ui->le_ImgFrontLocal->setStyleSheet("QLineEdit { background: rgb(255, 0, 0); selection-background-color: rgb(255, 0, 0); }");
                }

                ui->le_ImgBackLocal->setStyleSheet("QLineEdit { background: rgb(255, 255, 255); selection-background-color: rgb(255, 255, 255); }");
                ui->le_ImgBackLocal->setText(backImgPath);
                ui->le_ImgBackOnline->setText(dynamic_cast <TTS_AbstractCustomImage*>(item)->getOnlineImageBack());
                if(!dynamic_cast <TTS_AbstractCustomImage*>(item)->isBackMissing())
                {
                    if(dynamic_cast <TTS_AbstractCustomImage*>(item)->getOnlineImageBack() != "")
                    {
                        //QPixmap p(backImgPath);
                        QImageReader reader(backImgPath);
                        reader.setDecideFormatFromContent(true);
                        QPixmap p=QPixmap::fromImage(reader.read());

                        int wlabel = ui->backImgLabel->width();
                        int hlabel = ui->backImgLabel->height();

                        if(wlabel<p.width() || hlabel<p.height())
                        {
                            ui->backImgLabel->setPixmap(p.scaled(wlabel-20,hlabel-20,Qt::KeepAspectRatio));
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
                        ui->l_imgLocalBack->setVisible(false);
                        ui->l_imgOnlineBack->setVisible(false);
                    }

                }
                else
                {
                    //ui->backImgLabel->setVisible(false);
                    ui->le_ImgBackLocal->setStyleSheet("QLineEdit { background: rgb(255, 0, 0); selection-background-color: rgb(255, 0, 0); }");
                }
            }
        }
    }
}
