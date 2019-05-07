#ifndef TTS_CUSTOM_MODEL_H
#define TTS_CUSTOM_MODEL_H

#include "tts_treewidgetitem.h"
#include "tts_abstractfile.h"
#include <QJsonObject>

class TTS_Custom_Model : public TTS_TreeWidgetItem,public TTS_AbstractFile
{
public:
    TTS_Custom_Model(TTS_TreeWidgetItem*parent,QJsonObject &object);
    QString getOnlineTexture(void);
    QString getOnlineModel(void);
    QString getLocalTexture(void);
    QString getLocalModel(void);

    QString getOnlineNormal(void);
    QString getOnlineCollider(void);
    QString getLocalNormal(void);
    QString getLocalCollider(void);

    bool isModelMissing(void);
    bool isTextureMissing(void);
    bool isNormalMissing(void);
    bool isColliderMissing(void);

private:
    QString m_name;

    QString m_model;
    QString m_texture;
    QString m_collider;
    QString m_normal;

    QString m_localModel;
    QString m_localTexture;
    QString m_localCollider;
    QString m_localNormal;


    bool m_modelMissing;
    bool m_textureMissing;
    bool m_colliderMissing;
    bool m_normalMissing;

};

#endif // TTS_CUSTOM_MODEL_H
