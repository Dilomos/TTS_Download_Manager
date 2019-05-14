#ifndef TTS_CUSTOMASSETBUNDLE_H
#define TTS_CUSTOMASSETBUNDLE_H

#include "tts_abstractfile.h"
#include <QJsonObject>

class TTS_CustomAssetbundle :public TTS_AbstractFile
{
public:
    TTS_CustomAssetbundle(TTS_TreeWidgetItem*parent,QJsonObject &object);

    QString getOnlineAsset(void);
    QString getOnlineAssetSecondary(void);
    QString getLocalAsset(void);
    QString getLocalAssetSecondary(void);

    bool isAssetMissing(void);
    bool isAssetSecondaryMissing(void);

private:
    QString m_name;

    QString m_assetbundel;
    QString m_assetbundelSecondary;

    QString m_localAssetbundel;
    QString m_localAssetbundelSecondary;

    bool m_assetMissing;
    bool m_assetSecondaryMissing;
};

#endif // TTS_CUSTOMASSETBUNDLE_H
