#ifndef TTS_GAME_H
#define TTS_GAME_H

#include <QListWidgetItem>
#include <QTreeWidgetItem>
#include <QPixmap>
#include <QList>
#include "tts_custom_model.h"

class TTS_Game : public QListWidgetItem,public QTreeWidgetItem
{
public:
    TTS_Game(QString filename,int index);
    QString getPixPath(void);
    QString getFileName(void);
    unsigned int getIndex(void);
    QTreeWidgetItem *getCustomModelTreeItem(void);

private:
    QString m_jsonPath;
    QString m_iconePath;
    int m_gameIndex;
    QTreeWidgetItem *m_customModelParentTreeItem;
    QMap<QString,TTS_Custom_Model*> m_customModelMap;

    int figCount=0;
    int modelCount=0;
    int bagCount=0;
    int infBagCount=0;
    int quarterCount=0;
    int deckCount=0;
    int deckCustomCount=0;
    int boardCount=0;
    int cardCount=0;
};

#endif // TTS_GAME_H
