#ifndef FW_NET_DECLARATIONS_H
#define FW_NET_DECLARATIONS_H

#include "../pch.h"

namespace FW::Net {
class Protocol;
class OutputMessage;
class InputMessage;
class Connection;
class TcpListener;
class TcpClient;
class Manager;

typedef std::shared_ptr<Protocol> Protocol_ptr;
typedef std::shared_ptr<OutputMessage> OutputMessage_ptr;
typedef std::shared_ptr<InputMessage> InputMessage_ptr;
typedef std::shared_ptr<Connection> Connection_ptr;
typedef std::weak_ptr<Connection> Connection_wptr;
typedef std::shared_ptr<TcpListener> TcpListener_ptr;
typedef std::shared_ptr<TcpClient> TcpClient_ptr;
typedef std::shared_ptr<Manager> Manager_ptr;
}  // namespace FW::Net
#endif  // #ifndef FW_NET_DECLARATIONS_H