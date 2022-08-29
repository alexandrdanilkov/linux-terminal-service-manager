
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__ltsm_dbus_adaptor_h__adaptor__H__
#define __sdbuscpp__ltsm_dbus_adaptor_h__adaptor__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace LTSM {
namespace Manager {

class Service_adaptor
{
public:
    static constexpr const char* INTERFACE_NAME = "LTSM.Manager.Service";

protected:
    Service_adaptor(sdbus::IObject& object)
        : object_(object)
    {
        object_.registerMethod("busGetServiceVersion").onInterface(INTERFACE_NAME).withOutputParamNames("version").implementedAs([this](){ return this->busGetServiceVersion(); });
        object_.registerMethod("busStartLoginSession").onInterface(INTERFACE_NAME).withInputParamNames("remoteAddr", "connType").withOutputParamNames("display").implementedAs([this](const std::string& remoteAddr, const std::string& connType){ return this->busStartLoginSession(remoteAddr, connType); });
        object_.registerMethod("busCreateAuthFile").onInterface(INTERFACE_NAME).withInputParamNames("display").withOutputParamNames("path").implementedAs([this](const int32_t& display){ return this->busCreateAuthFile(display); });
        object_.registerMethod("busShutdownConnector").onInterface(INTERFACE_NAME).withInputParamNames("display").withOutputParamNames("success").implementedAs([this](const int32_t& display){ return this->busShutdownConnector(display); });
        object_.registerMethod("busShutdownService").onInterface(INTERFACE_NAME).withOutputParamNames("success").implementedAs([this](){ return this->busShutdownService(); });
        object_.registerMethod("busShutdownDisplay").onInterface(INTERFACE_NAME).withInputParamNames("display").withOutputParamNames("success").implementedAs([this](const int32_t& display){ return this->busShutdownDisplay(display); });
        object_.registerMethod("busStartUserSession").onInterface(INTERFACE_NAME).withInputParamNames("display", "userName", "remoteAddr", "connType").withOutputParamNames("display").implementedAs([this](const int32_t& display, const std::string& userName, const std::string& remoteAddr, const std::string& connType){ return this->busStartUserSession(display, userName, remoteAddr, connType); });
        object_.registerMethod("busSendMessage").onInterface(INTERFACE_NAME).withInputParamNames("display", "message").withOutputParamNames("result").implementedAs([this](const int32_t& display, const std::string& message){ return this->busSendMessage(display, message); });
        object_.registerMethod("busSetDebugLevel").onInterface(INTERFACE_NAME).withInputParamNames("level").withOutputParamNames("result").implementedAs([this](const std::string& level){ return this->busSetDebugLevel(level); });
        object_.registerMethod("busSetEncryptionInfo").onInterface(INTERFACE_NAME).withInputParamNames("display", "info").withOutputParamNames("result").implementedAs([this](const int32_t& display, const std::string& info){ return this->busSetEncryptionInfo(display, info); });
        object_.registerMethod("busSetSessionDurationSec").onInterface(INTERFACE_NAME).withInputParamNames("display", "duration").withOutputParamNames("result").implementedAs([this](const int32_t& display, const uint32_t& duration){ return this->busSetSessionDurationSec(display, duration); });
        object_.registerMethod("busSetSessionPolicy").onInterface(INTERFACE_NAME).withInputParamNames("display", "policy").withOutputParamNames("result").implementedAs([this](const int32_t& display, const std::string& policy){ return this->busSetSessionPolicy(display, policy); });
        object_.registerMethod("busEncryptionInfo").onInterface(INTERFACE_NAME).withInputParamNames("display").withOutputParamNames("result").implementedAs([this](const int32_t& display){ return this->busEncryptionInfo(display); });
        object_.registerMethod("busDisplayResized").onInterface(INTERFACE_NAME).withInputParamNames("display", "width", "height").withOutputParamNames("result").implementedAs([this](const int32_t& display, const uint16_t& width, const uint16_t& height){ return this->busDisplayResized(display, width, height); });
        object_.registerMethod("busConnectorTerminated").onInterface(INTERFACE_NAME).withInputParamNames("display").withOutputParamNames("result").implementedAs([this](const int32_t& display){ return this->busConnectorTerminated(display); });
        object_.registerMethod("busConnectorSwitched").onInterface(INTERFACE_NAME).withInputParamNames("oldDisplay", "newDisplay").withOutputParamNames("result").implementedAs([this](const int32_t& oldDisplay, const int32_t& newDisplay){ return this->busConnectorSwitched(oldDisplay, newDisplay); });
        object_.registerMethod("busConnectorAlive").onInterface(INTERFACE_NAME).withInputParamNames("display").withOutputParamNames("result").implementedAs([this](const int32_t& display){ return this->busConnectorAlive(display); });
        object_.registerMethod("busCheckAuthenticate").onInterface(INTERFACE_NAME).withInputParamNames("display", "login", "password").withOutputParamNames("result").implementedAs([this](const int32_t& display, const std::string& login, const std::string& password){ return this->busCheckAuthenticate(display, login, password); });
        object_.registerMethod("busGetSessions").onInterface(INTERFACE_NAME).withOutputParamNames("result").implementedAs([this](){ return this->busGetSessions(); });
        object_.registerMethod("busRenderRect").onInterface(INTERFACE_NAME).withInputParamNames("display", "rect", "color", "fill").withOutputParamNames("result").implementedAs([this](const int32_t& display, const sdbus::Struct<int16_t, int16_t, uint16_t, uint16_t>& rect, const sdbus::Struct<uint8_t, uint8_t, uint8_t>& color, const bool& fill){ return this->busRenderRect(display, rect, color, fill); });
        object_.registerMethod("busRenderText").onInterface(INTERFACE_NAME).withInputParamNames("display", "text", "pos", "color").withOutputParamNames("result").implementedAs([this](const int32_t& display, const std::string& text, const sdbus::Struct<int16_t, int16_t>& pos, const sdbus::Struct<uint8_t, uint8_t, uint8_t>& color){ return this->busRenderText(display, text, pos, color); });
        object_.registerMethod("busRenderClear").onInterface(INTERFACE_NAME).withInputParamNames("display").withOutputParamNames("result").implementedAs([this](const int32_t& display){ return this->busRenderClear(display); });
        object_.registerMethod("helperIsAutoComplete").onInterface(INTERFACE_NAME).withInputParamNames("display").withOutputParamNames("success").implementedAs([this](const int32_t& display){ return this->helperIsAutoComplete(display); });
        object_.registerMethod("helperIdleTimeoutAction").onInterface(INTERFACE_NAME).withInputParamNames("display").withOutputParamNames("result").implementedAs([this](const int32_t& display){ return this->helperIdleTimeoutAction(display); });
        object_.registerMethod("helperGetIdleTimeoutSec").onInterface(INTERFACE_NAME).withInputParamNames("display").withOutputParamNames("result").implementedAs([this](const int32_t& display){ return this->helperGetIdleTimeoutSec(display); });
        object_.registerMethod("helperGetTitle").onInterface(INTERFACE_NAME).withInputParamNames("display").withOutputParamNames("result").implementedAs([this](const int32_t& display){ return this->helperGetTitle(display); });
        object_.registerMethod("helperGetDateFormat").onInterface(INTERFACE_NAME).withInputParamNames("display").withOutputParamNames("result").implementedAs([this](const int32_t& display){ return this->helperGetDateFormat(display); });
        object_.registerMethod("helperGetUsersList").onInterface(INTERFACE_NAME).withInputParamNames("display").withOutputParamNames("result").implementedAs([this](const int32_t& display){ return this->helperGetUsersList(display); });
        object_.registerMethod("helperWidgetStartedAction").onInterface(INTERFACE_NAME).withInputParamNames("display").withOutputParamNames("result").implementedAs([this](const int32_t& display){ return this->helperWidgetStartedAction(display); });
        object_.registerMethod("helperSetSessionLoginPassword").onInterface(INTERFACE_NAME).withInputParamNames("display", "login", "password", "action").withOutputParamNames("result").implementedAs([this](const int32_t& display, const std::string& login, const std::string& password, const bool& action){ return this->helperSetSessionLoginPassword(display, login, password, action); });
        object_.registerSignal("helperWidgetStarted").onInterface(INTERFACE_NAME).withParameters<int32_t>("display");
        object_.registerSignal("helperSetLoginPassword").onInterface(INTERFACE_NAME).withParameters<int32_t, std::string, std::string, bool>("display", "login", "pass", "autologin");
        object_.registerSignal("helperWidgetCentered").onInterface(INTERFACE_NAME).withParameters<int32_t>("display");
        object_.registerSignal("loginFailure").onInterface(INTERFACE_NAME).withParameters<int32_t, std::string>("display", "msg");
        object_.registerSignal("loginSuccess").onInterface(INTERFACE_NAME).withParameters<int32_t, std::string>("display", "userName");
        object_.registerSignal("shutdownConnector").onInterface(INTERFACE_NAME).withParameters<int32_t>("display");
        object_.registerSignal("pingConnector").onInterface(INTERFACE_NAME).withParameters<int32_t>("display");
        object_.registerSignal("sendBellSignal").onInterface(INTERFACE_NAME).withParameters<int32_t>("display");
        object_.registerSignal("sessionReconnect").onInterface(INTERFACE_NAME).withParameters<std::string, std::string>("removeAddr", "connType");
        object_.registerSignal("sessionChanged").onInterface(INTERFACE_NAME).withParameters<int32_t>("display");
        object_.registerSignal("displayRemoved").onInterface(INTERFACE_NAME).withParameters<int32_t>("display");
        object_.registerSignal("clearRenderPrimitives").onInterface(INTERFACE_NAME).withParameters<int32_t>("display");
        object_.registerSignal("addRenderRect").onInterface(INTERFACE_NAME).withParameters<int32_t, sdbus::Struct<int16_t, int16_t, uint16_t, uint16_t>, sdbus::Struct<uint8_t, uint8_t, uint8_t>, bool>("display", "rect", "color", "fill");
        object_.registerSignal("addRenderText").onInterface(INTERFACE_NAME).withParameters<int32_t, std::string, sdbus::Struct<int16_t, int16_t>, sdbus::Struct<uint8_t, uint8_t, uint8_t>>("display", "text", "pos", "color");
        object_.registerSignal("debugLevel").onInterface(INTERFACE_NAME).withParameters<std::string>("level");
    }

