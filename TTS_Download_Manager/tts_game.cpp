#include "tts_game.h"
#include "tts_custom_model.h"
#include "tts_customimage.h"
#include "tts_customdeck.h"
#include "tts_tableandsky.h"
#include "tts_customassetbundle.h"
#include "tts_customtablet.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QRandomGenerator>
#include <QTextStream>
#include <QJsonObject>
#include <QTextStream>

TTS_Game::TTS_Game(QString filename,int index)
    : m_jsonPath(filename),m_gameIndex(index)
{
    m_isLoaded=false;
    m_isSomethingMissing=false;
    m_filesCount = 0;
    m_missingFilesCount = 0;

    int lastPoint = m_jsonPath.lastIndexOf("/")+1;
    QString fileNameNoPath = m_jsonPath.right(m_jsonPath.length()-lastPoint);

    QListWidgetItem::setText(fileNameNoPath);
    QTreeWidgetItem::setText(0,fileNameNoPath);
}

void TTS_Game::loadGameFromFile(void)
{
    if(m_isLoaded == false)
    {
        m_isLoaded = true;
        QFile loadFile(m_jsonPath);

        if (!loadFile.open(QIODevice::ReadOnly)) {
            qWarning("Couldn't open save file.");
        }

        int lastPoint = m_jsonPath.lastIndexOf(".");
        QString fileNameNoExt = m_jsonPath.left(lastPoint);
        fileNameNoExt+=".png";
        m_iconePath = fileNameNoExt;

        QByteArray saveData = loadFile.readAll();
        loadFile.close();

        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

        QJsonObject json=loadDoc.object();


        if (json.contains("SaveName") && json["SaveName"].isString())
        {
            m_Name = json["SaveName"].toString();
            QListWidgetItem::setText(m_Name);
            QTreeWidgetItem::setText(0,m_Name);
        }
        m_customTableParentTreeItem = new TTS_TreeWidgetItem(this,QStringList("Table"));
        TTS_TableAndSky *customTableSky = new TTS_TableAndSky(m_customTableParentTreeItem,json);

        m_filesCount+=customTableSky->getFileCount();
        if(customTableSky->isSomethingMissing())
        {
            m_isSomethingMissing=true;
            m_missingFilesCount+=customTableSky->getMissingFileCount();
            QListWidgetItem::setBackground(QBrush(QColor(Qt::red),Qt::SolidPattern));
        }

        if (json.contains("ObjectStates") && json["ObjectStates"].isArray())
        {
            m_customModelParentTreeItem = new TTS_TreeWidgetItem(this,QStringList("Models"));
            m_customTokenParentTreeItem = new TTS_TreeWidgetItem(this,QStringList("Tokens"));
            m_customTileParentTreeItem = new TTS_TreeWidgetItem(this,QStringList("Tiles"));
            m_customCardParentTreeItem = new TTS_TreeWidgetItem(this,QStringList("Cards"));
            m_customFigurineParentTreeItem = new TTS_TreeWidgetItem(this,QStringList("Figurines"));
            m_customBoardParentTreeItem = new TTS_TreeWidgetItem(this,QStringList("Boards"));
            m_customDiceParentTreeItem = new TTS_TreeWidgetItem(this,QStringList("Dices"));
            m_customAssetbundleParentTreeItem = new TTS_TreeWidgetItem(this,QStringList("Assetbundle"));
            m_customTabletParentTreeItem = new TTS_TreeWidgetItem(this,QStringList("Tablet"));

            QJsonArray objects=json["ObjectStates"].toArray();

            exploreContent(objects);
        }

        /*foreach (QString value, m_unknownedTag)
            QTextStream(stdout) <<value<<endl;*/
    }
}

void TTS_Game::exploreContent(QJsonArray objects)
{
    for (int index = 0; index < objects.size(); ++index) {
        QJsonObject object = objects[index].toObject();
        loadObject(object);
    }
}

