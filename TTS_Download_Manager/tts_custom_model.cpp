#include "tts_custom_model.h"
#include "tools.h"
#include "mainwindow.h"



TTS_Custom_Model::TTS_Custom_Model(TTS_TreeWidgetItem*parent,QJsonObject &object)
 :TTS_AbstractFile(parent)
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

    m_modelMissing=false;
    m_textureMissing=false;
    m_colliderMissing=false;
    m_normalMissing=false;
    m_missingFilesCount=0;
    m_filesCount=0;

    if(m_model != "")
    {
        m_filesCount++;
        m_localModel=MainWindow::modPath+"/Models/"+Tools::getModelSaveFileName(m_model);
        if(!Tools::fileExists(m_localModel))
        {
            m_modelMissing=true;
            m_missingFilesCount++;
            setBackground(0,QBrush(QColor(Qt::red),Qt::SolidPattern));
        }
    }
    else {
        m_localModel ="";
        setBackground(0,QBrush(QColor(Qt::red),Qt::SolidPattern));
    }

    if(m_collider != "")
    {
        m_filesCount++;
        m_localCollider=MainWindow::modPath+"/Models/"+Tools::getModelSaveFileName(m_collider);
        if(!Tools::fileExists(m_localCollider))
        {
            m_colliderMissing=true;
            m_missingFilesCount++;
            setBackground(0,QBrush(QColor(Qt::red),Qt::SolidPattern));
        }
    }
    else {
        m_localCollider ="";
    }

    if(m_texture != "")
    {
        m_filesCount++;
        m_localTexture=MainWindow::modPath+"/Images/"+Tools::getImgSaveFileName(m_texture);
        if(!Tools::fileExists(m_localTexture))
        {
            m_textureMissing=true;
            m_missingFilesCount++;
            setBackground(0,QBrush(QColor(Qt::red),Qt::SolidPattern));
        }
    }
    else {
        m_localTexture ="";
    }

    if(m_normal != "")
    {
        m_filesCount++;
        m_localNormal=MainWindow::modPath+"/Images/"+Tools::getImgSaveFileName(m_normal);
        if(!Tools::fileExists(m_localNormal))
        {
            m_normalMissing=true;
            m_missingFilesCount++;
        }
    }
    else {
        m_localNormal ="";
    }

    m_id=m_model+m_texture+m_normal+m_collider;
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

QString TTS_Custom_Model::getOnlineNormal(void)
{
    return m_normal;
}

QString TTS_Custom_Model::getOnlineCollider(void)
{
    return m_collider;
}

QString TTS_Custom_Model::getLocalNormal(void)
{
    return m_localNormal;
}

QString TTS_Custom_Model::getLocalCollider(void)
{
    return m_localCollider;
}

bool TTS_Custom_Model::isModelMissing(void)
{
   return m_modelMissing;
}
bool TTS_Custom_Model::isTextureMissing(void)
{
    return m_textureMissing;
}
bool TTS_Custom_Model::isNormalMissing(void)
{
    return m_normalMissing;
}
bool TTS_Custom_Model::isColliderMissing(void)
{
    return m_colliderMissing;
}
