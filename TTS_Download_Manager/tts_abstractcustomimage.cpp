#include "tts_abstractcustomimage.h"
#include "mainwindow.h"
#include "tools.h"

TTS_AbstractCustomImage::TTS_AbstractCustomImage(TTS_TreeWidgetItem*parent)
    :TTS_AbstractFile(parent)
{

}

QString TTS_AbstractCustomImage::getOnlineImageFront(void)
{
    return m_imageFront;
}

QString TTS_AbstractCustomImage::getLocalImageFront(void)
{
    return m_localImageFront;
}

QString TTS_AbstractCustomImage::getOnlineImageBack(void)
{
    return m_imageBack;
}

QString TTS_AbstractCustomImage::getLocalImageBack(void)
{
    return m_localImageBack;
}

void TTS_AbstractCustomImage::transformOnlineFileToLocal(void)
{
    m_frontMissing=false;
    m_backMissing=false;
    m_missingFilesCount=0;
    m_filesCount=0;

    if(m_imageFront != "")
    {
        m_localImageFront=MainWindow::modPath+"/Images/"+Tools::getImgSaveFileName(m_imageFront);
        m_filesCount++;
        if(!Tools::fileExists(m_localImageFront))
        {
            m_frontMissing=true;
            m_missingFilesCount++;
            setBackground(0,QBrush(QColor(Qt::red),Qt::SolidPattern));
        }
    }
    else
    {
        m_localImageFront ="";
    }


    if(m_imageBack != "")
    {
        m_filesCount++;
        m_localImageBack=MainWindow::modPath+"/Images/"+Tools::getImgSaveFileName(m_imageBack);
        if(!Tools::fileExists(m_localImageBack))
        {
            m_backMissing=true;
            m_missingFilesCount++;
            setBackground(0,QBrush(QColor(Qt::red),Qt::SolidPattern));
        }

    }
    else
    {
        m_localImageBack ="";
    }

    m_id=m_imageFront+m_imageBack;
}

void TTS_AbstractCustomImage::setTreeItemText(void)
{
    setText(0,m_name);
    setText(1,m_imageFront);
    setText(2,m_imageBack);
}

bool TTS_AbstractCustomImage::isFrontMissing(void)
{
    return m_frontMissing;
}

bool TTS_AbstractCustomImage::isBackMissing(void)
{
    return m_backMissing;
}
