#include "tts_customtablet.h"
#include "tools.h"
#include "mainwindow.h"

TTS_CustomTablet::TTS_CustomTablet(TTS_TreeWidgetItem*parent,QJsonObject &object)
    :TTS_AbstractFile(parent)
{
    m_name=object["Nickname"].toString();
    if(m_name == "")
        m_name="(no name)";

    QJsonObject customAsset = object["Tablet"].toObject();

    m_URL=customAsset["PageURL"].toString();

    m_missingFilesCount=0;
    m_filesCount=1;

    setText(0,m_name);
    setText(1,m_URL);

    setDisplayable(true);
    setTablet(true);

    m_id=m_URL;
}


QString TTS_CustomTablet::getUrl(void)
{
    return m_URL;
}
