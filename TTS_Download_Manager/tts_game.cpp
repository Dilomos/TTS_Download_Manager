#include "tts_game.h"
#include "tts_custom_model.h"
#include "tts_customimage.h"

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
            m_customModelParentTreeItem = new TTS_TreeWidgetItem(this,QStringList("Custom Model"));
            m_customImageParentTreeItem = new TTS_TreeWidgetItem(this,QStringList("Custom Images"));

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

            if(ObjectType == "Figurine_Custom")
            {
                figCount++;
            }
            else if(ObjectType == "Custom_Model")
            {
                modelCount++;
                TTS_Custom_Model *customModel = new TTS_Custom_Model(m_customModelParentTreeItem,object);
                if(!m_customModelMap.contains(customModel->getOnlineModel()))
                    m_customModelMap[customModel->getOnlineModel()]=customModel;
                else
                    delete customModel;
            }
            else if(ObjectType == "Bag")
            {
                bagCount++;
                if (object.contains("ContainedObjects") && object["ContainedObjects"].isArray())
                {
                    QJsonArray items=object["ContainedObjects"].toArray();

                    exploreContent(items);
                }
            }
            else if(ObjectType == "Infinite_Bag")
            {
                infBagCount++;
                if (object.contains("ContainedObjects") && object["ContainedObjects"].isArray())
                {
                    QJsonArray items=object["ContainedObjects"].toArray();

                    exploreContent(items);
                }
            }
            else if(ObjectType == "Quarter")
                quarterCount++;
            else if(ObjectType == "DeckCustom")
                deckCustomCount++;
            else if(ObjectType == "Deck")
               deckCount++;
            else if(ObjectType == "Custom_Board")
                boardCount++;
            else if(ObjectType == "Card")
                cardCount++;
            else if(ObjectType == "Custom_Tile")
            {
                tileCount++;
                TTS_CustomImage *customImage = new TTS_CustomImage(m_customImageParentTreeItem,object);
                if(!m_customImageMap.contains(customImage->getOnlineImageFront()))
                    m_customImageMap[customImage->getOnlineImageFront()]=customImage;
                else
                    delete customImage;
            }
            else if(ObjectType == "Custom_Token")
            {
                tokenCount++;

            }
            else
            {

               if(!m_unknownedTag.contains(ObjectType)){
                   m_unknownedTag[ObjectType]=ObjectType;
               }
            }
        }
    }
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
