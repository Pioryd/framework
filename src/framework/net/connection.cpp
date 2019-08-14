#include "connection.h"

#include "../core/logger.h"
#include "../global.h"

#include "tcplistener.h"

namespace FW::Net {
Connection::Connection(boost::asio::io_service& io_service,
                       std::function<void(Connection_ptr&)> on_close,
                       std::function<void(Connection_ptr&)> on_connected,
                       const PacketParseCallbacks_ptr& packet_parse_callbacks)
    : id_{-1},
      socket_{io_service},
      read_timer_{io_service},
      write_timer_{io_service},
      on_close{on_close},
      on_connected{on_connected},
      packet_parse_callbacks_{packet_parse_callbacks},
      state_{State::OPEN},
      out_msg{std::make_shared<OutputMessage>()} {}

Connection::~Connection() {
  Connection::auto_id.release(id_);
  close();
}

void Connection::init() { id_ = Connection::auto_id.lock(shared_from_this()); }

void Connection::connect(
    boost::asio::ip::basic_resolver<boost::asio::ip::tcp>::iterator
        endpoint_iterator) {
  std::lock_guard<std::recursive_mutex> lock(mutex_);

  if (state_ == State::CLOSED) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        get_info() + " Unable to connect. Connection is closed.");)
    return;
  }

  if (state_ == State::CONNECTING || state_ == State::CONNECTED) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        get_info() + " Unable to connect. Connection is connecting/connected.");)
    return;
  }

  state_ = State::CONNECTING;

  try {
    read_timer_.cancel();
    read_timer_.expires_from_now(std::chrono::seconds(config.read_timeout));
    read_timer_.async_wait(std::bind(&Connection::on_timeout, shared_from_this(),
                                    std::placeholders::_1));

    socket_.async_connect(*endpoint_iterator,
                          std::bind(&Connection::on_connect, shared_from_this(),
                                    std::placeholders::_1));
  } catch (boost::system::system_error& e) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        get_info() + " Unable to connect. Error: " + std::string(e.what()) +
        ".");)
    close();
  }
}

void Connection::close() {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (state_ == State::CLOSED) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        get_info() + " Unable to close connection. Connection is closed.");)
    return;
  }

  auto this_ptr = shared_from_this();
  on_close(this_ptr);

  if (socket_.is_open()) {
    try {
      read_timer_.cancel();
      write_timer_.cancel();
      socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
      socket_.close();
    } catch (boost::system::system_error& e) {
      G::Logger.error(get_info() +
                      " Unable to correctly close connection. Error: " +
                      std::string(e.what()) + ".");
    }
  }

  state_ = State::CLOSED;
}

void Connection::send() {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  if (state_ != State::CONNECTED) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        get_info() + " Unable to send. Connection is NOT connected.");)
    return;
  }

  bool isSocketWritingPackets = output_message_queue_.empty();
  output_message_queue_.emplace_back(out_msg);
  if (isSocketWritingPackets) { write_packet(out_msg); }
  out_msg = std::make_shared<OutputMessage>();
}

unsigned long Connection::get_host() {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  try {
    return socket_.remote_endpoint().address().to_v4().to_ulong();
  } catch (boost::system::system_error& e) {
    G::Logger.error("Unable to get ip. Error: " + std::string(e.what()));
  }
  return 0;
}

std::string Connection::get_host_as_string() {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  try {
    return socket_.remote_endpoint().address().to_string();
  } catch (boost::system::system_error& e) {
    G::Logger.error("Unable to get ip. Error: " + std::string(e.what()));
  }
  return "";
}

unsigned short Connection::get_port() {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  try {
    return socket_.remote_endpoint().port();
  } catch (boost::system::system_error& e) {
    G::Logger.error("Unable to get ip. Error: " + std::string(e.what()));
  }
  return 0;
}

std::string Connection::get_port_as_string() {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  return std::to_string(get_port());
}

std::string Connection::get_info() {
  // TODO BUG
  return std::string("std::string Connection::get_info()"
    /*"Connection[Host: " + get_host_as_string() + " Port: " +
                     get_port_as_string() + 
                     " Protocol: " + protocol_->name + "]"*/);
}

void Connection::read_packet() {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  try {
    read_timer_.cancel();

    if (state_ != State::CONNECTED) {
      FW_DEBUG_INSTRUCTIONS(G::Logger.error(
          get_info() + " Unable to read packet. Connection is NOT connected.");)
      return;
    }

    read_timer_.expires_from_now(std::chrono::seconds(config.read_timeout));
    read_timer_.async_wait(std::bind(&Connection::on_timeout, shared_from_this(),
                                    std::placeholders::_1));

    in_msg.reset();
    in_msg.set_length(NetworkMessage::PACKET_SIZE_BYTES_LENGTH);

    boost::asio::async_read(
        socket_,
        boost::asio::buffer(in_msg.get_buffer_at_current_position(),
                            NetworkMessage::PACKET_SIZE_BYTES_LENGTH),
        std::bind(&Connection::read_packet_size, shared_from_this(),
                  std::placeholders::_1));
  } catch (boost::system::system_error& e) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        get_info() + " Unable to read packet. Error: " + std::string(e.what()) +
        ".");)
    close();
  }
}

