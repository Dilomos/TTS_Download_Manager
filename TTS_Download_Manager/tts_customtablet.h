#ifndef TTS_CUSTOMTABLET_H
#define TTS_CUSTOMTABLET_H
#include "tts_abstractfile.h"
#include <QJsonObject>

class TTS_CustomTablet :public TTS_AbstractFile
{
public:
    TTS_CustomTablet(TTS_TreeWidgetItem*parent,QJsonObject &object);
    QString getUrl(void);

private:
    QString m_name;
    QString m_URL;
};

#endif // TTS_CUSTOMTABLET_H
