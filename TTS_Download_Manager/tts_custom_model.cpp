#include "tts_custom_model.h"
#include "tools.h"
#include "mainwindow.h"



TTS_Custom_Model::TTS_Custom_Model(TTS_TreeWidgetItem*parent,QJsonObject &object)
    : TTS_TreeWidgetItem(parent,QStringList(""))
{
    m_name=object["Nickname"].toString();
    if(m_name == "")
        m_name="(no name)";

    QJsonObject customMesh = object["CustomMesh"].toObject();

    m_model=customMesh["MeshURL"].toString();
    m_texture=customMesh["DiffuseURL"].toString();
    m_collider=customMesh["ColliderURL"].toString();
    m_normal=customMesh["NormalURL"].toString();

    setText(0,m_name);
    setText(1,m_texture);
    setText(2,m_model);

    setDisplayable(true);
    set3DModel(true);

    m_localModel=MainWindow::modPath+"/Models/"+Tools::getModelSaveFileName(m_model);
    if(!Tools::fileExists(m_localModel))
        m_localModel ="";

    m_localCollider=MainWindow::modPath+"/Models/"+Tools::getModelSaveFileName(m_collider);
    if(!Tools::fileExists(m_localCollider))
        m_localCollider ="";

    m_localTexture=MainWindow::modPath+"/Images/"+Tools::getImgSaveFileName(m_texture);
    if(!Tools::fileExists(m_localTexture))
        m_localTexture ="";

    m_localNormal=MainWindow::modPath+"/Images/"+Tools::getImgSaveFileName(m_normal);
    if(!Tools::fileExists(m_localNormal))
        m_localNormal ="";

}

QString TTS_Custom_Model::getOnlineTexture(void)
{
    return m_texture;
}

QString TTS_Custom_Model::getOnlineModel(void)
{
    return m_model;
}

QString TTS_Custom_Model::getLocalTexture(void)
{
    return m_localTexture;
}

QString TTS_Custom_Model::getLocalModel(void)
{
    return m_localModel;
}