    ~Service_adaptor() = default;

public:
    void emitHelperWidgetStarted(const int32_t& display)
    {
        object_.emitSignal("helperWidgetStarted").onInterface(INTERFACE_NAME).withArguments(display);
    }

    void emitHelperSetLoginPassword(const int32_t& display, const std::string& login, const std::string& pass, const bool& autologin)
    {
        object_.emitSignal("helperSetLoginPassword").onInterface(INTERFACE_NAME).withArguments(display, login, pass, autologin);
    }

    void emitHelperWidgetCentered(const int32_t& display)
    {
        object_.emitSignal("helperWidgetCentered").onInterface(INTERFACE_NAME).withArguments(display);
    }

    void emitLoginFailure(const int32_t& display, const std::string& msg)
    {
        object_.emitSignal("loginFailure").onInterface(INTERFACE_NAME).withArguments(display, msg);
    }

    void emitLoginSuccess(const int32_t& display, const std::string& userName)
    {
        object_.emitSignal("loginSuccess").onInterface(INTERFACE_NAME).withArguments(display, userName);
    }

    void emitShutdownConnector(const int32_t& display)
    {
        object_.emitSignal("shutdownConnector").onInterface(INTERFACE_NAME).withArguments(display);
    }

    void emitPingConnector(const int32_t& display)
    {
        object_.emitSignal("pingConnector").onInterface(INTERFACE_NAME).withArguments(display);
    }

