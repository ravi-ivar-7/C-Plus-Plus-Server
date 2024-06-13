#include <iostream>
#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "router.hpp"
#include "./system/sys_server_info.hpp"
#include "./utils/not_found.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

void Router::RouteRequest(std::shared_ptr<tcp::socket> socket)
{
    auto req = std::make_shared<http::request<http::string_body>>(); // to store parsed request
    auto buffer = std::make_shared<beast::flat_buffer>();            // temporary data storage

    http::async_read(*socket, *buffer, *req,
                     [socket, req, buffer](beast::error_code ec, std::size_t /*bytes_transfered*/)
                     {
                         if (!ec)
                         {
                             if (req->method() == http::verb::get && req->target().starts_with("/sys-server-info")) 
                                 sysServerInfo(std::move(*socket), std::move(*req));// in browser, http://localhost:7000/sys-server-info gives error (end of stream)






















                             else
                                 notFound(std::move(*socket), std::move(*req));
                         }
                         else
                         {
                             std::cerr << "Error reading request: " << ec.message() << std::endl;
                             socket->close();
                         }
                     });
}