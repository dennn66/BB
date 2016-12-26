#include "boredombreaker.h"
#include "clicker.h"
#include "dongle.h"
#include <QApplication>
#include <QTextCodec>

#define APP_NAME "BB"
#define APP_VERSION "5.0"
#define ORG_NAME "Evl"
#define ORG_DOMAIN "none"


// Данные для ведения логов
static QTextStream* logStream;
static QFile* logFile;

// Типы сообщений
static const char* msgType[] =
{
"(II) ", // Info
"(WW) ", // Warning
"(EE) ", // Error
"(FF) " // Fatal error
};

const QString TextDescription = QObject::tr(
"%1 %2\n"
"Built on " __DATE__ " at " __TIME__ ".\n"
"Based on Qt %3.\n"
"Copyright %4. All rights reserved.\n"
"See also %5\n")
.arg(QLatin1String(APP_NAME), QLatin1String(APP_VERSION),
QLatin1String(QT_VERSION_STR), QLatin1String(ORG_NAME), QLatin1String(ORG_DOMAIN)
);

// Вывод логов в файл
//void customMessageHandler(QtMsgType type, const char* msg);
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
// Создание файла для логов
void installLog();
// Закрытие файла логов
void finishLog();
// Информация об ОС
QString getOSInfo();

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Под Windows устанавливаем кодировку cp1251
    #ifdef Q_WS_WIN
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
    // Под остальными ОС - utf8
    #else
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    #endif

    installLog();

    BoredomBreaker* bbwin = new BoredomBreaker;

    bbwin->show();
    //QObject::connect(bbwin, SIGNAL(closeEvent()), &app, SLOT(quit()));

    int mainReturn = app.exec();

    finishLog();
    return mainReturn;
}

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg){

    Q_UNUSED(context);
    //std::cout << msgType[type] << msg << std::endl;
    if(logStream && logStream->device())
    {
        *logStream << msgType[type] << msg << "\r\n";
    }


}

void installLog()
{
logFile = new QFile("messages.log");
if(logFile->open(QFile::WriteOnly |  QIODevice::Unbuffered))
logStream = new QTextStream(logFile);

#ifdef Q_WS_WIN
logStream->setCodec("Windows-1251");
// Под остальными ОС - utf8
#else
logStream->setCodec("utf-8");
#endif

// Запись заголовка с информацией о запуске
if(logStream && logStream->device())
{
*logStream << endl << TextDescription << "\r\n";
*logStream << QString("Markers: (II) informational, (WW) warning,") << "\r\n";
*logStream << QString("(EE) error, (FF) fatal error.") << "\r\n";
*logStream << getOSInfo() << "\r\n";
*logStream << QString("Runned at %1.").arg(QDateTime::currentDateTime().toString()) << "\r\n" << "\r\n";
}

qInstallMessageHandler(customMessageHandler);

qDebug("Success opening log file");
}

void finishLog()
{
qDebug("Success closing log file");

delete logStream;
logStream = 0;
delete logFile;
logFile = 0;

qInstallMessageHandler(0);
}

QString getOSInfo()
{
QString infoStr("Current Operating System: %1");
#ifdef Q_OS_WIN
switch(QSysInfo::windowsVersion())
{
case QSysInfo::WV_NT: return infoStr.arg("Windows NT");
case QSysInfo::WV_2000: return infoStr.arg("Windows 2000");
case QSysInfo::WV_XP: return infoStr.arg("Windows XP");
case QSysInfo::WV_2003: return infoStr.arg("Windows 2003");
case QSysInfo::WV_VISTA: return infoStr.arg("Windows Vista");
case QSysInfo::WV_WINDOWS7: return infoStr.arg("Windows Seven");
default: return infoStr.arg("Windows");
}
#endif // Q_OS_WIN

#ifdef Q_OS_MAC
switch(QSysInfo::MacintoshVersion())
{
case QSysInfo::MV_CHEETAH: return infoStr.arg("Mac OS X 1.0 Cheetah");
case QSysInfo::MV_PUMA: return infoStr.arg("Mac OS X 1.1 Puma");
case QSysInfo::MV_JAGUAR: return infoStr.arg("Mac OS X 1.2 Jaguar");
case QSysInfo::MV_PANTHER: return infoStr.arg("Mac OS X 1.3 Panther");
case QSysInfo::MV_TIGER: return infoStr.arg("Mac OS X 1.4 Tiger");
case QSysInfo::MV_LEOPARD: return infoStr.arg("Mac OS X 1.5 Leopard");
case QSysInfo::MV_SNOWLEOPARD: return infoStr.arg("Mac OS X 1.6 Snow Leopard");
default: return infoStr.arg("Mac OS X");
}
#endif // Q_OS_MAC

#ifdef Q_OS_LINUX
#ifdef LINUX_OS_VERSION
return infoStr.arg(LINUX_OS_VERSION);
#else
return infoStr.arg("Linux");
#endif // LINUX_OS_VERSION
#endif // Q_OS_LINUX
}
