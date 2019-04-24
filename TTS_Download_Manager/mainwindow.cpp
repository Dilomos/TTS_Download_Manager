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
    //modPath="C:/Users/E9868092/Downloads/perso/TTS/Mods";
    modPath="G:/Jeux/SteamLibrary/SteamApps/common/Tabletop Simulator/Tabletop Simulator_Data/Mods";

    QString workshopPath=modPath+"\\Workshop";
    QString modelPath=modPath+"\\Models";
    QString imgPath=modPath+"\\Images";

    QStringList listFilter;
    listFilter << "*.json";

    QDirIterator dirIterator(workshopPath, listFilter ,QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);

    // Variable qui contiendra tous les fichiers correspondant à notre recherche
    QStringList fileList;
    // Tant qu'on n'est pas arrivé à la fin de l'arborescence...
    while(dirIterator.hasNext())
    {
        // ...on va au prochain fichier correspondant à notre filtre
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


    ui->progressBar->setVisible(false);

    //connect(ui->listWidget, &QListWidget::itemClicked, this,&MainWindow::gameClicked);
    //connect(ui->treeWidget, &QTreeWidget::itemClicked, this,&MainWindow::itemClicked);
    connect(ui->listWidget, &QListWidget::currentItemChanged, this,&MainWindow::gameClicked);
    connect(ui->treeWidget, &QTreeWidget::currentItemChanged, this,&MainWindow::itemClicked);
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
    ui->widgetTexture->setVisible(false);
    ui->widget3D->setVisible(false);
    if(dynamic_cast <TTS_TreeWidgetItem*>(item)->isDisplayable())
    {
        if(dynamic_cast <TTS_TreeWidgetItem*>(item)->is3DModel())
        {
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

            ui->widget3D->setVisible(true);
        }
    }

}
