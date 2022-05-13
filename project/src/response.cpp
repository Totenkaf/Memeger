#include <vector>
#include <boost/bind/bind.hpp>
#include <boost/beast/http/write.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include"../include/response.h"
#include <string>
#include <sstream>
 #include"../include/request.h"
#include"../DB/include/models.h"
#include"../DB/include/postgresql_db.h"
namespace server3 
{

 

void Not_Found(Response &resp)
{
resp.res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
resp.res.set(http::field::content_type, "application/json");
resp.res.set(http::field::connection, "Keep-Alive");
resp.res.result(http::status::not_found);
resp.res.body() = "{\"page\": \"Not found\", \"reason\" : \"wrong URI\" }";
}


void Send_Response(Response &resp ,std::string url,std::string status)
{
    resp.res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    resp.res.version(10);
    resp.res.set(http::field::location, url + status);
    resp.res.set(http::field::content_type, "application/json; charset=UTF-8");
    resp.res.set(http::field::connection, "Keep-Alive");
    resp.res.result(http::status::ok);
    resp.response_body = resp.j_response.dump();
    resp.res.body() = resp.response_body;
    resp.res.set(http::field::content_length, std::to_string(resp.response_body.length()));
}
 

//USERS GET

bool USER:: check_login(Request &req,Response &resp ,Postgre_DB &database)
{
    
req.j_req = json::parse(req.request_.body());
bool check = database.find_user_by_login(req.j_req["login"]);

resp.j_response["check_login"] = check;

if(!check)
{
Not_Found(resp);
return false;
}

else
{
 Send_Response(resp,"/user/",std::to_string(check));
// resp.res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
// resp.res.version(10);
// resp.res.set(http::field::location, "/user/" + std::to_string(check));
// resp.res.set(http::field::content_type, "application/json; charset=UTF-8");
// resp.res.set(http::field::connection, "Keep-Alive");
// resp.res.result(http::status::ok);
// resp.response_body = resp.j_response.dump();
// resp.res.body() = resp.response_body;
//resp.res.set(http::field::content_length, std::to_string(resp.response_body.length()));
return true;
}

}
bool USER::get_user(Request &req,Response &resp ,Postgre_DB &database)
{
    req.j_req = json::parse(req.request_.body());
    User usr = database.get_user_by_login(req.j_req["login"]);

     
        
    resp.j_response["id"] = usr.get_id();
    resp.j_response["login"] = usr.get_login();
    resp.j_response["password"] = usr.get_password();
    resp.j_response["status"] = usr.get_active_status();

    if(usr.get_login()== std::string())
    {
    Not_Found(resp);
    return false;
    }

    else
    {
        Send_Response(resp,"/user/",usr.get_active_status());
    // resp.res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    // resp.res.version(10);
    // resp.res.set(http::field::location, "/user/" + usr.get_active_status());
    // resp.res.set(http::field::content_type, "application/json; charset=UTF-8");
    // resp.res.set(http::field::connection, "Keep-Alive");
    // resp.res.result(http::status::ok);
    // resp.response_body = resp.j_response.dump();
    // resp.res.body() = resp.response_body;
    // resp.res.set(http::field::content_length, std::to_string(resp.response_body.length()));
    return true;
    }
}

//CHATS GET


bool CHAT::get_chat_by_name(Request &req,Response &resp ,Postgre_DB &database)
{
     req.j_req = json::parse(req.request_.body());
    
    Chat chat = database.get_chat_by_chat_name(req.j_req["chat_name"]);
    
    
    
    resp.j_response["chat_id_"] = chat.get_chat_id();
    resp.j_response["chat_name"] = chat.get_chat_name();
    
    bool check = database.find_chat_by_chat_name(req.j_req["chat_name"]) ;
    if( !database.find_chat_by_chat_name(req.j_req["chat_name"])   || chat.get_chat_name()== std::string()  )
    {
    Not_Found(resp);
    return false;
    }
    
    else
    {
         Send_Response(resp,"/chat/",std::to_string(check));
    // resp.res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    // resp.res.version(10);
    // resp.res.set(http::field::location, "/chat/" + std::to_string(check));
    // resp.res.set(http::field::content_type, "application/json; charset=UTF-8");
    // resp.res.set(http::field::connection, "Keep-Alive");
    // resp.res.result(http::status::ok);
    // resp.response_body = resp.j_response.dump();
    // resp.res.body() = resp.response_body;
    // resp.res.set(http::field::content_length, std::to_string(resp.response_body.length()));
    return true;
    }
}

bool CHAT::get_participants_from_chat(Request &req,Response &resp ,Postgre_DB &database)
{
      req.j_req = json::parse(req.request_.body());
                        
    Chat chat = database.get_chat_by_chat_name(req.j_req["chat_name"]);
    participants=chat.get_participants();
    
    int size =participants.size();
    if(size==0)
    {
       Not_Found(resp);
       return false; 
    }
    std::string name="paticipant #";
    for(int i=0;i<size;++i)
    {
        std::string name2= name+ std::to_string(i+1);
        resp.j_response[name2]=participants[i];
        
    }
    
    bool check = database.find_chat_by_chat_name(req.j_req["chat_name"]) ;
    if( !database.find_chat_by_chat_name(req.j_req["chat_name"])   || chat.get_chat_name()== std::string()  )
    {
    Not_Found(resp);
    return false;
    }
    
    else
    {
        Send_Response(resp,"/participants/",std::to_string(check));
    // resp.res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    // resp.res.version(10);
    // resp.res.set(http::field::location, "/participants/" + std::to_string(check));
    // resp.res.set(http::field::content_type, "application/json; charset=UTF-8");
    // resp.res.set(http::field::connection, "Keep-Alive");
    // resp.res.result(http::status::ok);
    // resp.response_body = resp.j_response.dump();
    // resp.res.body() = resp.response_body;
    // resp.res.set(http::field::content_length, std::to_string(resp.response_body.length()));
    return true;
    }
}
bool CHAT::get_all_chats_by_login(Request &req,Response &resp ,Postgre_DB &database)
{
    req.j_req = json::parse(req.request_.body());
                        
    
    chats=database.get_all_chats_by_user_login( req.j_req["login"] );
     
    int size =chats.size();
    if(size==0)
    {
    Not_Found(resp);
    return false; 
    }
    std::string name="chats: ";
    std::string name1="participants_from_chat: "; 
    
    for(size_t i=0;i<size;++i)
    {
        std::string name2= name+ std::to_string(i+1);
        resp.j_response[name2]=chats[i].get_chat_name();
        participants=chats[i].get_participants();
        std::string name3;
        for (size_t j = 0; j < participants.size(); j++)
        {
            name3= name1+ std::to_string(j+1);
             resp.j_response[name3]=participants ;
        }
        
        
    }
    
    
    Send_Response(resp,"/participants/",std::to_string(0));
    //  resp.res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    //  resp.res.version(10);
    //  resp.res.set(http::field::location, "/participants/" + std::to_string(0));
    //  resp.res.set(http::field::content_type, "application/json; charset=UTF-8");
    // resp.res.set(http::field::connection, "Keep-Alive");
    //  resp.res.result(http::status::ok);
    //  resp.response_body =  resp.j_response.dump();
    //  resp.res.body() =  resp.response_body;
    //  resp.res.set(http::field::content_length, std::to_string( resp.response_body.length()));
     return true;
        
                        
}
bool CHAT::find_chat(Request &req,Response &resp ,Postgre_DB &database)
{

    req.j_req = json::parse(req.request_.body());
    bool check  =database.find_chat_by_chat_name(req.j_req["chat_name"]);

        
    resp.j_response["check_chat"] = check;

    if(!check)
    {
    Not_Found(resp);
    return false;
    }
    
    else
    {
         Send_Response(resp,"/chat/",std::to_string(check));
    // resp.res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    // resp.res.version(10);
    // resp.res.set(http::field::location, "/chat/" + std::to_string(check));
    // resp.res.set(http::field::content_type, "application/json; charset=UTF-8");
    // resp.res.set(http::field::connection, "Keep-Alive");
    // resp. res.result(http::status::ok);
    // resp.response_body = resp.j_response.dump();
    // resp.res.body() = resp.response_body;
    // resp.res.set(http::field::content_length, std::to_string(resp.response_body.length()));
    return true;
    }
}

