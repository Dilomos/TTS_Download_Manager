#ifndef TOOLS_H
#define TOOLS_H

#include <QString>

class Tools
{
public:
    Tools();
    static QString getImgSaveFileName(QString file);
    static QString getModelSaveFileName(QString file);
    static bool fileExists(QString path);
};

#endif // TOOLS_H
