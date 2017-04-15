#include <QProcess>
#include <QString>
#include <QTextCodec>
#include <QTextStream>
#include <QTcpSocket>
#include <QVariantMap>
#include "de_web_plugin.h"
#include "de_web_plugin_private.h"
#include "json.h"

int DeRestPluginPrivate::handleInfoApi(const ApiRequest &req, ApiResponse &rsp)
{
    if (req.path[2] != "info")
    {
        return REQ_NOT_HANDLED;
    }

    if(!checkApikeyAuthentification(req, rsp))
    {
        return REQ_READY_SEND;
    }

    if ((req.path.size() == 4) && (req.hdr.method() == "GET") && (req.path[3] == "timezones"))
    {
        return getTimezones(req,rsp);
    }

    return REQ_NOT_HANDLED;
}


int DeRestPluginPrivate::getTimezones(const ApiRequest &, ApiResponse &rsp)
{
    QVariantList timezones = getTimezoneMap();
    rsp.list=timezones;
    rsp.httpStatus = HttpStatusOk;
    return REQ_READY_SEND;
}

QVariantList DeRestPluginPrivate::getTimezoneMap() {
    QVariantList timezones;

#ifdef Q_OS_LINUX

    QProcess process;
    process.setProgram("timedatectl");
    QStringList arguments;
    arguments.append("list-timezones");
    process.setArguments(arguments);
    process.start();
    process.waitForFinished();
    QByteArray rawResult = process.readAllStandardOutput();
    QTextStream *timezoneStream = new QTextStream(rawResult);

    while (!timezoneStream->atEnd())
    {
        QString timezone = timezoneStream->readLine().trimmed();
        if (timezone.isEmpty())
        {
            continue;
        }
        timezones.append(timezone);
    }

    delete timezoneStream;
#endif

    return timezones;
}