 bool MESSAGE::get_last_messages(Request &req,Response &resp ,Postgre_DB &database)
 {
       req.j_req = json::parse(  req.request_.body());
                        
        std::string str;
        str=   req.j_req["num_of_messages"];
        int digit =boost::lexical_cast<int>( str.c_str());
        messages=database.get_last_N_messages_from_chat(  req.j_req["chat_id"],digit);
        

        if(messages.size()==0)
        {
        Not_Found(resp);
        return false;
        }
        std::string mess="message:";
        for (size_t i = 0; i < messages.size(); i++)
        {
                std::string mess2=mess +std::to_string(i+1);
                resp.j_response[mess2]=messages[i].get_message_text();
        }
        
         Send_Response(resp,"/messages/",std::to_string(0));
        //  resp.res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        //  resp.res.version(10);
        //  resp.res.set(http::field::location, "/messages/" + std::to_string(0));
        //  resp.res.set(http::field::content_type, "application/json; charset=UTF-8");
        //  resp.res.set(http::field::connection, "Keep-Alive");
        //  resp.res.result(http::status::ok);
        //  resp.response_body =  resp.j_response.dump();
        //  resp.res.body() =  resp.response_body;
        //  resp.res.set(http::field::content_length, std::to_string( resp.response_body.length()));
        return true;
            
    

 }

