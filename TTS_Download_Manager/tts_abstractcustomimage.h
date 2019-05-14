#ifndef TTS_ABSTRACTCUSTOMIMAGE_H
#define TTS_ABSTRACTCUSTOMIMAGE_H
#include "tts_abstractfile.h"

class TTS_AbstractCustomImage:public TTS_AbstractFile
{
public:
    TTS_AbstractCustomImage(TTS_TreeWidgetItem*parent);
    QString getOnlineImageFront(void);
    QString getLocalImageFront(void);
    QString getOnlineImageBack(void);
    QString getLocalImageBack(void);

    bool isFrontMissing(void);
    bool isBackMissing(void);

protected:
    QString m_name;

    QString m_imageFront;
    QString m_localImageFront;

    QString m_imageBack;
    QString m_localImageBack;

    bool m_frontMissing;
    bool m_backMissing;

    void transformOnlineFileToLocal(void);
    void setTreeItemText(void);
};

#endif // TTS_ABSTRACTCUSTOMIMAGE_H
