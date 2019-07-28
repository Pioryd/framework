#ifndef FW_NET_PROTOCOL_H
#define FW_NET_PROTOCOL_H

#include "../crypt/crypt.h"
#include "../crypt/rsa.h"
#include "connection.h"

namespace FW::Net {
class Protocol : public std::enable_shared_from_this<Protocol> {
 public:
  enum class AutoSend { ENABLED, DISABLED, TERMINATED };

 public:
  Protocol(const std::string& name,
           std::function<void(Protocol_ptr&)> onConnected,
           std::function<void(Protocol_ptr&, InputMessage&)> onPrsePacket);
  virtual ~Protocol() = default;

  Protocol(const Protocol&) = delete;
  Protocol& operator=(const Protocol&) = delete;

  void setConnection(const Connection_ptr& connection);

  void prepareToWrite(const OutputMessage_ptr& msg) const;
  void onRecvMessage(InputMessage& msg);

  Connection_ptr getConnection() const;

  void send(const OutputMessage_ptr& msg) const;

  void disconnect() const;

 public:
  std::function<void(Protocol_ptr&)> onConnected;
  std::function<void(Protocol_ptr&)> onRelease;
  std::function<void(Protocol_ptr&, InputMessage&)> onPrsePacket;

 private:
  std::string name_;
  Connection_wptr connection_;

 private:
  friend class Connection;
};
}  // namespace FW::Net
#endif  // FW_NET_PROTOCOL_H
