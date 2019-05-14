#ifndef TTS_TABLEANDSKY_H
#define TTS_TABLEANDSKY_H
#include "tts_abstractcustomimage.h"
#include <QJsonObject>

class TTS_TableAndSky : public TTS_AbstractCustomImage
{
public:
    TTS_TableAndSky(TTS_TreeWidgetItem*parent,QJsonObject &object);
};

#endif // TTS_TABLEANDSKY_H
