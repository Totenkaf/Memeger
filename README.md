# Memeger
BMSTU_VK_Project | Углебленное программирование на C/С++
Клиент серверное приложение для обмена файлами между пользователями

# DataBase Part
Реляционная база данных PostgreSql для хранения данных пользователей, чатов, сообщений
Локальный хост: 127.0.0.1
Локальный порт: 3452
Имя пользователя по умолчанию: postgres
Имя базы данных по умолчанию: postgres
Пароль по умолчанию: не устанавливается

## __1) Установка PostgreSQL на Ubuntu:__
```
- sudo apt update
- sudo apt install postgresql postgresql-contrib
```

## __2) Установка <pqxx/pqxx> -> libpqxx.so__
```
- sudo apt-get update -y
- sudo apt-get install -y libpqxx-dev
```

## __3) Добавление пароля и расширения для генерации UUID внутри БД:__
```
- sudo -u postgres psql -c “ALTER USER postgres PASSWORD ‘postgres’;”
- sudo -u postgres psql -c “CREATE EXTENSION IF NOT EXISTS 'uuid-ossp';”
```
