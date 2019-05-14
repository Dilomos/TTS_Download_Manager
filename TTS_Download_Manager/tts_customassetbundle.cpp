#include "tts_customassetbundle.h"
#include "tools.h"
#include "mainwindow.h"

TTS_CustomAssetbundle::TTS_CustomAssetbundle(TTS_TreeWidgetItem*parent,QJsonObject &object)
    :TTS_AbstractFile(parent)
{
    m_name=object["Nickname"].toString();
    if(m_name == "")
        m_name="(no name)";

    QJsonObject customAsset = object["CustomAssetbundle"].toObject();

    m_assetbundel=customAsset["AssetbundleURL"].toString();
    m_assetbundelSecondary=customAsset["AssetbundleSecondaryURL"].toString();

    setText(0,m_name);
    setText(1,m_assetbundel);
    setText(2,m_assetbundelSecondary);

    setDisplayable(true);
    setAssetbundle(true);

    m_assetMissing=false;
    m_assetSecondaryMissing=false;

    m_missingFilesCount=0;
    m_filesCount=0;

    if(m_assetbundel != "")
    {
        m_filesCount++;
        m_localAssetbundel=MainWindow::modPath+"/Assetbundles/"+Tools::getAssetSaveFileName(m_assetbundel);
        if(!Tools::fileExists(m_localAssetbundel))
        {
            m_assetMissing=true;
            m_missingFilesCount++;
            setBackground(0,QBrush(QColor(Qt::red),Qt::SolidPattern));
        }
    }
    else {
        m_localAssetbundel ="";
    }

    if(m_assetbundelSecondary != "")
    {
        m_filesCount++;
        m_localAssetbundelSecondary=MainWindow::modPath+"/Assetbundles/"+Tools::getAssetSaveFileName(m_assetbundelSecondary);
        if(!Tools::fileExists(m_localAssetbundelSecondary))
        {
            m_assetSecondaryMissing=true;
            m_missingFilesCount++;
        }
    }
    else {
        m_localAssetbundelSecondary ="";
    }

    m_id=m_assetbundel+m_assetbundelSecondary;
}

QString TTS_CustomAssetbundle::getOnlineAsset(void)
{
   return m_assetbundel;
}
QString TTS_CustomAssetbundle::getOnlineAssetSecondary(void)
{
    return m_assetbundelSecondary;
}
QString TTS_CustomAssetbundle::getLocalAsset(void)
{
    return m_localAssetbundel;
}
QString TTS_CustomAssetbundle::getLocalAssetSecondary(void)
{
    return m_localAssetbundelSecondary;
}

bool TTS_CustomAssetbundle::isAssetMissing(void)
{
    return m_assetMissing;
}
bool TTS_CustomAssetbundle::isAssetSecondaryMissing(void)
{
    return m_assetSecondaryMissing;
}
