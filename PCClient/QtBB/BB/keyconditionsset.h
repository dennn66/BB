#ifndef KEYCONDITIONSSET_H
#define KEYCONDITIONSSET_H

#include <QString>
#include <QTextStream>
#include <QSettings>
#include "xpbar.h"
#include "keycondition.h"

#define KEYNUM 48

class KeyConditionsSet
{
public:
    KeyConditionsSet();
    ~KeyConditionsSet();
    KeyCondition* condition[KEYNUM];
    QString settings_file_name;
    QString nic;

    int LoadConfig(QString file_name);
    int SaveConfig(QString file_name);
private:
    static const char* DefaultKeyDB[KEYNUM];

signals:

public slots:
};

#endif // KEYCONDITIONSSET_H
