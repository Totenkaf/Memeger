[![.github/workflows/ci.yml](https://github.com/Totenkaf/Memeger/actions/workflows/ci.yml/badge.svg)](https://github.com/Totenkaf/Memeger/actions/workflows/ci.yml)

# Memeger
BMSTU_VK_Project | Углубленное программирование на C/С++  
Клиент серверное приложение для обмена файлами между пользователями

# DataBase Part
Реляционная база данных PostgreSQL для хранения данных пользователей, чатов, сообщений  
Локальный хост: 127.0.0.1  
Локальный порт: 5432  
Имя пользователя по умолчанию: postgres  
Имя базы данных по умолчанию: postgres  
Пароль по умолчанию: не устанавливается  

## __1) Установка PostgreSQL на Ubuntu:__
```
- sudo apt-get update -y
- sudo apt-get install -y postgresql
```

## __2) Установка <pqxx/pqxx> -> libpqxx.so__
```
- sudo apt-get install -y libpqxx-dev
```

## __3) Добавление пароля и расширения для генерации UUID внутри БД:__
```
- sudo -u postgres psql postgres -c "\conninfo"
- sudo -u postgres psql postgres -c "ALTER USER postgres PASSWORD 'postgres';"
- sudo -u postgres psql postgres -c "CREATE EXTENSION IF NOT EXISTS \"uuid-ossp\";"
```
