#include <vector>
#include <boost/bind/bind.hpp>
#include <boost/beast/http/write.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include"request.h"
#include <string>
#include <sstream>
 

namespace server3 
{

 bool Request:: req_method(const Request& req)
{
    if(req.request_.method()==http::verb::get){return true;}
    else return false;
}


}