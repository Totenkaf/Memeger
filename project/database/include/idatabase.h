// Copyright 2022 by Artem Ustsov

#pragma once
#include <iostream>
#include <pqxx/pqxx>
#include <string>

class IDataBase {
 private:
  virtual int save(const std::string& table,
                   const std::vector<std::string>& table_fields,
                   const std::vector<std::string>& values,
                   std::vector<std::string>& output_params,
                   std::string where) = 0;
  virtual int update(const std::string& table,
                     const std::vector<std::string>& table_fields,
                     const std::vector<std::string>& values,
                     std::string where) = 0;
  virtual int insert(const std::string& table,
                     const std::vector<std::string>& table_fields,
                     const std::vector<std::string>& values,
                     std::vector<std::string>& output_params) = 0;
  virtual int delete_(const std::string& table, std::string where) = 0;
  virtual pqxx::result select(const std::string& table, std::string where,
                              std::vector<std::string> what, int limit) = 0;
};
