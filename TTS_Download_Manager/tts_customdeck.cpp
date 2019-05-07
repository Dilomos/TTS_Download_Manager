#include "tts_customdeck.h"
#include "tools.h"
#include "mainwindow.h"

TTS_CustomDeck::TTS_CustomDeck(TTS_TreeWidgetItem*parent,QJsonObject &object)
    : TTS_AbstractCustomImage(parent)
{
    m_name=object["Nickname"].toString();
    if(m_name == "")
        m_name="(no name)";

    QJsonObject customImage = object["CustomDeck"].toObject();
    foreach(const QString& key, customImage.keys()) {
            QJsonObject value = customImage.value(key).toObject();
            m_imageFront=value["FaceURL"].toString();
            m_imageBack=value["BackURL"].toString();
        }

    setTreeItemText();

    setDisplayable(true);
    setTexture(true);

    transformOnlineFileToLocal();
}
