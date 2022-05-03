//Copyright 2022 by Artem Ustsov

#pragma once
#include <iostream>
#include <string>
#include <pqxx/pqxx>


class IDataBase {
  private:
    virtual int save(const std::string& table, const std::string& table_fiels, std::vector <std::string> values, std::string& output_res, std::string where) = 0;
    virtual int update(const std::string& table, const std::string& table_fields, std::vector <std::string> values, std::string where = "") = 0;
    virtual int insert(const std::string& table, const std::string& table_fields, std::vector<std::string> values, std::string& output_res) = 0;
    virtual int delete_(const std::string & table, std::string where = "") = 0;
    virtual pqxx::result select(const std::string& table, std::string where = "", std::vector <std::string> what = std::vector <std::string>()) = 0;
};
