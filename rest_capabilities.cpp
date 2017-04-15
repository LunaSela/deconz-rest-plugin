#include <QString>
#include <QTextCodec>
#include <QTcpSocket>
#include <QVariantMap>
#include "de_web_plugin.h"
#include "de_web_plugin_private.h"
#include "json.h"

int DeRestPluginPrivate::handleCapabilitiesApi(const ApiRequest &req, ApiResponse &rsp)
{
    if (req.path[2] != "capabilities")
    {
        return REQ_NOT_HANDLED;
    }

    if(!checkApikeyAuthentification(req, rsp))
    {
        return REQ_READY_SEND;
    }

    if ((req.path.size() == 3) && (req.hdr.method() == "GET"))
    {
        return getCapabilities(req,rsp);
    }

    return REQ_NOT_HANDLED;
}

int DeRestPluginPrivate::getCapabilities(const ApiRequest &, ApiResponse &rsp) {

    QVariantMap lights;
    QVariantMap sensors;
    QVariantMap sensorsClip;
    QVariantMap sensorsZll;
    QVariantMap sensorsZgp;
    QVariantMap groups;
    QVariantMap scenes;
    QVariantMap scenesLightstates;
    QVariantMap schedules;
    QVariantMap rules;
    QVariantMap rulesConditions;
    QVariantMap rulesActions;
    QVariantMap resourcelinks;

    rsp.map["timezones"] = getTimezoneMap();

    lights["available"]= 63 - lights.size();
    rsp.map["lights"] = lights;

    int sensorsAvail = MAX_SENSORS - this->sensors.size();
    sensors["available"]= sensorsAvail;
    sensorsClip["available"]= sensorsAvail;
    sensorsZll["available"]= sensorsAvail;
    sensorsZgp["available"]= sensorsAvail;
    sensors["clip"]=sensorsClip;
    sensors["zll"]=sensorsZll;
    sensors["zgp"]=sensorsZgp;
    rsp.map["sensors"]=sensors;

    groups["available"]=64;
    rsp.map["groups"]=groups;

    scenes["available"]=0;
    scenesLightstates["available"]=0;
    scenes["lightstates"]=scenesLightstates;
    rsp.map["scenes"]=scenes;

    schedules["available"]=30000;
    rsp.map["schedules"]=schedules;

    rules["available"]=30000;
    rulesConditions["available"]=30000;
    rulesActions["available"]=30000;
    rules["conditions"]=rulesConditions;
    rules["actions"]=rulesActions;
    rsp.map["rules"]=rules;

    resourcelinks["available"]=0;
    rsp.map["resourcelinks"]=resourcelinks;

    rsp.httpStatus = HttpStatusOk;
    return REQ_READY_SEND;
}
