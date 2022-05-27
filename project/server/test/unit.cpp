#include <gtest/gtest.h>
#include"async.http.server.h"
#include"response.h"
 #include"models.h"
#include"postgresql_db.h"
#include"request.h"
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <thread>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/lexical_cast.hpp>
using namespace server3;
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
using json = nlohmann::json;

// Postgre_DB database;
// database.drop_tables();
 
 class DataBaseEnvironment : public ::testing::Test {
public:
  //void TearDown() override { database.drop_tables(); }
  Postgre_DB database =
      Postgre_DB("127.0.0.1", "5432", "postgres", "postgres", "postgres");
  virtual ~DataBaseEnvironment() {}
};

TEST_F(DataBaseEnvironment,check_method_post)
{
    
    Request obj_request;
    obj_request.request_.method(http::verb::post);
    bool check=obj_request.req_method(obj_request);
    EXPECT_EQ(check,false);

}
TEST_F(DataBaseEnvironment,check_method_get)
{
    Request obj_request;
    obj_request.request_.method(http::verb::get);
    bool check=obj_request.req_method(obj_request);
    EXPECT_EQ(check,true);

}
TEST_F(DataBaseEnvironment, check_user)
 {
 
   
   std::string user_login="TOM";
    std::string chat_name="Monolog";
     std::string content="Privet";
     
     Request obj_request;
    Response obj_response; 
	std::string body = "{\"login\":\"" + user_login + "\"," + "\"chat_name\":\"" + chat_name + "\"," + "\"content\":\"" +content+"\"}";
    
    std::string path="/check_login/";
 obj_request.request_.method(http::verb::get);
 obj_request.request_.target(path);
obj_request.request_.body() = body;


obj_response.res={http::status::bad_request,  obj_request.request_.version()};
                
                 obj_request.request_str =  obj_request.request_.target().to_string();

                // std::string &request_str=obj_request.request_str;
                // boost::beast::http::request<boost::beast::http::string_body> &request_=obj_request.request_;
                // boost::beast::http::response<boost::beast::http::string_body> &res=obj_response.res;
                // std::string &response_body=obj_response.response_body;
                 

     
bool check=obj_response.usr.check_login(obj_request,obj_response,database);
 
EXPECT_EQ(check,false);
 
  
}
TEST_F(DataBaseEnvironment, add_user)
 {
//   std::size_t num_threads = boost::lexical_cast<std::size_t>(4);
//   auto s = std::make_shared<DataBaseEnvironment3::DataBaseEnvironment>("localhost", "8080", num_threads);

//   s->run();
    Request obj_request;
    Response obj_response; 

   std::string user_login="Jerry";
    std::string user_password="rwfewe";
    
     
	std::string body = "{\"login\":\"" + user_login + "\"," + "\"password\":\"" + user_password +  "\"}";
     
     
	
    
    std::string path="/registration/";
 obj_request.request_.method(http::verb::post);
 obj_request.request_.target(path);
obj_request.request_.body() = body;


obj_response.res={http::status::bad_request,  obj_request.request_.version()};
                
                 obj_request.request_str =  obj_request.request_.target().to_string();

                // std::string &request_str=obj_request.request_str;
                // boost::beast::http::request<boost::beast::http::string_body> &request_=obj_request.request_;
                // boost::beast::http::response<boost::beast::http::string_body> &res=obj_response.res;
                // std::string &response_body=obj_response.response_body;
                 

     
bool check=obj_response.usr.registration(obj_request,obj_response,database);
 
EXPECT_EQ(check,true);
 
  
}

TEST_F(DataBaseEnvironment, check_login)
 {
//   std::size_t num_threads = boost::lexical_cast<std::size_t>(4);
//   auto s = std::make_shared<DataBaseEnvironment3::DataBaseEnvironment>("localhost", "8080", num_threads);

//   s->run();
    Request obj_request;
    Response obj_response; 

   std::string user_login="Jerry";
     
    
     
	std::string body = "{\"login\":\"" + user_login +"\"}";
     
     
	
    
    std::string path="/check_login/";
 obj_request.request_.method(http::verb::get);
 obj_request.request_.target(path);
obj_request.request_.body() = body;


obj_response.res={http::status::bad_request,  obj_request.request_.version()};
                
                 obj_request.request_str =  obj_request.request_.target().to_string();

                // std::string &request_str=obj_request.request_str;
                // boost::beast::http::request<boost::beast::http::string_body> &request_=obj_request.request_;
                // boost::beast::http::response<boost::beast::http::string_body> &res=obj_response.res;
                // std::string &response_body=obj_response.response_body;
                 

     
bool check=obj_response.usr.check_login(obj_request,obj_response,database);
 
EXPECT_EQ(check,true);
 
  
}






