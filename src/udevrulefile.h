#ifndef UDEVRULEFILE_H
#define UDEVRULEFILE_H

#include <QString>

struct UdevRuleFile
{
    QString filePath;
    QString fileName;
    QString lastModified;
    QString birthTime;
    QString fileSize;
    QString description;

    UdevRuleFile(QString path, QString name , QString modify, QString birth, QString fsize, QString desc)
    {
        filePath = path;
        fileName = name;
        lastModified = modify;
        birthTime = birth;
        fileSize = fsize;
        description = desc;
    }

    UdevRuleFile(){}

    UdevRuleFile(QString f)
    {
        filePath = f;
    }

    bool operator==(const QString& right) const
    {
        if (filePath == right)
            return true;
        else
            return false;
    }

    bool operator==(const UdevRuleFile& right) const
    {
        if (filePath == right.filePath)
            return true;
        else
            return false;
    }
};


#endif // UDEVRULEFILE_H
