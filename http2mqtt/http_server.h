#pragma once

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/http.hpp>

#include <map>

using RequestID = unsigned long;
using Method = std::string;
using Target = std::string;
using Headers = std::map<std::string, std::string>;
using Body = std::string;
using HttpStatusCode = unsigned;
class HttpConnection;

struct Request {
  RequestID id;
  Method method;
  Target target;
  std::map<std::string, std::string> headers;
  Body body;
};

struct Response {
  RequestID request_id;
  HttpStatusCode status;
  Body body;
};

class HttpServer final {
public:
  using RequestHandler = std::function<void(const Request&)>;

  HttpServer(const std::string& address, unsigned short port,
      boost::asio::io_context &ioc);

  void SetRequestHandler(RequestHandler request_handler);
  void PostResponse(const Response& response);

private:
  void HandleRequest();
  void CleanClosedConnections();

  boost::asio::io_context &ioc_;
  boost::asio::ip::tcp::acceptor acceptor_;
  boost::asio::ip::tcp::socket socket_;
  HttpServer::RequestHandler request_handler_;
  std::map<RequestID, std::weak_ptr<HttpConnection>> connections_;
};