#include "tools.h"
#include <QStringList>
#include <QFileInfo>
#include <QMimeDatabase>
#include "mainwindow.h"
#include <QTextStream>

Tools::Tools()
{

}




QString Tools::getImgSaveFileName(QString file)
{
    QString localSaveName = file;
    localSaveName.remove("{Unique}");
    localSaveName.remove(QRegExp(QString::fromUtf8("[-`~!@#$%^&*()_â€”+=|:;<>Â«Â»,.?///\\{\\}\'\"\\[\\]]")));

    QString extension;

    QString fullPathFile = MainWindow::modPath+"/Images/"+localSaveName+".png";
    QFileInfo check_file(fullPathFile);
    if(check_file.exists())
    {
        extension="png";
    }
    else
    {
        fullPathFile = MainWindow::modPath+"/Images/"+localSaveName+".jpg";
        check_file.setFile(fullPathFile);
        if(check_file.exists())
        {
            extension="jpg";
        }
        else {
            fullPathFile = MainWindow::modPath+"/Images/"+localSaveName;
            QTextStream(stdout) << "Unable to find: "<<fullPathFile <<endl;
            extension="jpg";
        }
    }

    localSaveName+="."+extension;
    return localSaveName;
}

QString Tools::getModelSaveFileName(QString file)
{
    QString localSaveName = file;
    localSaveName.remove(QRegExp(QString::fromUtf8("[-`~!@#$%^&*()_â€”+=|:;<>Â«Â»,.?///\\{\\}\'\"\\[\\]]")));
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
