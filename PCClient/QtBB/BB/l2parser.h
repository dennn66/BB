#ifndef L2PARSER_H
#define L2PARSER_H

#include <QObject>
#include <QTime>

#include "l2window.h"

class L2parser : public QObject
{
    Q_OBJECT
public:
    L2Window* currentl2w;

    explicit L2parser(QObject *parent = 0);
    void setActiveL2W(L2Window* l2w);



public slots:
   void process();

signals:
   void finished();
   void error(QString err);
   void showParserStatus(int updatetime);

};

#endif // L2PARSER_H