TEST_F(DataBaseEnvironment, change_login)
 {
//   std::size_t num_threads = boost::lexical_cast<std::size_t>(4);
//   auto s = std::make_shared<DataBaseEnvironment3::DataBaseEnvironment>("localhost", "8080", num_threads);

//   s->run();
    Request obj_request;
    Response obj_response; 

   std::string user_login="Jerry";
     
     
    std::string new_user_login="TOM";

     std::string status="active";
	std::string body = "{\"login\":\"" + user_login + "\","  +"\"new_log\":\"" +new_user_login+"\"}";
     
     
	
    
    std::string path="/change_login/";
 obj_request.request_.method(http::verb::post);
 obj_request.request_.target(path);
obj_request.request_.body() = body;


obj_response.res={http::status::bad_request,  obj_request.request_.version()};
                
                 obj_request.request_str =  obj_request.request_.target().to_string();

                // std::string &request_str=obj_request.request_str;
                // boost::beast::http::request<boost::beast::http::string_body> &request_=obj_request.request_;
                // boost::beast::http::response<boost::beast::http::string_body> &res=obj_response.res;
                // std::string &response_body=obj_response.response_body;
                 

     
bool check=obj_response.usr.change_login(obj_request,obj_response,database);
 
EXPECT_EQ(check,true);
 
  
}





TEST_F(DataBaseEnvironment, change_password)
 {
//   std::size_t num_threads = boost::lexical_cast<std::size_t>(4);
//   auto s = std::make_shared<DataBaseEnvironment3::DataBaseEnvironment>("localhost", "8080", num_threads);

//   s->run();

    Request obj_request;
    Response obj_response; 

   std::string user_login="TOM";
    std::string user_password="rwfewe";
    
    std::string new_user_pass="123";
    
	std::string body = "{\"login\":\"" + user_login + "\","  +"\"new_pass\":\"" +new_user_pass+"\"}";
     
	
    
    std::string path="/change_password/";
 obj_request.request_.method(http::verb::post);
 obj_request.request_.target(path);
obj_request.request_.body() = body;


obj_response.res={http::status::bad_request,  obj_request.request_.version()};
                
                 obj_request.request_str =  obj_request.request_.target().to_string();

                // std::string &request_str=obj_request.request_str;
                // boost::beast::http::request<boost::beast::http::string_body> &request_=obj_request.request_;
                // boost::beast::http::response<boost::beast::http::string_body> &res=obj_response.res;
                // std::string &response_body=obj_response.response_body;
                 

     
bool check=obj_response.usr.change_password(obj_request,obj_response,database);
 
EXPECT_EQ(check,true);
 
  
}




TEST_F(DataBaseEnvironment, change_user_status)
 {
//   std::size_t num_threads = boost::lexical_cast<std::size_t>(4);
//   auto s = std::make_shared<DataBaseEnvironment3::DataBaseEnvironment>("localhost", "8080", num_threads);

//   s->run();
    Request obj_request;
    Response obj_response; 

  std::string user_login="TOM";
    std::string status="inactive";
	std::string body = "{\"login\":\"" + user_login + "\","  +"\"status\":\"" +status+"\"}";
     
	
    
    std::string path="/change_user_status/";
 obj_request.request_.method(http::verb::post);
 obj_request.request_.target(path);
obj_request.request_.body() = body;


obj_response.res={http::status::bad_request,  obj_request.request_.version()};
                
                 obj_request.request_str =  obj_request.request_.target().to_string();

                // std::string &request_str=obj_request.request_str;
                // boost::beast::http::request<boost::beast::http::string_body> &request_=obj_request.request_;
                // boost::beast::http::response<boost::beast::http::string_body> &res=obj_response.res;
                // std::string &response_body=obj_response.response_body;
                 

     
bool check=obj_response.usr.change_user_status(obj_request,obj_response,database);
 
EXPECT_EQ(check,true);
 
  
}



