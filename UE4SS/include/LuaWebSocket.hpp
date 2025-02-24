// WebSocket.hpp
#pragma once

#include <easywsclient.hpp>
//#include "easywsclient.cpp" // <-- include only if you don't want compile separately

#include <assert.h>
#include <stdio.h>
#include <string>

#include <LuaMadeSimple/LuaMadeSimple.hpp>
#include <LuaMadeSimple/LuaObject.hpp>
using easywsclient::WebSocket;
namespace RC
{
    class UE4SSProgram;

    namespace Unreal
    {
        class UClass;
    }


    class LuaWebSocket
    {
public:    
	LuaWebSocket();
    virtual ~LuaWebSocket();
    std::list<std::string> receiveMessage();
    void clearMessage();

    bool init();
    void sendMessage(const std::string& text);
    void stop();
    bool isReady() const;
    void receive_message();
    WebSocket::pointer getSocket()
    {
        return _webSocket;
    }
private:
    std::list<std::string> _message_str;
    WebSocket::pointer _webSocket;

    std::string _url;
    int      status;
    };
}