#ifndef TTS_TREEWIDGETITEM_H
#define TTS_TREEWIDGETITEM_H

#include <QTreeWidgetItem>

class TTS_TreeWidgetItem : public QTreeWidgetItem
{
public:
    //TTS_TreeWidgetItem(bool );
    TTS_TreeWidgetItem(TTS_TreeWidgetItem *parent=nullptr, const QStringList &strings=QStringList(),bool isDisplayable=false,bool is3DModel=false,bool isTexture=false);

    bool isDisplayable(void);
    bool is3DModel(void);
    bool isTexture(void);
    bool isAssetbundle(void);
    bool isTablet(void);

    void setDisplayable(bool val = false);
    void set3DModel(bool val = false);
    void setTexture(bool val = false);
    void setAssetbundle(bool val = false);
    void setTablet(bool val = false);

private:
    bool m_isDisplayable;
    bool m_is3DModel;
    bool m_isTexture;
    bool m_isAssetbundle;
    bool m_isTablet;
};

#endif // TTS_TREEWIDGETITEM_H
