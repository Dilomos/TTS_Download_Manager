#include "tts_customimage.h"
#include "tools.h"
#include "mainwindow.h"

TTS_CustomImage::TTS_CustomImage(TTS_TreeWidgetItem*parent,QJsonObject &object)
    : TTS_TreeWidgetItem(parent,QStringList(""))
{
    m_name=object["Nickname"].toString();
    if(m_name == "")
        m_name="(none)";

    QJsonObject customImage = object["CustomImage"].toObject();

    m_imageFront=customImage["ImageURL"].toString();
    m_imageBack=customImage["ImageSecondaryURL"].toString();

    setText(0,m_name);
    setText(1,m_imageFront);
    setText(2,m_imageBack);

    setDisplayable(true);
    setTexture(true);

    m_localImageFront=MainWindow::modPath+"/Models/"+Tools::getModelSaveFileName(m_imageFront);
    if(!Tools::fileExists(m_localImageFront))
        m_localImageFront ="";

    m_localImageBack=MainWindow::modPath+"/Models/"+Tools::getModelSaveFileName(m_imageBack);
    if(!Tools::fileExists(m_localImageBack))
        m_localImageBack ="";

}

QString TTS_CustomImage::getOnlineImageFront(void)
{
    return m_imageFront;
}

QString TTS_CustomImage::getLocalImageFront(void)
{
    return m_localImageFront;
}
