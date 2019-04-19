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

    /*foreach(const QString& key, json.keys()) {
           QJsonValue value = json.value(key);
           QTextStream(stdout) << key;

           if(json[key].isString())
            QTextStream(stdout)<< " : is String "<<endl;

           if(json[key].isArray())
            QTextStream(stdout)<<" : is Array <== "<<endl;

           //<<", Value = " << value.toString();
       }

    QTextStream(stdout) << " "<< endl<<endl;*/

    if (json.contains("SaveName") && json["SaveName"].isString())
    {
        mName = json["SaveName"].toString();
        QListWidgetItem::setText(mName);
        QTreeWidgetItem::setText(0,mName);
    }


    if (json.contains("ObjectStates") && json["ObjectStates"].isArray())
    {
        m_customModelParentTreeItem = new QTreeWidgetItem(this,QStringList("Custom Model"));

        QJsonArray objects=json["ObjectStates"].toArray();

        for (int index = 0; index < objects.size(); ++index) {
            QJsonObject object = objects[index].toObject();

            if (object.contains("Name") /*&& object["Name"].isString()*/)
            {
                QString ObjectType=object["Name"].toString();
                if(ObjectType == "Figurine_Custom")
                    figCount++;

                if(ObjectType == "Custom_Model")
                {
                    modelCount++;
                    TTS_Custom_Model *customModel = new TTS_Custom_Model(m_customModelParentTreeItem,object);
                    if(!m_customModelMap.contains(customModel->getTexture()))
                        m_customModelMap[customModel->getTexture()]=customModel;
                    else
                        delete customModel;
                }


                if(ObjectType == "Bag")
                    bagCount++;
                if(ObjectType == "Infinite_Bag")
                    infBagCount++;
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

unsigned int TTS_Game::getIndex(void)
{
    return m_gameIndex;
}

QTreeWidgetItem *TTS_Game::getCustomModelTreeItem(void)
{
   return m_customModelParentTreeItem;
}
