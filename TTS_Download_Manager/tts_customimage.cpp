#include "tts_customimage.h"

TTS_CustomImage::TTS_CustomImage(TTS_TreeWidgetItem*parent,QJsonObject &object)
    : TTS_AbstractCustomImage(parent)
{
    m_name=object["Nickname"].toString();
    if(m_name == "")
        m_name="(no name)";

    QJsonObject customImage = object["CustomImage"].toObject();

    m_imageFront=customImage["ImageURL"].toString();
    m_imageBack=customImage["ImageSecondaryURL"].toString();

    setTreeItemText();

    setDisplayable(true);
    setTexture(true);

    transformOnlineFileToLocal();
}