void Connection::read_packet_size(const boost::system::error_code& error) {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  try {
    read_timer_.cancel();

    if (state_ != State::CONNECTED) {
      FW_DEBUG_INSTRUCTIONS(G::Logger.error(
          get_info() +
          " Unable to read packet size. Connection is NOT connected.");)
      return;
    }

    if (error) {
      FW_DEBUG_INSTRUCTIONS(G::Logger.error(
          get_info() + " Unable to read packet size. Error: " + error.message() +
          ".");)
      close();
      return;
    }

    in_msg.set_length(in_msg.peek_uint16());
    if (in_msg.get_length() == 0 ||
        in_msg.get_length() >= NetworkMessage::MAX_PACKET_BODY_LENGTH) {
      FW_DEBUG_INSTRUCTIONS(G::Logger.error(
          get_info() +
          " Wrong packet size: " + std::to_string(in_msg.get_length()) + ".");)
      close();
      return;
    }

    read_timer_.expires_from_now(std::chrono::seconds(config.read_timeout));
    read_timer_.async_wait(std::bind(&Connection::on_timeout, shared_from_this(),
                                    std::placeholders::_1));

    boost::asio::async_read(
        socket_,
        boost::asio::buffer(in_msg.get_buffer_at_current_position(),
                            in_msg.get_length() - in_msg.get_buffer_position()),
        std::bind(&Connection::read_packet_body, shared_from_this(),
                  std::placeholders::_1));
  } catch (boost::system::system_error& e) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        get_info() +
        " Unable to read packet size. Error: " + std::string(e.what()));)
    close();
  }
}

void Connection::read_packet_body(const boost::system::error_code& error) {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  try {
    read_timer_.cancel();
  } catch (boost::system::system_error& e) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        get_info() +
        " Unable to read packet body. Error: " + std::string(e.what()));)
    close();
  }

  if (error) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        get_info() + " Unable to parse packet. Error: " + error.message() +
        ".");)
    close();
    return;
  }

  if (state_ != State::CONNECTED) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        get_info() +
        " Unable to read packet body. Connection is NOT connected.");)
    return;
  }

  parse_packet();

  read_packet();
}

void Connection::write_packet(const OutputMessage_ptr& msg) {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  try {
    write_timer_.cancel();

    if (state_ != State::CONNECTED) {
      FW_DEBUG_INSTRUCTIONS(G::Logger.error(
          get_info() + " Unable to write packet. Connection is NOT connected.");)
      return;
    }

    prepare_to_write(msg);

    write_timer_.expires_from_now(std::chrono::seconds(config.write_timeout));
    write_timer_.async_wait(std::bind(&Connection::on_timeout, shared_from_this(),
                                     std::placeholders::_1));

    boost::asio::async_write(
        socket_, boost::asio::buffer(msg->get_output_buffer(), msg->get_length()),
        std::bind(&Connection::on_write_packet, shared_from_this(),
                  std::placeholders::_1));
  } catch (boost::system::system_error& e) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        get_info() +
        " Unable to write packet. Error: " + std::string(e.what()));)
    close();
  }
}

void Connection::on_connect(const boost::system::error_code& error) {
  try {
    read_timer_.cancel();
  } catch (boost::system::system_error& e) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        get_info() +
        " Unable to handle connect. Error: " + std::string(e.what()));)
    close();
  }

  if (error == boost::asio::error::operation_aborted) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        get_info() + " Unable to handle connect. Operation aborted.");)
    return;
  }

  if (error) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        get_info() + " Unable to handle connect. Error: " + error.message() +
        ".");)
    close();
    return;
  }

  boost::asio::ip::tcp::no_delay option(true);
  socket_.set_option(option);
  state_ = State::CONNECTED;
  auto this_connection = shared_from_this();
  on_connected(this_connection);
  read_packet();
}

void Connection::on_accept() {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  state_ = State::CONNECTED;
  auto this_connection = shared_from_this();
  on_connected(this_connection);
  read_packet();
}

void Connection::on_write_packet(const boost::system::error_code& error) {
  std::lock_guard<std::recursive_mutex> lock(mutex_);
  write_timer_.cancel();
  output_message_queue_.pop_front();

  if (error) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        get_info() + " Error while writing packet. Error: " + error.message() +
        ".");)
    output_message_queue_.clear();
    close();
    return;
  }

  if (state_ != State::CONNECTED) {
    FW_DEBUG_INSTRUCTIONS(G::Logger.error(
        get_info() +
        " Unable to handle write packet. Connection is NOT connected.");)
    return;
  }

  if (!output_message_queue_.empty()) write_packet(output_message_queue_.front());
}

void Connection::on_timeout(const boost::system::error_code& error) {
  if (error == boost::asio::error::operation_aborted) {
    // FW_DEBUG_INSTRUCTIONS(
    //    G::Logger.info(get_info() + " Timeout. Operation aborted.");)
    return;
  }

  FW_DEBUG_INSTRUCTIONS(
      G::Logger.error(get_info() + " Timeout. Error: " + error.message() + ".");)
  close();
}

void Connection::prepare_to_write(const OutputMessage_ptr& msg) const {
  msg->write_message_length();
}

void Connection::parse_packet() {
  uint8_t packet_id = in_msg.get_uint8();
  auto this_connection = shared_from_this();

  try {
    Connection::packet_parse_callbacks_->call(packet_id, this_connection);
  } catch (std::exception e) {
    FW::G::Logger.error("Unable to parse packet[" + std::to_string(packet_id) +
                        "]. Error " + std::string(e.what()));
    close();
  }
}

boost::asio::ip::tcp::socket& Connection::getSocket() { return socket_; }

int32_t Connection::get_id() { return id_; }

Connection::State Connection::get_state() { return state_; }
}  // namespace FW::Net
