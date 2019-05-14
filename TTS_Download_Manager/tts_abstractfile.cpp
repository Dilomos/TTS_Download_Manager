#include "tts_abstractfile.h"

TTS_AbstractFile::TTS_AbstractFile(TTS_TreeWidgetItem*parent)
: TTS_TreeWidgetItem(parent,QStringList(""))
{

}

bool TTS_AbstractFile::isSomethingMissing(void)
{
    if( m_missingFilesCount > 0)
        return true;
    else {
        return false;
    }
    //return m_modelMissing|m_textureMissing|m_colliderMissing|m_normalMissing;
}

QString TTS_AbstractFile::getId(void)
{
    return m_id;
}

int TTS_AbstractFile::getFileCount(void)
{
    return m_filesCount;
}

int TTS_AbstractFile::getMissingFileCount(void)
{
    return m_missingFilesCount;
}