TEST_F(DataBaseEnvironment, add_chat)
 {
//   std::size_t num_threads = boost::lexical_cast<std::size_t>(4);
//   auto s = std::make_shared<DataBaseEnvironment3::DataBaseEnvironment>("localhost", "8080", num_threads);

//   s->run();
    Request obj_request;
    Response obj_response; 

  std::string chat_name="ULTRA";
    std::vector<std::string> participants(1);//укажите количество динамически
    participants[0]="TOM";
      
     int size=participants.size();
    
    std::string body = "{\"chat_name\":\"" + chat_name+ "\"," + "\"participant_1\":\"" + participants[0] + "\"," +    "\"size\":\"" + std::to_string(size) + "\"}";
     
	
    
    std::string path="/add_chat/";
 obj_request.request_.method(http::verb::post);
 obj_request.request_.target(path);
obj_request.request_.body() = body;


obj_response.res={http::status::bad_request,  obj_request.request_.version()};
                
                 obj_request.request_str =  obj_request.request_.target().to_string();

                // std::string &request_str=obj_request.request_str;
                // boost::beast::http::request<boost::beast::http::string_body> &request_=obj_request.request_;
                // boost::beast::http::response<boost::beast::http::string_body> &res=obj_response.res;
                // std::string &response_body=obj_response.response_body;
                 

     
bool  check = obj_response.cht.add_chat(obj_request,obj_response,database);
 
EXPECT_EQ(check,true);
 
  
}



TEST_F(DataBaseEnvironment, change_chat_name)
 {

    Request obj_request;
    Response obj_response; 

      std::string chat_name="ULTRA";
   std::string  new_chat_name={"Ultra"};
      
    
    std::string body = "{\"chat_name\":\"" + chat_name+ "\"," + "\"new_chat_name\":\"" + new_chat_name+   "\"}";
	
    
    std::string path="/change_chat_name/";
 obj_request.request_.method(http::verb::post);
 obj_request.request_.target(path);
obj_request.request_.body() = body;


obj_response.res={http::status::bad_request,  obj_request.request_.version()};
                
                 obj_request.request_str =  obj_request.request_.target().to_string();

                // std::string &request_str=obj_request.request_str;
                // boost::beast::http::request<boost::beast::http::string_body> &request_=obj_request.request_;
                // boost::beast::http::response<boost::beast::http::string_body> &res=obj_response.res;
                // std::string &response_body=obj_response.response_body;
                 

     
bool  check = obj_response.cht.change_chat_name(obj_request,obj_response,database);
 
EXPECT_EQ(check,true);
 
  
}

TEST_F(DataBaseEnvironment, add_message)
 {
//   std::size_t num_threads = boost::lexical_cast<std::size_t>(4);
//   auto s = std::make_shared<DataBaseEnvironment3::DataBaseEnvironment>("localhost", "8080", num_threads);

//   s->run();

    Request obj_request;
    Response obj_response; 

     std::string user_login="TOM";
    std::string chat_name="Ultra";
     std::string content="Privet";
     
	std::string body = "{\"login\":\"" + user_login + "\"," + "\"chat_name\":\"" + chat_name + "\"," + "\"content\":\"" +content+"\"}";
	
    
    std::string path="/add_message/";
 obj_request.request_.method(http::verb::post);
 obj_request.request_.target(path);
obj_request.request_.body() = body;


obj_response.res={http::status::bad_request,  obj_request.request_.version()};
                
                 obj_request.request_str =  obj_request.request_.target().to_string();

                // std::string &request_str=obj_request.request_str;
                // boost::beast::http::request<boost::beast::http::string_body> &request_=obj_request.request_;
                // boost::beast::http::response<boost::beast::http::string_body> &res=obj_response.res;
                // std::string &response_body=obj_response.response_body;
                 

     
bool  check = obj_response.msg.add_message(obj_request,obj_response,database);
 
EXPECT_EQ(check,true);
 
  
}




