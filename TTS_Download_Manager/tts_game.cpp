#include "tts_game.h"
#include "tts_custom_model.h"
#include "tts_customimage.h"
#include "tts_customdeck.h"

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

        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

        QJsonObject json=loadDoc.object();
        QString mName;

        if (json.contains("SaveName") && json["SaveName"].isString())
        {
            mName = json["SaveName"].toString();
            QListWidgetItem::setText(mName);
            QTreeWidgetItem::setText(0,mName);
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

        if (object.contains("Name"))
        {
            QString ObjectType=object["Name"].toString();

            TTS_AbstractFile*file=nullptr;
            if( (ObjectType == "Custom_Model")|| (ObjectType == "Custom_Model_Stack") )
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
            else if( (ObjectType == "Custom_Model_Bag") || (ObjectType == "Custom_Model_Infinite_Bag") || (ObjectType == "Bag") || (ObjectType == "Infinite_Bag") || (ObjectType == "Custom_Assetbundle_Infinite_Bag") || (ObjectType == "Custom_Assetbundle_Bag") )
            {
                m_bagCount++;
                if( object.contains("CustomMesh") )
                {
                    TTS_Custom_Model *customModel = new TTS_Custom_Model(m_customModelParentTreeItem,object);
                    if(m_filesMap.contains(customModel->getId()))
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
                TTS_CustomImage *customImage = new TTS_CustomImage(m_customFigurineParentTreeItem,object);
                if(m_filesMap.contains(customImage->getId()))
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
               TTS_CustomDeck *customImage = new TTS_CustomDeck(m_customCardParentTreeItem,object);
               if(m_filesMap.contains(customImage->getId()))
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
                TTS_CustomImage *customImage = new TTS_CustomImage(m_customTileParentTreeItem,object);
                if(m_filesMap.contains(customImage->getId()))
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
                TTS_CustomImage *customImage = new TTS_CustomImage(m_customTokenParentTreeItem,object);
                if(m_filesMap.contains(customImage->getId()))
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
                TTS_CustomImage *customImage = new TTS_CustomImage(m_customBoardParentTreeItem,object);
                if(m_filesMap.contains(customImage->getId()))
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
                TTS_CustomImage *customImage = new TTS_CustomImage(m_customDiceParentTreeItem,object);
                if(m_filesMap.contains(customImage->getId()))
                {
                    delete customImage;
                }
                else {
                    file=customImage;
                }
            }
            else if( (ObjectType == "Custom_Assetbundle")|| (ObjectType == "Tablet"))
            {

            }
            else if( (ObjectType == "Quarter")|| (ObjectType == "PlayerPawn")|| (ObjectType == "Bowl")|| (ObjectType == "PiecePack_Suns")|| (ObjectType == "go_game_piece_white")|| (ObjectType == "Die_4")|| (ObjectType == "Die_10")|| (ObjectType == "Die_12")|| (ObjectType == "Die_8") || (ObjectType == "Digital_Clock")  || (ObjectType == "BlockRectangle") || (ObjectType == "ScriptingTrigger") || (ObjectType == "FogOfWarTrigger") || (ObjectType == "RandomizeTrigger") || (ObjectType == "Die_20")|| (ObjectType == "Die_6")|| (ObjectType == "3DText") || (ObjectType == "Die_6_Rounded") || (ObjectType == "Tileset_Chest") || (ObjectType == "Notecard") || (ObjectType == "Counter") || (ObjectType == "Die_6_Rounded") )
                m_otherCount++;
            else
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
