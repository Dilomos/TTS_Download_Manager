#ifndef TTS_GAME_H
#define TTS_GAME_H

#include <QListWidgetItem>
#include <QPixmap>
#include <QList>

#include "tts_treewidgetitem.h"

class TTS_Custom_Model;
class TTS_CustomImage;

class TTS_Game : public QListWidgetItem,public TTS_TreeWidgetItem
{
public:
    TTS_Game(QString filename,int index);
    QString getPixPath(void);
    QString getFileName(void);
    int getIndex(void);
    TTS_TreeWidgetItem *getCustomModelTreeItem(void);
    void loadGameFromFile(void);
    bool isLoaded(void);

private:
    QString m_jsonPath;
    QString m_iconePath;
    int m_gameIndex;
    TTS_TreeWidgetItem *m_customModelParentTreeItem;
    TTS_TreeWidgetItem *m_customImageParentTreeItem;

    bool m_isLoaded;

    QMap<QString,TTS_Custom_Model*> m_customModelMap;
    QMap<QString,TTS_CustomImage*> m_customImageMap;
    void exploreContent(QJsonArray objects);

    QMap<QString,QString> m_unknownedTag;

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
