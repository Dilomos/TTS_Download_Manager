#include "tools.h"
#include <QStringList>
#include <QFileInfo>

Tools::Tools()
{

}




QString Tools::getImgSaveFileName(QString file)
{
    QString localSaveName = file;
    QStringList pathPart=localSaveName.split('.');
    QString extension = pathPart.last();
    if(extension.length() > 3)
        extension="jpg";
    localSaveName.remove(QRegExp(QString::fromUtf8("[-`~!@#$%^&*()_â€”+=|:;<>Â«Â»,.?///{}\'\"\\[]")));
    localSaveName+="."+extension;
    return localSaveName;
}

QString Tools::getModelSaveFileName(QString file)
{
    QString localSaveName = file;
    localSaveName.remove(QRegExp(QString::fromUtf8("[-`~!@#$%^&*()_â€”+=|:;<>Â«Â»,.?///{}\'\"\\[]")));
    localSaveName+=".obj";
    return localSaveName;
}

bool Tools::fileExists(QString path) {
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}
