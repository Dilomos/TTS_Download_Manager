#ifndef TTS_CUSTOMDECK_H
#define TTS_CUSTOMDECK_H

#include "tts_abstractcustomimage.h"
#include <QJsonObject>

class TTS_CustomDeck : public TTS_AbstractCustomImage
{
public:
    TTS_CustomDeck(TTS_TreeWidgetItem*parent,QJsonObject &object);

};

#endif // TTS_CUSTOMDECK_H