TEST_F(DataBaseEnvironment, delete_message)
 {
//   std::size_t num_threads = boost::lexical_cast<std::size_t>(4);
//   auto s = std::make_shared<DataBaseEnvironment3::DataBaseEnvironment>("localhost", "8080", num_threads);

//   s->run();

    Request obj_request;
    Response obj_response; 
std::string user_login="TOM";
    std::string chat_name="Ultra";
    std::string content="Privet";
     
	std::string body = "{\"login\":\"" + user_login + "\"," + "\"chat_name\":\"" + chat_name + "\"," + "\"content\":\"" +content+ "\"}";
	
    
    std::string path="/delete_message/";
 obj_request.request_.method(http::verb::post);
 obj_request.request_.target(path);
obj_request.request_.body() = body;


obj_response.res={http::status::bad_request,  obj_request.request_.version()};
                
                 obj_request.request_str =  obj_request.request_.target().to_string();

                // std::string &request_str=obj_request.request_str;
                // boost::beast::http::request<boost::beast::http::string_body> &request_=obj_request.request_;
                // boost::beast::http::response<boost::beast::http::string_body> &res=obj_response.res;
                // std::string &response_body=obj_response.response_body;
                 

     
bool  check = obj_response.msg.delete_message(obj_request,obj_response,database);
 
EXPECT_EQ(check,false);
 
  
}





TEST_F(DataBaseEnvironment, delete_chat)
 {
//   std::size_t num_threads = boost::lexical_cast<std::size_t>(4);
//   auto s = std::make_shared<DataBaseEnvironment3::DataBaseEnvironment>("localhost", "8080", num_threads);

//   s->run();

    Request obj_request;
    Response obj_response; 

  std::string chat_name="Ultra";
    std::vector<std::string> participants(1);//укажите количество динамически
    participants[0]="TOM";
      
     int size=participants.size();
    
    std::string body = "{\"chat_name\":\"" + chat_name+ "\"," + "\"participant_1\":\"" + participants[0] + "\"," +    "\"size\":\"" + std::to_string(size) + "\"}";
     
	
    
    std::string path="/delete_chat/";
 obj_request.request_.method(http::verb::post);
 obj_request.request_.target(path);
obj_request.request_.body() = body;


obj_response.res={http::status::bad_request,  obj_request.request_.version()};
                
                 obj_request.request_str =  obj_request.request_.target().to_string();

                // std::string &request_str=obj_request.request_str;
                // boost::beast::http::request<boost::beast::http::string_body> &request_=obj_request.request_;
                // boost::beast::http::response<boost::beast::http::string_body> &res=obj_response.res;
                // std::string &response_body=obj_response.response_body;
                 

     
bool  check = obj_response.cht.delete_chat(obj_request,obj_response,database);
 
EXPECT_EQ(check,true);
 
  
}


TEST_F(DataBaseEnvironment, delete_user)
 {
//   std::size_t num_threads = boost::lexical_cast<std::size_t>(4);
//   auto s = std::make_shared<DataBaseEnvironment3::DataBaseEnvironment>("localhost", "8080", num_threads);

//   s->run();

    Request obj_request;
    Response obj_response; 

   std::string user_login="TOM";
    
    std::string body = "{\"login\":\"" + user_login +"\"}";
     
     
	
    
    std::string path="/delete_user/";
 obj_request.request_.method(http::verb::post);
 obj_request.request_.target(path);
obj_request.request_.body() = body;


obj_response.res={http::status::bad_request,  obj_request.request_.version()};
                
                 obj_request.request_str =  obj_request.request_.target().to_string();

                // std::string &request_str=obj_request.request_str;
                // boost::beast::http::request<boost::beast::http::string_body> &request_=obj_request.request_;
                // boost::beast::http::response<boost::beast::http::string_body> &res=obj_response.res;
                // std::string &response_body=obj_response.response_body;
                 

     
bool check=obj_response.usr.delete_user(obj_request,obj_response,database);
 
EXPECT_EQ(check,true);
  
  
}
int main(int argc, char **argv) 
{
     
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
//database.drop_tables();