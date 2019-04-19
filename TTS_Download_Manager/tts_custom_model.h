#ifndef TTS_CUSTOM_MODEL_H
#define TTS_CUSTOM_MODEL_H

#include <QTreeWidgetItem>
#include <QJsonObject>

class TTS_Custom_Model : public QTreeWidgetItem
{
public:
    TTS_Custom_Model(QTreeWidgetItem*parent,QJsonObject &object);
    QString getTexture(void);

private:
    QString m_name;
    QString m_model;
    QString m_texture;
    QString m_collider;
    QString m_normal;

};

#endif // TTS_CUSTOM_MODEL_H
