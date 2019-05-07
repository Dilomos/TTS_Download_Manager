#ifndef TTS_CUSTOMIMAGE_H
#define TTS_CUSTOMIMAGE_H

#include "tts_abstractcustomimage.h"
#include <QJsonObject>

class TTS_CustomImage : public TTS_AbstractCustomImage
{
public:
    TTS_CustomImage(TTS_TreeWidgetItem*parent,QJsonObject &object);
};

#endif // TTS_CUSTOMIMAGE_H