void TTS_Game::loadObject(QJsonObject object,TTS_TreeWidgetItem *parent)
{
    if (object.contains("Name"))
    {
        QString ObjectType=object["Name"].toString();

        TTS_AbstractFile*file=nullptr;

        if( (ObjectType == "Custom_Model")|| (ObjectType == "Custom_Model_Stack") )
        {
            m_modelCount++;
            if(parent == nullptr)
                parent=m_customModelParentTreeItem;
            TTS_Custom_Model *customModel = new TTS_Custom_Model(parent,object);
            if(m_filesMap.contains(customModel->getId()) || (customModel->getId()==""))
            {
                delete customModel;
            }
            else {
                file=customModel;
            }

        }
        else if( (ObjectType == "Custom_Model_Bag") || (ObjectType == "Custom_Model_Infinite_Bag")
                 || (ObjectType == "Bag") || (ObjectType == "Infinite_Bag")
                 || (ObjectType == "Custom_Assetbundle_Infinite_Bag") || (ObjectType == "Custom_Assetbundle_Bag") )
        {
            m_bagCount++;
            if( object.contains("CustomMesh") )
            {
                if(parent == nullptr)
                    parent=m_customModelParentTreeItem;
                TTS_Custom_Model *customModel = new TTS_Custom_Model(parent,object);
                if(m_filesMap.contains(customModel->getId()) || (customModel->getId()==""))
                {
                    delete customModel;
                }
                else {
                    file=customModel;
                }
            }

            if (object.contains("ContainedObjects") && object["ContainedObjects"].isArray())
            {
                QJsonArray items=object["ContainedObjects"].toArray();

                exploreContent(items);
            }
        }
        else if(ObjectType == "Figurine_Custom")
        {
            m_figCount++;
            if(parent == nullptr)
                parent=m_customFigurineParentTreeItem;
            TTS_CustomImage *customImage = new TTS_CustomImage(parent,object);
            if(m_filesMap.contains(customImage->getId()) || (customImage->getId()==""))
            {
                delete customImage;
            }
            else {
                file=customImage;
            }
        }
        else if( (ObjectType == "Deck") || (ObjectType == "DeckCustom") || (ObjectType == "Card"))
        {
           m_deckCount++;
           if(parent == nullptr)
               parent=m_customCardParentTreeItem;
           TTS_CustomDeck *customImage = new TTS_CustomDeck(parent,object);
           if(m_filesMap.contains(customImage->getId()) || (customImage->getId()==""))
           {
               delete customImage;
           }
           else {
               file=customImage;
           }
        }
        else if( (ObjectType == "Custom_Tile") || (ObjectType == "Custom_Tile_Stack"))
        {
            m_tileCount++;
            if(parent == nullptr)
                parent=m_customTileParentTreeItem;
            TTS_CustomImage *customImage = new TTS_CustomImage(parent,object);
            if(m_filesMap.contains(customImage->getId()) || (customImage->getId()==""))
            {
                delete customImage;
            }
            else {
                file=customImage;
            }
        }
        else if( (ObjectType == "Custom_Token") || (ObjectType == "Custom_Token_Stack") )
        {
            m_tokenCount++;
            if(parent == nullptr)
                parent=m_customTokenParentTreeItem;
            TTS_CustomImage *customImage = new TTS_CustomImage(parent,object);
            if(m_filesMap.contains(customImage->getId()) || (customImage->getId()==""))
            {
                delete customImage;
            }
            else {
                file=customImage;
            }
        }
        else if(ObjectType == "Custom_Board")
        {
            m_boardCount++;
            if(parent == nullptr)
                parent=m_customBoardParentTreeItem;
            TTS_CustomImage *customImage = new TTS_CustomImage(parent,object);
            if(m_filesMap.contains(customImage->getId()) || (customImage->getId()==""))
            {
                delete customImage;
            }
            else {
                file=customImage;
            }
        }
        else if(ObjectType == "Custom_Dice")
        {
            m_diceCount++;
            if(parent == nullptr)
                parent=m_customDiceParentTreeItem;
            TTS_CustomImage *customImage = new TTS_CustomImage(parent,object);
            if(m_filesMap.contains(customImage->getId()) || (customImage->getId()==""))
            {
                delete customImage;
            }
            else {
                file=customImage;
            }
        }
        else if(ObjectType == "Custom_Assetbundle")
        {
            if(parent == nullptr)
                parent=m_customAssetbundleParentTreeItem;
            TTS_CustomAssetbundle *customAsset = new TTS_CustomAssetbundle(parent,object);
            if(m_filesMap.contains(customAsset->getId()) || (customAsset->getId()==""))
            {
                delete customAsset;
            }
            else {
                file=customAsset;
            }
        }
        else if(ObjectType == "Tablet")
        {
            if(parent == nullptr)
                parent=m_customTabletParentTreeItem;
            TTS_CustomTablet *customTablet = new TTS_CustomTablet(parent,object);
            if(m_filesMap.contains(customTablet->getId()) || (customTablet->getId()==""))
            {
                delete customTablet;
            }
            else {
                file=customTablet;
            }
        }
        else if( (ObjectType == "Quarter")|| (ObjectType == "PlayerPawn")|| (ObjectType == "Bowl")|| (ObjectType == "PiecePack_Suns")|| (ObjectType == "go_game_piece_white")|| (ObjectType == "Die_4")|| (ObjectType == "Die_10")|| (ObjectType == "Die_12")|| (ObjectType == "Die_8") || (ObjectType == "Digital_Clock")  || (ObjectType == "BlockRectangle") || (ObjectType == "ScriptingTrigger") || (ObjectType == "FogOfWarTrigger") || (ObjectType == "RandomizeTrigger") || (ObjectType == "Die_20")|| (ObjectType == "Die_6")|| (ObjectType == "3DText") || (ObjectType == "Die_6_Rounded") || (ObjectType == "Tileset_Chest") || (ObjectType == "Notecard") || (ObjectType == "Counter") || (ObjectType == "Die_6_Rounded") )
            m_otherCount++;
        /*else
        {
            if( object.contains("CustomMesh") )
            {
                m_modelCount++;
                TTS_Custom_Model *customModel = new TTS_Custom_Model(m_customModelParentTreeItem,object);
                if(m_filesMap.contains(customModel->getId()))
                {
                    delete customModel;
                }
                else {
                    file=customModel;
                }

            }

            if( object.contains("ContainedObjects") )
            {
                m_bagCount++;
                if (object.contains("ContainedObjects") && object["ContainedObjects"].isArray())
                {
                    QJsonArray items=object["ContainedObjects"].toArray();

                    exploreContent(items);
                }
            }

            if( object.contains("CustomImage")
             || object.contains("CustomDeck")  )
                m_otherCount++;
           if(!m_unknownedTag.contains(ObjectType)){
               m_unknownedTag[ObjectType]=ObjectType;
           }
        }*/

        if(object.contains("States"))
        {
            //QTextStream(stdout) << "Multiple States for: "<<ObjectType <<endl;

            QJsonObject states = object["States"].toObject();

            foreach(const QString& key, states.keys()) {
                    QJsonObject state = states[key].toObject();
                    loadObject(state,dynamic_cast <TTS_TreeWidgetItem*>(file));
                }
        }

        if(file != nullptr)
        {
            m_filesMap[file->getId()]=file;
            m_filesCount+=file->getFileCount();
            if(file->isSomethingMissing())
            {
                m_isSomethingMissing=true;
                m_missingFilesCount+=file->getMissingFileCount();
                QListWidgetItem::setBackground(QBrush(QColor(Qt::red),Qt::SolidPattern));
            }
        }
    }
}

bool TTS_Game::isSomethingMissing(void)
{
   return m_isSomethingMissing;
}

QString TTS_Game::getPixPath(void)
{
    return m_iconePath;
}

QString TTS_Game::getFileName(void)
{
    return m_jsonPath;
}

int TTS_Game::getIndex(void)
{
    return m_gameIndex;
}

TTS_TreeWidgetItem *TTS_Game::getCustomModelTreeItem(void)
{
   return m_customModelParentTreeItem;
}

int TTS_Game::getFileCount(void)
{
    return m_filesCount;
}

int TTS_Game::getMissingFileCount(void)
{
    return m_missingFilesCount;
}
