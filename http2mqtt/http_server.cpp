//
// Copyright (c) 2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

//------------------------------------------------------------------------------
//
// Example: HTTP server, small
//
//------------------------------------------------------------------------------

#include "http_server.h"

#include <memory>
#include <string>

namespace ip = boost::asio::ip;      // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;    // from <boost/asio.hpp>
namespace http = boost::beast::http; // from <boost/beast/http.hpp>

class HttpConnection : public std::enable_shared_from_this<HttpConnection> {
public:
  HttpConnection(tcp::socket socket, boost::asio::io_context &ioc)
      : socket_(std::move(socket)), ioc_{ioc} {}

  // Initiate the asynchronous operations associated with the connection.
  void Start(HttpServer::RequestHandler handler) {
    request_handler_ = std::move(handler);
    ReadRequest();
    CheckDeadline();
  }

  // Construct a response message based on the program state.
  void WriteResponse(const Response& response) {
    response_.version(request_.version());
    response_.keep_alive(false);
    response_.result(response.status);

    response_.set(http::field::content_type, "application/json");
    boost::beast::ostream(response_.body()) << response.body;
    response_.set(http::field::content_length, response_.body().size());

    http::async_write(socket_, response_,
                      [self = shared_from_this()](boost::beast::error_code ec, std::size_t) {
                        self->socket_.shutdown(tcp::socket::shutdown_send, ec);
                        self->deadline_.cancel();
                      });
  }

private:
  // The socket for the currently connected client.
  tcp::socket socket_;
  boost::asio::io_context &ioc_;

  // The buffer for performing reads.
  boost::beast::flat_buffer buffer_{8192};

  // The request message.
  http::request<http::string_body> request_;

  // The response message.
  http::response<http::dynamic_body> response_;

  // The timer for putting a deadline on connection processing.
  boost::asio::basic_waitable_timer<std::chrono::steady_clock> deadline_{
      ioc_, std::chrono::seconds(60)};

  HttpServer::RequestHandler request_handler_;

  // Asynchronously receive a complete request message.
  void ReadRequest() {
    http::async_read(
        socket_, buffer_, request_,
        [self = shared_from_this()](boost::beast::error_code ec, std::size_t bytes_transferred) {
          boost::ignore_unused(bytes_transferred);
          if (!ec)
            self->HandleRequest();
        });
  }

  // Determine what needs to be done with the request message.
  void HandleRequest() {
    if (request_handler_) {
      Request request;
      request.id = reinterpret_cast<unsigned long>(this);
      request.method = request_.method_string().to_string();
      request.target = request_.target().to_string();
      request.body = request_.body();
      request_handler_(request);
    }
  }

  // Check whether we have spent enough time on this connection.
  void CheckDeadline() {
    auto self = shared_from_this();

    deadline_.async_wait([self](boost::beast::error_code ec) {
      if (!ec) {
        // Close socket to cancel any outstanding operation.
        self->socket_.close(ec);
      }
    });
  }
};

HttpServer::HttpServer(const std::string& address, unsigned short port,
                       boost::asio::io_context &ioc)
    : ioc_{ioc}, acceptor_{ioc, {boost::asio::ip::make_address(address), port}}, socket_{ioc} {
  HandleRequest();
}

void HttpServer::HandleRequest() {
  acceptor_.async_accept(socket_, [&](boost::beast::error_code ec) {
    if (!ec) {
      auto connection = std::make_shared<HttpConnection>(std::move(socket_), ioc_);
      connections_[reinterpret_cast<unsigned long>(connection.get())] = connection;
      connection->Start(request_handler_);
    }

    CleanClosedConnections();
    HandleRequest();
  });
}

void HttpServer::CleanClosedConnections() {
  for (auto it = connections_.begin(); it != connections_.end(); ) {
    if (it->second.lock() == nullptr) {
      it = connections_.erase(it);
    } else {
      ++it;
    }
  }
}

void HttpServer::SetRequestHandler(HttpServer::RequestHandler request_handler) {
  request_handler_ = request_handler;
}

void HttpServer::PostResponse(const Response& response) {
  auto it = connections_.find(response.request_id);
  if (it != connections_.end()) {
    auto connection = it->second.lock();
    if (connection) {
      connection->WriteResponse(response);
    }
  }
}
