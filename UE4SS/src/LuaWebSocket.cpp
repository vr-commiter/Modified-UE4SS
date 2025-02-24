// WebSocket.cpp

#include <thread>
#include <mutex>
#include <queue>
#include <list>
#include <signal.h>
#include <iostream>
#include <fstream>
#ifdef _WIN32
#pragma comment( lib, "ws2_32" )
#include <WinSock2.h>
#endif
#include "LuaWebSocket.hpp"
#include <easywsclient.hpp>
using easywsclient::WebSocket;

static std::ofstream loggerfile("websock.txt");
namespace RC
{
LuaWebSocket::LuaWebSocket()
{
}

LuaWebSocket::~LuaWebSocket()
{
}


void LuaWebSocket::receive_message() {
    pollingMtx.lock();
    _webSocket->poll();
    _webSocket->dispatch([this](const std::string& message) {
        _message_str.push_back(message);
    });
    pollingMtx.unlock();
}

bool LuaWebSocket::init()
{
    if (_webSocket != nullptr)
    {
        return false;
    }
#ifdef _WIN32
    INT rc;
    WSADATA wsaData;
if (loggerfile.is_open())
      {
        loggerfile << "LuaWebSocket::init\n";
      }
    rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (rc) {
        printf("WSAStartup Failed.\n");
        return 1;
    }
#endif
    _url = "ws://127.0.0.1:18233/v1/tact";

    if (_url.empty())
        return false;

      
    _webSocket = WebSocket::from_url(_url);
    return true;
}

bool LuaWebSocket::isReady() const
{
    if (_webSocket == nullptr)
    {
        return false;
    }
    pollingMtx.lock();
    WebSocket::readyStateValues isClosed = ws->getReadyState();
    pollingMtx.unlock();
    
    return isClosed != WebSocket::CLOSED;
}

void LuaWebSocket::sendMessage(const std::string& message)
{
        pollingMtx.lock();
	_webSocket->send(message);
    _webSocket->poll();
        pollingMtx.unlock();
    if (loggerfile.is_open())
      {
        loggerfile << message.c_str();
      }
}

std::list<std::string> LuaWebSocket::receiveMessage(){
    return _message_str;
}


void LuaWebSocket::clearMessage(){
    _message_str.clear();
}

void LuaWebSocket::stop()
{
#ifdef _WIN32
    WSACleanup();
#endif
}
}