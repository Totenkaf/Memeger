#include "request.h"
#include <boost/beast/http/write.hpp>
#include <boost/bind/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace server3 {
bool Request::req_method(const Request &req) {
  if (req.request_.method() == http::verb::get) {
    return true;
  } else {
    return false;
  }
}
} // namespace server3
