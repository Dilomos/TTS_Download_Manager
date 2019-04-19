#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tts_game.h"

#include <QTextStream>
#include <QDirIterator>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //QString folder(QStringLiteral("E:\\Coding\\TTSDownloadManager"));
    //QString modPath(QStringLiteral("E:\\Coding\\TTSDownloadManager\\Mods"));
    QString modPath(QStringLiteral("C:\\Users\\E9868092\\Downloads\\perso\\TTS\\Mods"));
    //QString folder(QStringLiteral("G:\\Jeux\\SteamLibrary\\SteamApps\\common\\Tabletop Simulator\\Tabletop Simulator_Data\\Mods\\Workshop"));


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

    for(int i=0 ; (i < gameCount) /*&& (i < 10)*/ ; i++)
    {
        TTS_Game *game=new TTS_Game(fileList.at(i),i+1);
        ui->listWidget->addItem(game);
        //QTextStream(stdout) << fileList.at(i)<<endl;
    }

    ui->lGameCount->setText("0/"+QString::number(gameCount));

    connect(ui->listWidget, &QListWidget::itemClicked, this,&MainWindow::gameClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::gameClicked(QListWidgetItem * item)
{
    ui->label->setPixmap(QPixmap(dynamic_cast <TTS_Game*>(item)->getPixPath()));
    ui->statusBar->showMessage(dynamic_cast <TTS_Game*>(item)->getFileName());
    ui->lGameCount->setText(QString::number(dynamic_cast <TTS_Game*>(item)->getIndex())+"/"+QString::number(gameCount));

    ui->treeWidget->takeTopLevelItem(0);
    ui->treeWidget->addTopLevelItem(dynamic_cast <QTreeWidgetItem*>(item));
    ui->treeWidget->header()->setResizeContentsPrecision(+);
}