    void emitSendBellSignal(const int32_t& display)
    {
        object_.emitSignal("sendBellSignal").onInterface(INTERFACE_NAME).withArguments(display);
    }

    void emitSessionReconnect(const std::string& removeAddr, const std::string& connType)
    {
        object_.emitSignal("sessionReconnect").onInterface(INTERFACE_NAME).withArguments(removeAddr, connType);
    }

    void emitSessionChanged(const int32_t& display)
    {
        object_.emitSignal("sessionChanged").onInterface(INTERFACE_NAME).withArguments(display);
    }

    void emitDisplayRemoved(const int32_t& display)
    {
        object_.emitSignal("displayRemoved").onInterface(INTERFACE_NAME).withArguments(display);
    }

    void emitClearRenderPrimitives(const int32_t& display)
    {
        object_.emitSignal("clearRenderPrimitives").onInterface(INTERFACE_NAME).withArguments(display);
    }

    void emitAddRenderRect(const int32_t& display, const sdbus::Struct<int16_t, int16_t, uint16_t, uint16_t>& rect, const sdbus::Struct<uint8_t, uint8_t, uint8_t>& color, const bool& fill)
    {
        object_.emitSignal("addRenderRect").onInterface(INTERFACE_NAME).withArguments(display, rect, color, fill);
    }

