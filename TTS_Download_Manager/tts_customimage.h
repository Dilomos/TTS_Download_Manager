#ifndef TTS_CUSTOMIMAGE_H
#define TTS_CUSTOMIMAGE_H
#include "tts_treewidgetitem.h"
#include <QJsonObject>

class TTS_CustomImage : public TTS_TreeWidgetItem
{
public:
    TTS_CustomImage(TTS_TreeWidgetItem*parent,QJsonObject &object);
    QString getOnlineImageFront(void);
    QString getLocalImageFront(void);
    QString getOnlineImageBack(void);
    QString getLocalImageBack(void);

private:
    QString m_name;

    QString m_imageFront;
    QString m_localImageFront;

    QString m_imageBack;
    QString m_localImageBack;
};

#endif // TTS_CUSTOMIMAGE_H
