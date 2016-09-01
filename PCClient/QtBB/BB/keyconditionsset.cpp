#include "keyconditionsset.h"

char* KeyConditionsSet::DefaultKeyDB[48] = {"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8","F9","F10","F11",                   "F12",
                                    "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8", "9", "0",  "-",                     "=",
                                    "Q",  "W",  "E",  "R",  "T",  "Y",  "U",  "I", "O", "P",  "[",                     "]",
                                    "P1", "P2", "P3", "P4", "P5", "P6", "P7", "P8","P9","P0", "KEYPAD_DOT_AND_DELETE", "KEYPAD_SLASH"};



KeyConditionsSet::KeyConditionsSet()
{
    settings_file_name="default.cfg";
    nic = "<unnamed>";

    for(int i = 0;i < 48;i++){
        condition[i] = new KeyCondition(DefaultKeyDB[i]);
    }
}

KeyConditionsSet::~KeyConditionsSet()
{
    qDebug("KeyConditionsSet::~KeyConditionsSet()");
    for(int i = 0;i < 48;i++){
        delete condition[i];
    }
}


int KeyConditionsSet::LoadConfig(QString file_name){
    qDebug("KeyConditionsSet::LoadConfig");

    QSettings sett(file_name, QSettings::IniFormat);
    nic = sett.value("MAIN/NicName", "").toString();
    qDebug("Nic: %s", nic.toStdString().c_str());
    settings_file_name = file_name;
    qDebug("File: %s", file_name.toStdString().c_str());
    for(int i = 0;i < 48;i++){
        QString topic = "CONDITION";
        QTextStream topicstream(&topic);
        topicstream <<  i+1 << "/";

        QString var;
        QTextStream varstream(&var);
//        qDebug("topic: %s", topic.toStdString().c_str());
        for(int j = idCoolDown; j<CONDFNUM; j++){
            var = topic;
            varstream  << KeyCondition::conditionf_tag[j];
            condition[i]->conditionf[j] = sett.value(var.toStdString().c_str(), 0).toFloat();
        }

        for(int j = idMinCP; j<CONDINUM; j++){
            var = topic;
            varstream  << KeyCondition::conditioni_tag[j];
            QVariant v_cond = sett.value(var.toStdString().c_str(), 0xFF);

            //qDebug(v_cond.toString().toStdString().c_str());
            QString s_cond = v_cond.toString();
            if(strlen(s_cond.toStdString().c_str()) < 1){
                condition[i]->conditioni[j] = 0xFF;
            } else {
                condition[i]->conditioni[j] = s_cond.toInt();
            }
            //qDebug("Cond ID: %d = %d, \"%s\", %d", j, condition[i]->conditioni[j], s_cond.toStdString().c_str(), strlen(s_cond.toStdString().c_str()));
        }

        var = topic;
        varstream  << "$FSet";
        condition[i]->FSet = sett.value(var.toStdString().c_str()).toBool();


        for(int j = 0; j<CONDBNUM; j++){
            var = topic;
            varstream  << "$FGroupEnable" <<j;
            condition[i]->setGroupState(j, sett.value(var.toStdString().c_str(), 1).toBool());
        }

        var = topic;
        varstream  << "$FCtrl";
        condition[i]->ctrl = sett.value(var.toStdString().c_str()).toBool();
        var = topic;
        varstream  << "$FShift";
        condition[i]->shift = sett.value(var.toStdString().c_str()).toBool();
        var = topic;
        varstream  << "$Buttons";
        condition[i]->KeyString = sett.value(var.toStdString().c_str(), ".").toString();
        qDebug("\"%s\",", condition[i]->KeyString.toStdString().c_str());
    }

    return true;
}

int KeyConditionsSet::SaveConfig(QString file_name){
    qDebug("KeyConditionsSet::LoadConfig");

    QSettings sett(file_name, QSettings::IniFormat);

    sett.setValue("MAIN/NicName", nic);
    //nic = sett.value("MAIN/NicName", "").toString();
    qDebug("Nic: %s", nic.toStdString().c_str());


    settings_file_name = file_name;
    qDebug("File: %s", file_name.toStdString().c_str());
    for(int i = 0;i < 48;i++){
        QString topic = "CONDITION";
        QTextStream topicstream(&topic);
        topicstream <<  i+1 << "/";

        QString var;
        QTextStream varstream(&var);
        for(int j = idCoolDown; j<CONDFNUM; j++){
            var = topic;
            varstream  << KeyCondition::conditionf_tag[j];
            QString value;
            QTextStream valuestream(&value);
            valuestream << condition[i]->conditionf[j];
            sett.setValue(var.toStdString().c_str(), value.toStdString().c_str());
        }

        for(int j = idMinCP; j<CONDINUM; j++){
            var = topic;
            varstream  << KeyCondition::conditioni_tag[j];
            sett.setValue(var.toStdString().c_str(), condition[i]->conditioni[j]);
        }

        var = topic;
        varstream  << "$FSet";
        sett.setValue(var.toStdString().c_str(), condition[i]->FSet);

        for(int j = 0; j<CONDBNUM; j++){
            var = topic;
            varstream  << "$FGroupEnable" <<j;
            sett.setValue(var.toStdString().c_str(), condition[i]->getGroupState(j));
        }
        var = topic;
        varstream  << "$FCtrl";
        sett.setValue(var.toStdString().c_str(), condition[i]->ctrl);
        var = topic;
        varstream  << "$FShift";
        sett.setValue(var.toStdString().c_str(), condition[i]->shift);
        var = topic;
        varstream  << "$Buttons";
//        condition[i]->KeyString = sett.value(var.toStdString().c_str(), ".").toString();
        sett.setValue(var.toStdString().c_str(), condition[i]->KeyString);
//        qDebug("\"%s\",", condition[i]->KeyString.toStdString().c_str());
    }

    return true;
}