    void emitAddRenderText(const int32_t& display, const std::string& text, const sdbus::Struct<int16_t, int16_t>& pos, const sdbus::Struct<uint8_t, uint8_t, uint8_t>& color)
    {
        object_.emitSignal("addRenderText").onInterface(INTERFACE_NAME).withArguments(display, text, pos, color);
    }

    void emitDebugLevel(const std::string& level)
    {
        object_.emitSignal("debugLevel").onInterface(INTERFACE_NAME).withArguments(level);
    }

private:
    virtual int32_t busGetServiceVersion() = 0;
    virtual int32_t busStartLoginSession(const std::string& remoteAddr, const std::string& connType) = 0;
    virtual std::string busCreateAuthFile(const int32_t& display) = 0;
    virtual bool busShutdownConnector(const int32_t& display) = 0;
    virtual bool busShutdownService() = 0;
    virtual bool busShutdownDisplay(const int32_t& display) = 0;
    virtual int32_t busStartUserSession(const int32_t& display, const std::string& userName, const std::string& remoteAddr, const std::string& connType) = 0;
    virtual bool busSendMessage(const int32_t& display, const std::string& message) = 0;
    virtual bool busSetDebugLevel(const std::string& level) = 0;
    virtual bool busSetEncryptionInfo(const int32_t& display, const std::string& info) = 0;
    virtual bool busSetSessionDurationSec(const int32_t& display, const uint32_t& duration) = 0;
    virtual bool busSetSessionPolicy(const int32_t& display, const std::string& policy) = 0;
    virtual std::string busEncryptionInfo(const int32_t& display) = 0;
    virtual bool busDisplayResized(const int32_t& display, const uint16_t& width, const uint16_t& height) = 0;
    virtual bool busConnectorTerminated(const int32_t& display) = 0;
    virtual bool busConnectorSwitched(const int32_t& oldDisplay, const int32_t& newDisplay) = 0;
    virtual bool busConnectorAlive(const int32_t& display) = 0;
    virtual bool busCheckAuthenticate(const int32_t& display, const std::string& login, const std::string& password) = 0;
    virtual std::vector<sdbus::Struct<int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, std::string, std::string, std::string, std::string, std::string>> busGetSessions() = 0;
    virtual bool busRenderRect(const int32_t& display, const sdbus::Struct<int16_t, int16_t, uint16_t, uint16_t>& rect, const sdbus::Struct<uint8_t, uint8_t, uint8_t>& color, const bool& fill) = 0;
    virtual bool busRenderText(const int32_t& display, const std::string& text, const sdbus::Struct<int16_t, int16_t>& pos, const sdbus::Struct<uint8_t, uint8_t, uint8_t>& color) = 0;
    virtual bool busRenderClear(const int32_t& display) = 0;
    virtual bool helperIsAutoComplete(const int32_t& display) = 0;
    virtual bool helperIdleTimeoutAction(const int32_t& display) = 0;
    virtual int32_t helperGetIdleTimeoutSec(const int32_t& display) = 0;
    virtual std::string helperGetTitle(const int32_t& display) = 0;
    virtual std::string helperGetDateFormat(const int32_t& display) = 0;
    virtual std::vector<std::string> helperGetUsersList(const int32_t& display) = 0;
    virtual bool helperWidgetStartedAction(const int32_t& display) = 0;
    virtual bool helperSetSessionLoginPassword(const int32_t& display, const std::string& login, const std::string& password, const bool& action) = 0;

private:
    sdbus::IObject& object_;
};

}} // namespaces

#endif