 //USER POST

 bool USER::registration(Request &req,Response &resp ,Postgre_DB &database)
 {
      req.j_req = json::parse( req.request_.body());
    User user( req.j_req["login"], req.j_req["password"]);
    
    int result = database.add_user(user);

    if (result != 0) 
    {
        Not_Found(resp);
        return false;
    }
         
        resp.j_response["status"] = result;

        Send_Response(resp,"/registration_user/",std::to_string(result));

//     resp.res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
//     resp.res.version(10);
//     resp.res.set(http::field::content_type, "application/json; charset=UTF-8");
//     resp.res.set(http::field::connection, "Keep-Alive");
//     resp.res.set(http::field::location, "/user/" + std::to_string(result));
//    resp.res.result(http::status::ok);
//     resp.response_body = resp.j_response.dump();
//     resp.res.body() = resp.response_body;
//     resp.res.set(http::field::content_length, std::to_string(resp.response_body.length()));
    return true;
 }
 bool USER::change_login(Request &req,Response &resp ,Postgre_DB &database)
 {
      req.j_req = json::parse(req.request_.body());
    User user=database.get_user_by_login(req.j_req["login"]);
        
    
    int result = database.change_user_login(user,req.j_req["new_log"]);

    if (result != 0)
     {
       Not_Found(resp);
       return false;
    }
         
        resp.j_response["status"] = result;


 Send_Response(resp,"/change_login/",std::to_string(result));
//     resp.res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
//     resp.res.version(10);
//     resp.res.set(http::field::content_type, "application/json; charset=UTF-8");
//     resp.res.set(http::field::connection, "Keep-Alive");
//     resp.res.set(http::field::location, "/user/" + std::to_string(result));
//    resp.res.result(http::status::ok);
//     resp.response_body = resp.j_response.dump();
//     resp.res.body() = resp.response_body;
//     resp.res.set(http::field::content_length, std::to_string(resp.response_body.length()));
    return true;
 }
bool USER::change_password(Request &req,Response &resp ,Postgre_DB &database)
{
     req.j_req = json::parse(req.request_.body());
    User user=database.get_user_by_login(req.j_req["login"]);
        
    
    int result = database.change_user_password(user,req.j_req["new_pass"]);

    if (result != 0)
     {
       Not_Found(resp);
       return false;
    }
         
        resp.j_response["status"] = result;


Send_Response(resp,"/change_pass/",std::to_string(result));
//     resp.res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
//     resp.res.version(10);
//     resp.res.set(http::field::content_type, "application/json; charset=UTF-8");
//     resp.res.set(http::field::connection, "Keep-Alive");
//     resp.res.set(http::field::location, "/user/" + std::to_string(result));
//    resp.res.result(http::status::ok);
//     resp.response_body = resp.j_response.dump();
//     resp.res.body() = resp.response_body;
//     resp.res.set(http::field::content_length, std::to_string(resp.response_body.length()));
    return true;
}
bool USER::change_user_status(Request &req,Response &resp ,Postgre_DB &database)
{
    req.j_req = json::parse(req.request_.body());
    User user=database.get_user_by_login(req.j_req["login"]);
        
    
    int result = database.change_user_status(user,req.j_req["status"]);

    if (result != 0)
     {
       Not_Found(resp);
       return false;
    }
         
        resp.j_response["status"] = result;

Send_Response(resp,"/change_status/",std::to_string(result));

//     resp.res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
//     resp.res.version(10);
//     resp.res.set(http::field::content_type, "application/json; charset=UTF-8");
//     resp.res.set(http::field::connection, "Keep-Alive");
//     resp.res.set(http::field::location, "/user/" + std::to_string(result));
//    resp.res.result(http::status::ok);
//     resp.response_body = resp.j_response.dump();
//     resp.res.body() = resp.response_body;
//     resp.res.set(http::field::content_length, std::to_string(resp.response_body.length()));
    return true;
}
bool USER::delete_user(Request &req,Response &resp ,Postgre_DB &database)
{
    req.j_req = json::parse(req.request_.body());
    User user=database.get_user_by_login(req.j_req["login"]);
        
    
    int result = database.delete_user(user );

    if (result != 0)
     {
       Not_Found(resp);
       return false;
    }
         
        resp.j_response["status"] = result;


Send_Response(resp,"/delete_user/",std::to_string(result));
//     resp.res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
//     resp.res.version(10);
//     resp.res.set(http::field::content_type, "application/json; charset=UTF-8");
//     resp.res.set(http::field::connection, "Keep-Alive");
//     resp.res.set(http::field::location, "/user/" + std::to_string(result));
//    resp.res.result(http::status::ok);
//     resp.response_body = resp.j_response.dump();
//     resp.res.body() = resp.response_body;
//     resp.res.set(http::field::content_length, std::to_string(resp.response_body.length()));
    return true;
}


//CHAT POST

bool CHAT::add_chat(Request &req,Response &resp ,Postgre_DB &database)
{
       req.j_req = json::parse( req.request_.body());
       std::string str;
        str=   req.j_req["size"];
        int count =boost::lexical_cast<int>( str.c_str());
        std::string part="participant_";
    for(int i=0;i<count;i++)
    {
        std::string str=part+std::to_string(i+1);
        participants.push_back(req.j_req[str]) ;
    }
    //participants={ req.j_req["participant_1"], req.j_req["participant_2"]};
    
    Chat chat( req.j_req["chat_name"],participants);
    int result=database.add_chat(chat);


    if (result != 0) 
    {
         Not_Found(resp);
         return false;
    }
          
        resp.j_response["status"] = result;
        
        

    

Send_Response(resp,"/add_chat/",std::to_string(result));
    // resp.res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    // resp.res.version(10);
    // resp.res.set(http::field::content_type, "application/json; charset=UTF-8");
    // resp.res.set(http::field::connection, "Keep-Alive");
    // resp.res.set(http::field::location, "/chat/" + std::to_string(result));
    // resp.res.result(http::status::ok);
    // resp.response_body = resp.j_response.dump();
    // resp.res.body() = resp.response_body;
    //  resp.res.set(http::field::content_length, std::to_string(resp.response_body.length()));
     return true;
}

bool CHAT::add_new_participant(Request &req,Response &resp ,Postgre_DB &database)
{
     req.j_req = json::parse(req.request_.body());

    Chat chat=database.get_chat_by_chat_name(req.j_req["chat_name"]);
    User user= database.get_user_by_login( req.j_req["participant"]);
    int result=database.add_new_participant(user,chat);


    if (result != 0) 
    {
        Not_Found(resp);
        return false;
    }
         
        resp.j_response["status"] = result;
        
        



Send_Response(resp,"/chat_add_part",std::to_string(result));
    //  resp.res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    //  resp.res.version(10);
    //  resp.res.set(http::field::content_type, "application/json; charset=UTF-8");
    //  resp.res.set(http::field::connection, "Keep-Alive");
    // resp.res.set(http::field::location, "/chat/" + std::to_string(result));
    //  resp.res.result(http::status::ok);
    //  resp.response_body =  resp.j_response.dump();
    //  resp.res.body() =  resp.response_body;
    //  resp.res.set(http::field::content_length, std::to_string( resp.response_body.length()));
     return true;
}

bool CHAT::change_chat_name(Request &req,Response &resp ,Postgre_DB &database)
{
    req.j_req = json::parse( req.request_.body());
                  
    Chat chat=database.get_chat_by_chat_name( req.j_req["chat_name"]);
    
    int result=database.change_chat_name(chat, req.j_req["new_chat_name"]);


    if (result != 0)
    {
        Not_Found(resp);
        return false;
         
    }
        
       resp.j_response["status"] = result;
        
        

    

Send_Response(resp,"/change_chatname/",std::to_string(result));
    // resp.res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    // resp.res.version(10);
    // resp.res.set(http::field::content_type, "application/json; charset=UTF-8");
    // resp.res.set(http::field::connection, "Keep-Alive");
    // resp.res.set(http::field::location, "/chat/" + std::to_string(result));
    // resp.res.result(http::status::ok);
    // resp.response_body = resp.j_response.dump();
    // resp.res.body() = resp.response_body;
    // resp.res.set(http::field::content_length, std::to_string(resp.response_body.length()));
    return true;
}

bool CHAT::delete_chat(Request &req,Response &resp ,Postgre_DB &database)
{
req.j_req = json::parse(req.request_.body());
                    
                    
Chat chat=database.get_chat_by_chat_name(req.j_req["chat_name"]);
int result=database.delete_chat(chat);


if (result != 0) 
{
    Not_Found(resp);
    return false;
}
     
    resp.j_response["status"] = result;
    


Send_Response(resp,"/del_chat/",std::to_string(result));
// resp.res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
// resp.res.version(10);
// resp.res.set(http::field::content_type, "application/json; charset=UTF-8");
// resp.res.set(http::field::connection, "Keep-Alive");
// resp.res.set(http::field::location, "/chat/" + std::to_string(result));
// resp.res.result(http::status::ok);
// resp.response_body = resp.j_response.dump();
// resp.res.body() = resp.response_body;
// resp.res.set(http::field::content_length, std::to_string(resp.response_body.length()));
return true;
}


// MESSAGE POST


bool MESSAGE::add_message(Request &req,Response &resp ,Postgre_DB &database)
{
    req.j_req = json::parse(req.request_.body());

    std::string user_id=database.get_user_id(req.j_req["login"]);
    Chat chat=database.get_chat_by_chat_name(req.j_req["chat_name"]);
    TextMessage message= TextMessage(chat.get_chat_id(),user_id,user_id,req.j_req["content"]);
    int result=database.add_message( message);


    if (result != 0)
     {
        
       Not_Found(resp);
       return false;
    }
         
        resp.j_response["status"] = result;
        


Send_Response(resp,"/add_msg/",std::to_string(result));
    // resp.res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    // resp.res.version(10);
    // resp.res.set(http::field::content_type, "application/json; charset=UTF-8");
    // resp.res.set(http::field::connection, "Keep-Alive");
    // resp.res.set(http::field::location, "/message/" + std::to_string(result));
    // resp.res.result(http::status::ok);
    // resp.response_body = resp.j_response.dump();
    // resp.res.body() = resp.response_body;
    // resp.res.set(http::field::content_length, std::to_string(resp.response_body.length()));
    return true;
}
bool MESSAGE::delete_message(Request &req,Response &resp ,Postgre_DB &database)
{
req.j_req = json::parse(req.request_.body());

    std::string user_id=database.get_user_id(req.j_req["login"]);
    Chat chat=database.get_chat_by_chat_name(req.j_req["chat_name"]);
    TextMessage message= TextMessage(chat.get_chat_id(),user_id,user_id,req.j_req["content"]);
    int result=database.delete_message( message);


    if (result != 0)
     {
        
       Not_Found(resp);
       return false;
    }
         
        resp.j_response["status"] = result;
        


        Send_Response(resp,"/del_mess/",std::to_string(result));
    // resp.res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    // resp.res.version(10);
    // resp.res.set(http::field::content_type, "application/json; charset=UTF-8");
    // resp.res.set(http::field::connection, "Keep-Alive");
    // resp.res.set(http::field::location, "/message/" + std::to_string(result));
    // resp.res.result(http::status::ok);
    // resp.response_body = resp.j_response.dump();
    // resp.res.body() = resp.response_body;
    // resp.res.set(http::field::content_length, std::to_string(resp.response_body.length()));
    return true;
}

}