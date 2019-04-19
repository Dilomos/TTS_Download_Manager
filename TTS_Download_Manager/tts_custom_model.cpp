#include "tts_custom_model.h"

TTS_Custom_Model::TTS_Custom_Model(QTreeWidgetItem*parent,QJsonObject &object)
    : QTreeWidgetItem(parent)
{
    m_name=object["Description"].toString();
    m_model=object["CustomMesh"].toObject()["MeshURL"].toString();
    m_texture=object["CustomMesh"].toObject()["DiffuseURL"].toString();
    m_collider=object["CustomMesh"].toObject()["ColliderURL"].toString();
    m_normal=object["CustomMesh"].toObject()["NormalURL"].toString();

    setText(0,m_texture);
}

QString TTS_Custom_Model::getTexture(void)
{
    return m_texture;
}
