#include "tts_game.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QRandomGenerator>
#include <QTextStream>
#include <QJsonObject>

TTS_Game::TTS_Game(QString filename,int index)
    : m_jsonPath(filename),m_gameIndex(index)
{
    QFile loadFile(filename);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
    }

    int lastPoint = filename.lastIndexOf(".");
    QString fileNameNoExt = filename.left(lastPoint);
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

        QJsonArray objects=json["ObjectStates"].toArray();

        exploreContent(objects);
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
                figCount++;

            if(ObjectType == "Custom_Model")
            {
                modelCount++;
                TTS_Custom_Model *customModel = new TTS_Custom_Model(m_customModelParentTreeItem,object);
                if(!m_customModelMap.contains(customModel->getOnlineModel()))
                    m_customModelMap[customModel->getOnlineModel()]=customModel;
                else
                    delete customModel;
            }

            if(ObjectType == "Bag")
            {
                bagCount++;
                if (object.contains("ContainedObjects") && object["ContainedObjects"].isArray())
                {
                    QJsonArray items=object["ContainedObjects"].toArray();

                    exploreContent(items);
                }
            }

            if(ObjectType == "Infinite_Bag")
            {
                infBagCount++;
                if (object.contains("ContainedObjects") && object["ContainedObjects"].isArray())
                {
                    QJsonArray items=object["ContainedObjects"].toArray();

                    exploreContent(items);
                }
            }


            if(ObjectType == "Quarter")
                quarterCount++;
            if(ObjectType == "DeckCustom")
                deckCustomCount++;
            if(ObjectType == "Deck")
               deckCount++;
            if(ObjectType == "Custom_Board")
                boardCount++;
            if(ObjectType == "Card")
                cardCount++;
            if(ObjectType == "Custom_Tile")
                tileCount++;
            if(ObjectType == "Custom_Token")
                tokenCount++;


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
