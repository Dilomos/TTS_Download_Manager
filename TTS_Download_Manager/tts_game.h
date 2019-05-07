#ifndef TTS_GAME_H
#define TTS_GAME_H

#include <QListWidgetItem>
#include <QPixmap>
#include <QList>

#include "tts_treewidgetitem.h"

class TTS_AbstractFile;

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
    bool isSomethingMissing(void);
    int getFileCount(void);
    int getMissingFileCount(void);

private:
    QString m_jsonPath;
    QString m_iconePath;
    int m_gameIndex;
    TTS_TreeWidgetItem *m_customModelParentTreeItem;
    TTS_TreeWidgetItem *m_customTokenParentTreeItem;
    TTS_TreeWidgetItem *m_customTileParentTreeItem;
    TTS_TreeWidgetItem *m_customCardParentTreeItem;
    TTS_TreeWidgetItem *m_customFigurineParentTreeItem;
    TTS_TreeWidgetItem *m_customBoardParentTreeItem;
    TTS_TreeWidgetItem *m_customDiceParentTreeItem;

    bool m_isLoaded;
    bool m_isSomethingMissing;

    QMap<QString,TTS_AbstractFile*> m_filesMap;
    void exploreContent(QJsonArray objects);

    QMap<QString,QString> m_unknownedTag;

    int m_figCount=0;
    int m_modelCount=0;
    int m_bagCount=0;
    int m_infBagCount=0;
    int m_otherCount=0;
    int m_deckCount=0;
    int m_deckCustomCount=0;
    int m_boardCount=0;
    int m_cardCount=0;
    int m_tokenCount=0;
    int m_tileCount=0;
    int m_diceCount=0;

    int m_filesCount;
    int m_missingFilesCount;
};

#endif // TTS_GAME_H
