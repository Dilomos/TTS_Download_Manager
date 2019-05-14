#ifndef TTS_ABSTRACTFILE_H
#define TTS_ABSTRACTFILE_H
#include <QString>
#include "tts_treewidgetitem.h"

class TTS_AbstractFile:public TTS_TreeWidgetItem
{
public:
    TTS_AbstractFile(TTS_TreeWidgetItem*parent);
    bool isSomethingMissing(void);
    QString getId(void);
    int getFileCount(void);
    int getMissingFileCount(void);

protected :
    int m_missingFilesCount;
    int m_filesCount;
    QString m_id;
};

#endif // TTS_ABSTRACTFILE_H
