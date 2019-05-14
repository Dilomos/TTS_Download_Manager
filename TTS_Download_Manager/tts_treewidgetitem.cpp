#include "tts_treewidgetitem.h"

/*TTS_TreeWidgetItem::TTS_TreeWidgetItem()
{
}*/

TTS_TreeWidgetItem::TTS_TreeWidgetItem(TTS_TreeWidgetItem *parent, const QStringList &strings,bool isDisplayable,bool is3DModel,bool isTexture)
    :QTreeWidgetItem(parent,strings),m_isDisplayable(isDisplayable),m_is3DModel(is3DModel),m_isTexture(isTexture)
{
    m_isDisplayable=false;
    m_is3DModel=false;
    m_isTexture=false;
    m_isAssetbundle=false;
    m_isTablet=false;
}

bool TTS_TreeWidgetItem::isDisplayable(void)
{
    return m_isDisplayable;
}

bool TTS_TreeWidgetItem::is3DModel(void)
{
    return m_is3DModel;
}

bool TTS_TreeWidgetItem::isTexture(void)
{
    return m_isTexture;
}

bool TTS_TreeWidgetItem::isAssetbundle(void)
{
    return m_isAssetbundle;
}

bool TTS_TreeWidgetItem::isTablet(void)
{
    return m_isTablet;
}

void TTS_TreeWidgetItem::setDisplayable(bool val)
{
    m_isDisplayable = val;
}

void TTS_TreeWidgetItem::set3DModel(bool val)
{
    m_is3DModel = val;
}

void TTS_TreeWidgetItem::setTexture(bool val)
{
    m_isTexture = val;
}

void TTS_TreeWidgetItem::setAssetbundle(bool val)
{
    m_isAssetbundle = val;
}

void TTS_TreeWidgetItem::setTablet(bool val)
{
    m_isTablet = val;
}

