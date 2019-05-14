#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tts_game.h"
#include "tools.h"
#include "orbittransformcontroller.h"
#include "tts_custom_model.h"
#include "tts_customimage.h"
#include "tts_customassetbundle.h"
#include "tts_customtablet.h"

#include <QTextStream>
#include <QDirIterator>
#include <QVector>
#include <QFileDialog>

QString MainWindow::modPath="";
QString MainWindow::exportPath="";

GameLoaderThread::GameLoaderThread(QListWidget* m_listToLoad,int id,int startIndex,int lastIndex)
    :m_listToLoad(m_listToLoad),m_id(id),m_startIndex(startIndex),m_lastIndex(lastIndex)
{}

void GameLoaderThread::run() {
    for (int i = m_startIndex; i < m_lastIndex; i++) {
        emit readingGameNumber(i-m_startIndex,m_id);
        dynamic_cast <TTS_Game*>(m_listToLoad->item(i))->loadGameFromFile();
        }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->mainToolBar->setVisible(false);

    getParameterFromFile();

    ui->progressBar->setVisible(false);
    ui->widgetTexture->setVisible(false);

    connect(ui->tb_gameFile, &QToolButton::pressed, this, &MainWindow::openFileExternally);

    connect(ui->tb_localModel, &QToolButton::pressed, this, &MainWindow::openFileExternally);
    connect(ui->tb_onlineModel, &QToolButton::pressed, this, &MainWindow::openFileExternally);
    connect(ui->tb_localDiffuse, &QToolButton::pressed, this, &MainWindow::openFileExternally);
    connect(ui->tb_onlineDiffuse, &QToolButton::pressed, this, &MainWindow::openFileExternally);
    connect(ui->tb_localNormal, &QToolButton::pressed, this, &MainWindow::openFileExternally);
    connect(ui->tb_onlineNormal, &QToolButton::pressed, this, &MainWindow::openFileExternally);
    connect(ui->tb_localCollider, &QToolButton::pressed, this, &MainWindow::openFileExternally);
    connect(ui->tb_onlineCollider, &QToolButton::pressed, this, &MainWindow::openFileExternally);

    connect(ui->tb_localFrontImg, &QToolButton::pressed, this, &MainWindow::openFileExternally);
    connect(ui->tb_onlineFrontImg, &QToolButton::pressed, this, &MainWindow::openFileExternally);
    connect(ui->tb_localBackImg, &QToolButton::pressed, this, &MainWindow::openFileExternally);
    connect(ui->tb_onlineBackImg, &QToolButton::pressed, this, &MainWindow::openFileExternally);

    connect(ui->tb_AssetLocal, &QToolButton::pressed, this, &MainWindow::openFileExternally);
    connect(ui->tb_AssetOnline, &QToolButton::pressed, this, &MainWindow::openFileExternally);
    connect(ui->tb_AssetSecLocal, &QToolButton::pressed, this, &MainWindow::openFileExternally);
    connect(ui->tb_AssetSecOnline, &QToolButton::pressed, this, &MainWindow::openFileExternally);

    connect(ui->actionSet_Mods_Folder, &QAction::triggered, this, &MainWindow::setModsFolder);
    connect(ui->actionSet_Export_Folder, &QAction::triggered, this, &MainWindow::setExportFolder);

    init3DView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getParameterFromFile()
{
    QFile loadFile(/*QDir::currentPath()+*/"TTS_Download_Manager.json");
    QFileInfo check_file(loadFile);
    if(check_file.exists())
    {
        if (!loadFile.open(QIODevice::ReadOnly)) {
            qWarning("Couldn't open save file.");
        }

        QByteArray saveData = loadFile.readAll();
        loadFile.close();

        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

        QJsonObject json=loadDoc.object();

        modPath=json["ModsFolder"].toString();
        exportPath=json["ExportFolder"].toString();

    }
    else
    {

        //modPath="E:/Coding/github/TTS_Download_Manager/Mods";
        modPath="G:/Jeux/SteamLibrary/SteamApps/common/Tabletop Simulator/Tabletop Simulator_Data/Mods";
        //modPath="C:/Users/E9868092/Downloads/perso/TTS/Mods";

        exportPath="E:/TTS_export";

        saveParametersToFile();
    }
}

void MainWindow::saveParametersToFile(void)
{
    QFile loadFile(/*QDir::currentPath()+*/"TTS_Download_Manager.json");
    QJsonObject recordObject;
    recordObject.insert("ModsFolder", QJsonValue::fromVariant(modPath));
    recordObject.insert("ExportFolder", QJsonValue::fromVariant(exportPath));

    QJsonDocument doc(recordObject);

    if (!loadFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
    }

    loadFile.write(doc.toJson());
    loadFile.close();
}

void MainWindow::setModsFolder(bool checked)
{
    QString modFolder = QFileDialog::getExistingDirectory(0, ("Select TTS Mods Folder"), modPath);

    if(modFolder != "")
    {
        modPath = modFolder;
        saveParametersToFile();
    }
}

void MainWindow::setExportFolder(bool checked)
{
    QString exportFolder = QFileDialog::getExistingDirectory(0, ("Select TTS Mods Folder"), exportPath);

    if(exportFolder != "")
    {
        exportPath = exportFolder;
        saveParametersToFile();
    }
}

void MainWindow::searchGames(void)
{
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

    m_threadVal.resize(4);
    m_threadFinishedCount=0;

    GameLoaderThread *gameLoaderThread  = new GameLoaderThread(ui->listWidget,0,0,ui->listWidget->count()/3);
    GameLoaderThread *gameLoaderThread2 = new GameLoaderThread(ui->listWidget,1,ui->listWidget->count()/3,(ui->listWidget->count()/3)*2);
    GameLoaderThread *gameLoaderThread3 = new GameLoaderThread(ui->listWidget,2,(ui->listWidget->count()/3)*2,ui->listWidget->count());

    connect(gameLoaderThread, &GameLoaderThread::readingGameNumber, this, &MainWindow::updateProgressBar);
    connect(gameLoaderThread2, &GameLoaderThread::readingGameNumber, this, &MainWindow::updateProgressBar);
    connect(gameLoaderThread3, &GameLoaderThread::readingGameNumber, this, &MainWindow::updateProgressBar);

    connect(gameLoaderThread, &GameLoaderThread::finished, this, &MainWindow::hideProgressBar);
    connect(gameLoaderThread2, &GameLoaderThread::finished, this, &MainWindow::hideProgressBar);
    connect(gameLoaderThread3, &GameLoaderThread::finished, this, &MainWindow::hideProgressBar);

    connect(gameLoaderThread, &GameLoaderThread::finished, gameLoaderThread, &QObject::deleteLater);
    connect(gameLoaderThread2, &GameLoaderThread::finished, gameLoaderThread2, &QObject::deleteLater);
    connect(gameLoaderThread3, &GameLoaderThread::finished, gameLoaderThread3, &QObject::deleteLater);

    gameLoaderThread->start();
    gameLoaderThread2->start();
    gameLoaderThread3->start();

    ui->widgetTexture->setVisible(false);
    ui->widget3D->setVisible(false);
    ui->widgetAsset->setVisible(false);
}

void MainWindow::updateProgressBar(const int &val,int threadID)
{
    m_threadVal[threadID]=val;

    int tmp_val=0;
    for(int i=0;i<m_threadVal.size();i++)
        tmp_val += m_threadVal.at(i);

    ui->progressBar->setValue(tmp_val);
}

void MainWindow::hideProgressBar(void)
{
    m_threadFinishedCount++;

    if(m_threadFinishedCount == 3)
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
    container->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //QHBoxLayout *hLayout = new QHBoxLayout(ui->widget3D);
    //QVBoxLayout *vLayout = new QVBoxLayout(ui->widget3D);
    ui->horizontalLayout_42->setAlignment(Qt::AlignTop);
    ui->horizontalLayout_42->insertWidget(0,container, 1);
    //hLayout->addLayout(vLayout);

    Qt3DInput::QInputAspect *input = new Qt3DInput::QInputAspect;
    view->registerAspect(input);

    rootEntity = new Qt3DCore::QEntity;

    Qt3DRender::QCamera *camera = view->camera();
    camera->lens()->setPerspectiveProjection(40.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(0, 8.0f, 15.0f));
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

void MainWindow::openFileExternally(void)
{
      // e.g. casting to the class you know its connected with
      QToolButton* button = qobject_cast<QToolButton*>(sender());
      if( button == ui->tb_gameFile )
      {
         QDesktopServices::openUrl(QUrl::fromLocalFile(ui->le_gameName->text()));
      }else if( button == ui->tb_localModel )
      {
         QDesktopServices::openUrl(QUrl::fromLocalFile(ui->le_ModelLocal->text()));
      }else if( button == ui->tb_onlineModel )
      {
         QDesktopServices::openUrl(QUrl(ui->le_ModelOnline->text()));
      }else if( button == ui->tb_localDiffuse )
      {
         QDesktopServices::openUrl(QUrl::fromLocalFile(ui->le_DiffuseLocal->text()));
      }else  if( button == ui->tb_onlineDiffuse )
      {
         QDesktopServices::openUrl(QUrl(ui->le_DiffuseOnline->text()));
      }else if( button == ui->tb_localNormal )
      {
         QDesktopServices::openUrl(QUrl::fromLocalFile(ui->le_NormalLocal->text()));
      }else if( button == ui->tb_onlineNormal )
      {
         QDesktopServices::openUrl(QUrl(ui->le_NormalOnline->text()));
      }else if( button == ui->tb_localCollider )
      {
         QDesktopServices::openUrl(QUrl::fromLocalFile(ui->le_ColliderLocal->text()));
      }else if( button == ui->tb_onlineCollider )
      {
         QDesktopServices::openUrl(QUrl(ui->le_ColliderOnline->text()));
      }else if( button == ui->tb_localFrontImg )
      {
         QDesktopServices::openUrl(QUrl::fromLocalFile(ui->le_ImgFrontLocal->text()));
      }else if( button == ui->tb_onlineFrontImg )
      {
         QDesktopServices::openUrl(QUrl(ui->le_ImgFrontOnline->text()));
      }else  if( button == ui->tb_localBackImg )
      {
         QDesktopServices::openUrl(QUrl::fromLocalFile(ui->le_ImgBackLocal->text()));
      }else if( button == ui->tb_onlineBackImg )
      {
         QDesktopServices::openUrl(QUrl(ui->le_ImgBackOnline->text()));
      }else if( button == ui->tb_AssetLocal )
      {
         QDesktopServices::openUrl(QUrl::fromLocalFile(ui->le_AssetLocal->text()));
      }else if( button == ui->tb_AssetOnline )
      {
         QDesktopServices::openUrl(QUrl(ui->le_AssetOnline->text()));
      }else  if( button == ui->tb_AssetSecLocal )
      {
         QDesktopServices::openUrl(QUrl::fromLocalFile(ui->le_AssetSecLocal->text()));
      }else if( button == ui->tb_AssetSecOnline )
      {
         QDesktopServices::openUrl(QUrl(ui->le_AssetSecOnline->text()));
      }
}

void MainWindow::gameClicked(QListWidgetItem * item,QListWidgetItem * prevItem)
{
    if(item != nullptr)
    {
        ui->widgetTexture->setVisible(false);
        ui->widget3D->setVisible(false);

        dynamic_cast <TTS_Game*>(item)->loadGameFromFile();

        QString fileCountLabelTmp=QString::number(dynamic_cast <TTS_Game*>(item)->getFileCount())+" elements";
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
        ui->widgetAsset->setVisible(false);
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

                ui->le_ModelLocal->setStyleSheet("QLineEdit { background: rgb(255, 255, 255); selection-background-color: rgb(255, 255, 255); }");
                ui->le_NormalLocal->setStyleSheet("QLineEdit { background: rgb(255, 255, 255); selection-background-color: rgb(255, 255, 255); }");
                ui->le_DiffuseLocal->setStyleSheet("QLineEdit { background: rgb(255, 255, 255); selection-background-color: rgb(255, 255, 255); }");
                ui->le_ColliderLocal->setStyleSheet("QLineEdit { background: rgb(255, 255, 255); selection-background-color: rgb(255, 255, 255); }");


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
                ui->l_texture->setPixmap(QPixmap());
                if(!dynamic_cast <TTS_Custom_Model*>(item)->isTextureMissing())
                {
                    customModelTextureLoader->setSource(QUrl::fromLocalFile(dynamic_cast <TTS_Custom_Model*>(item)->getLocalTexture()));

                    QImageReader reader(dynamic_cast <TTS_Custom_Model*>(item)->getLocalTexture());
                    reader.setDecideFormatFromContent(true);
                    QPixmap p=QPixmap::fromImage(reader.read());

                    int wlabel = ui->l_texture->width();
                    int hlabel = ui->l_texture->height();

                    if(wlabel<p.width() || hlabel<p.height())
                    {
                        ui->l_texture->setPixmap(p.scaled(wlabel-20,hlabel-20,Qt::KeepAspectRatio));
                    }
                    else
                    {
                        ui->l_texture->setPixmap(p);
                    }
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
                ui->frontImgLabel->setPixmap(QPixmap("qrc:/empty.png"));
                if(dynamic_cast <TTS_AbstractCustomImage*>(item)->getOnlineImageFront() == "")
                {
                    ui->frontImgLabel->setVisible(false);
                }
                ui->le_ImgFrontLocal->setVisible(true);
                ui->le_ImgFrontOnline->setVisible(true);
                ui->l_imgLocalFront->setVisible(true);
                ui->l_imgOnlineFront->setVisible(true);
                ui->tb_localFrontImg->setVisible(true);
                ui->tb_onlineFrontImg->setVisible(true);


                ui->backImgLabel->setVisible(true);
                ui->backImgLabel->setPixmap(QPixmap("qrc:/empty.png"));
                if(dynamic_cast <TTS_AbstractCustomImage*>(item)->getOnlineImageBack() == "")
                {
                    ui->backImgLabel->setVisible(false);
                }
                ui->le_ImgBackLocal->setVisible(true);
                ui->le_ImgBackOnline->setVisible(true);
                ui->l_imgLocalBack->setVisible(true);
                ui->l_imgOnlineBack->setVisible(true);
                ui->tb_localBackImg->setVisible(true);
                ui->tb_onlineBackImg->setVisible(true);


                ui->le_ImgFrontLocal->setStyleSheet("QLineEdit { background: rgb(255, 255, 255); selection-background-color: rgb(255, 255, 255); }");

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
                        ui->tb_localFrontImg->setVisible(false);
                        ui->tb_onlineFrontImg->setVisible(false);
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
                        ui->tb_localBackImg->setVisible(false);
                        ui->tb_onlineBackImg->setVisible(false);
                    }

                }
                else
                {
                    //ui->backImgLabel->setVisible(false);
                    ui->le_ImgBackLocal->setStyleSheet("QLineEdit { background: rgb(255, 0, 0); selection-background-color: rgb(255, 0, 0); }");
                }
            }

            if(dynamic_cast <TTS_TreeWidgetItem*>(item)->isAssetbundle())
            {
                ui->widgetAsset->setVisible(true);

                QString assetPath=dynamic_cast <TTS_CustomAssetbundle*>(item)->getLocalAsset();
                QString assetSecPath=dynamic_cast <TTS_CustomAssetbundle*>(item)->getLocalAssetSecondary();

                ui->l_AssetLocal->setVisible(true);
                ui->le_AssetLocal->setVisible(true);
                ui->l_AssetOnline->setVisible(true);
                ui->le_AssetOnline->setVisible(true);
                ui->tb_AssetLocal->setVisible(true);
                ui->tb_AssetOnline->setVisible(true);

                ui->l_AssetSecLocal->setVisible(true);
                ui->le_AssetSecLocal->setVisible(true);
                ui->l_AssetSecOnline->setVisible(true);
                ui->le_AssetSecOnline->setVisible(true);
                ui->tb_AssetSecLocal->setVisible(true);
                ui->tb_AssetSecOnline->setVisible(true);


                ui->le_AssetLocal->setStyleSheet("QLineEdit { background: rgb(255, 255, 255); selection-background-color: rgb(255, 255, 255); }");
                ui->le_AssetLocal->setText(assetPath);
                ui->le_AssetOnline->setText(dynamic_cast <TTS_CustomAssetbundle*>(item)->getOnlineAsset());
                if(!dynamic_cast <TTS_CustomAssetbundle*>(item)->isAssetMissing())
                {
                    if(dynamic_cast <TTS_CustomAssetbundle*>(item)->getOnlineAsset() == "")
                    {
                        ui->l_AssetLocal->setVisible(false);
                        ui->le_AssetLocal->setVisible(false);
                        ui->l_AssetOnline->setVisible(false);
                        ui->le_AssetOnline->setVisible(false);
                        ui->tb_AssetLocal->setVisible(false);
                        ui->tb_AssetOnline->setVisible(false);
                    }
                }
                else
                {
                    ui->le_AssetLocal->setStyleSheet("QLineEdit { background: rgb(255, 0, 0); selection-background-color: rgb(255, 0, 0); }");
                }

                ui->le_AssetSecLocal->setStyleSheet("QLineEdit { background: rgb(255, 255, 255); selection-background-color: rgb(255, 255, 255); }");
                ui->le_AssetSecLocal->setText(assetPath);
                ui->le_AssetSecOnline->setText(dynamic_cast <TTS_CustomAssetbundle*>(item)->getOnlineAssetSecondary());
                if(!dynamic_cast <TTS_CustomAssetbundle*>(item)->isAssetSecondaryMissing())
                {
                    if(dynamic_cast <TTS_CustomAssetbundle*>(item)->getOnlineAssetSecondary() == "")
                    {
                        ui->l_AssetSecLocal->setVisible(false);
                        ui->le_AssetSecLocal->setVisible(false);
                        ui->l_AssetSecOnline->setVisible(false);
                        ui->le_AssetSecOnline->setVisible(false);
                        ui->tb_AssetSecLocal->setVisible(false);
                        ui->tb_AssetSecOnline->setVisible(false);
                    }
                }
                else
                {
                    ui->le_AssetSecLocal->setStyleSheet("QLineEdit { background: rgb(255, 0, 0); selection-background-color: rgb(255, 0, 0); }");
                }
            }

            if(dynamic_cast <TTS_TreeWidgetItem*>(item)->isTablet())
            {
                ui->widgetAsset->setVisible(true);

                QString assetPath=dynamic_cast <TTS_CustomTablet*>(item)->getUrl();

                ui->l_AssetLocal->setVisible(false);
                ui->le_AssetLocal->setVisible(false);
                ui->l_AssetOnline->setVisible(true);
                ui->le_AssetOnline->setVisible(true);
                ui->tb_AssetLocal->setVisible(false);
                ui->tb_AssetOnline->setVisible(true);

                ui->l_AssetSecLocal->setVisible(false);
                ui->le_AssetSecLocal->setVisible(false);
                ui->l_AssetSecOnline->setVisible(false);
                ui->le_AssetSecOnline->setVisible(false);
                ui->tb_AssetSecLocal->setVisible(false);
                ui->tb_AssetSecOnline->setVisible(false);

                ui->le_AssetOnline->setText(assetPath);
            }
        }
    }
}
