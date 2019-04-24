#ifndef TTS_GAME_H
#define TTS_GAME_H

#include <QListWidgetItem>
#include <QPixmap>
#include <QList>
#include "tts_custom_model.h"
#include "tts_treewidgetitem.h"

class TTS_Game : public QListWidgetItem,public TTS_TreeWidgetItem
{
public:
    TTS_Game(QString filename,int index);
    QString getPixPath(void);
    QString getFileName(void);
    int getIndex(void);
    TTS_TreeWidgetItem *getCustomModelTreeItem(void);

private:
    QString m_jsonPath;
    QString m_iconePath;
    int m_gameIndex;
    TTS_TreeWidgetItem *m_customModelParentTreeItem;
    QMap<QString,TTS_Custom_Model*> m_customModelMap;
    void exploreContent(QJsonArray objects);

    int figCount=0;
    int modelCount=0;
    int bagCount=0;
    int infBagCount=0;
    int quarterCount=0;
    int deckCount=0;
    int deckCustomCount=0;
    int boardCount=0;
    int cardCount=0;
    int tokenCount=0;
    int tileCount=0;
};

#endif // TTS_GAME_H
