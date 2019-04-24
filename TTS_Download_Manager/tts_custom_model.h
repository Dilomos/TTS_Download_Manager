#ifndef TTS_CUSTOM_MODEL_H
#define TTS_CUSTOM_MODEL_H

#include "tts_treewidgetitem.h"
#include <QJsonObject>

class TTS_Custom_Model : public TTS_TreeWidgetItem
{
public:
    TTS_Custom_Model(TTS_TreeWidgetItem*parent,QJsonObject &object);
    QString getOnlineTexture(void);
    QString getOnlineModel(void);
    QString getLocalTexture(void);
    QString getLocalModel(void);

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

};

#endif // TTS_CUSTOM_MODEL_H
