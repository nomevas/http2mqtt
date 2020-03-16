#pragma once

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/http.hpp>

#include <map>

using SessionID = unsigned long;
class HttpConnection;
using Input = boost::beast::http::request<boost::beast::http::string_body>;
using Output = boost::beast::http::response<boost::beast::http::string_body>;

class HttpServer final {
public:
  using RequestHandler = std::function<void(SessionID session_id, const Input &)>;

  HttpServer(const std::string& address, unsigned short port,
      boost::asio::io_context &ioc);

  void SetRequestHandler(RequestHandler request_handler);
  void PostResponse(SessionID session_id, const Output & response);

private:
  void HandleRequest();
  void CleanClosedConnections();

  boost::asio::io_context &ioc_;
  boost::asio::ip::tcp::acceptor acceptor_;
  boost::asio::ip::tcp::socket socket_;
  HttpServer::RequestHandler request_handler_;
  std::map<SessionID, std::weak_ptr<HttpConnection>> connections_;
};