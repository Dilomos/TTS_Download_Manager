#ifndef TTS_ABSTRACTFILE_H
#define TTS_ABSTRACTFILE_H
#include <QString>

class TTS_AbstractFile
{
public:
    TTS_AbstractFile();
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
