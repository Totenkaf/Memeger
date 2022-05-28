// Copyright 2022 by Artem Ustsov
#pragma once
#include "idatabase.h"
#include "models.h"

class Postgre_DB : public IDataBase {
private:
  std::shared_ptr<pqxx::connection> PG_conn;

  auto save(const std::string &table,
            const std::vector<std::string> &table_fields,
            const std::vector<std::string> &values,
            std::vector<std::string> &output_params, const std::string &where)
      -> int override;
  auto update(const std::string &table,
              const std::vector<std::string> &table_fields,
              const std::vector<std::string> &values, const std::string &where)
      -> int override;
  auto insert(const std::string &table,
              const std::vector<std::string> &table_fields,
              const std::vector<std::string> &values,
              std::vector<std::string> &output_params) -> int override;
  auto delete_(const std::string &table, const std::string &where)
      -> int override;
  auto select(const std::string &table, const std::string &where,
              const std::vector<std::string> &what, int limit)
      -> pqxx::result override;

  auto init_tables() -> int;
  static auto execution_table(const std::string &table_name,
                              std::shared_ptr<pqxx::connection> &PG_conn)
      -> int;

  auto parse_table_fields(const std::vector<std::string> &fields)
      -> std::string;
  auto parse_table_values(const std::vector<std::string> &values)
      -> std::string;
  auto parse_output_params(const std::vector<std::string> &output_params)
      -> std::string;
  static auto remove_danger_characters(const std::string &row_column)
      -> std::string;
  [[nodiscard]] auto get_user_id(const std::string &login) -> std::string;
  [[nodiscard]] auto get_user_login(const std::string &id) -> std::string;
  auto add_user_chat_link(const std::string &user_id,
                          const std::string &chat_id) -> int;

  std::string db_host_;
  std::string db_port_;
  std::string db_name_;
  std::string db_user_;
  std::string db_password_;

public:
  Postgre_DB() = default;
  Postgre_DB(std::string db_host, std::string db_port, std::string db_name,
             std::string db_user, std::string db_password);

  Postgre_DB(const Postgre_DB &other) = default;
  Postgre_DB(Postgre_DB &&other) = default;

  auto operator=(const Postgre_DB &) -> Postgre_DB & = default;
  auto operator=(Postgre_DB &&other) -> Postgre_DB & = default;

  ~Postgre_DB() override;

  auto drop_tables() -> int;

  auto add_user(User &user) -> int;
  auto change_user_login(User &user, const std::string &new_login) -> int;
  auto change_user_password(User &user, const std::string &new_password) -> int;
  auto change_user_status(User &user, const std::string &new_status) -> int;
  auto delete_user(User &user) -> int;
  [[nodiscard]] auto find_user_by_login(const std::string &login) -> bool;
  [[nodiscard]] auto get_user_by_login(const std::string &login) -> User;

  auto add_message(const std::shared_ptr<IMessage>& message) -> int;
  auto delete_message(const std::shared_ptr<IMessage>& message) -> int;
  [[nodiscard]] auto get_last_N_messages_from_chat(const Chat &chat,
                                                   int num_of_messages)
      -> std::vector<std::shared_ptr<IMessage>>;

  auto add_chat(Chat &chat) -> int;
  auto add_new_participant(const User &user, const Chat &chat) -> int;
  auto change_chat_name(Chat &chat, const std::string &new_chat_name) -> int;
  auto delete_chat(Chat &chat) -> int;
  [[nodiscard]] auto find_chat_by_chat_name(const std::string &chat_name)
      -> bool;
  [[nodiscard]] auto get_chat_by_chat_name(const std::string &chat_name)
      -> Chat;
  [[nodiscard]] auto get_chat_by_id(const std::string &chat_id) -> Chat;
  [[nodiscard]] auto get_participants_from_chat(const Chat &chat)
      -> std::vector<std::string>;
  [[nodiscard]] auto get_all_chats_by_user_login(const std::string &login)
      -> std::vector<Chat>;
};
