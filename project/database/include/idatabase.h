// Copyright 2022 by Artem Ustsov

#pragma once
#include <iostream>
#include <pqxx/pqxx>
#include <string>

class IDataBase {
private:
  [[nodiscard]] virtual auto save(const std::string& table,
                   const std::vector<std::string>& table_fields,
                   const std::vector<std::string>& values,
                   std::vector<std::string>& output_params,
                   const std::string& where) -> int = 0;
  [[nodiscard]] virtual auto update(const std::string& table,
                     const std::vector<std::string>& table_fields,
                     const std::vector<std::string>& values,
                     const std::string& where) -> int = 0;
  [[nodiscard]] virtual auto insert(const std::string& table,
                     const std::vector<std::string>& table_fields,
                     const std::vector<std::string>& values,
                     std::vector<std::string>& output_params) -> int = 0;
  [[nodiscard]] virtual auto delete_(const std::string& table, const std::string& where) -> int = 0;
  [[nodiscard]] virtual auto select(const std::string& table, const std::string& where,
                              const std::vector<std::string>& what,
                              int limit = -1) -> pqxx::result = 0;
};
