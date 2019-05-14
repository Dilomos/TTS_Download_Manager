#include "tts_tableandsky.h"
#include "tools.h"
#include "mainwindow.h"

TTS_TableAndSky::TTS_TableAndSky(TTS_TreeWidgetItem*parent,QJsonObject &object)
    : TTS_AbstractCustomImage(parent)
{
    m_name=object["Table"].toString();
    if(m_name == "")
        m_name="(no table name)";

    QString sky_name = object["Sky"].toString();
    if(sky_name == "")
        sky_name="(no sky name)";
    m_name+=" " + sky_name;

     m_imageFront=object["TableURL"].toString();
     m_imageBack=object["SkyURL"].toString();

    setTreeItemText();

    setDisplayable(true);
    setTexture(true);

    transformOnlineFileToLocal();
}
