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
- sudo apt-get install postgresql postgresql-contrib
```

## __2) Установка <pqxx/pqxx> -> libpqxx.so__
```
- sudo apt-get install -y libpqxx-dev
```

## __3) Добавление пароля и расширения для генерации UUID внутри БД:__
```
- sudo -u postgres psql postgres -c "ALTER USER postgres PASSWORD 'postgres';"
- sudo -u postgres psql postgres -с "CREATE EXTENSION IF NOT EXISTS \"uuid-ossp\";"
```

# Server&Client Part
Асинхронный сервер на библиотеке BOOST::ASIO, количество потоков указывается пользователем  
Клиент на библиотеке BOOST::ASIO  
Локальный хост: 127.0.0.1  
Локальный порт: 8080  

## __1) Установка BOOST+Json на Ubuntu:__
```
- sudo apt update && sudo apt upgrade
- sudo apt-get install libboost-all-dev
- git clone https://github.com/nlohmann/json.git 
- cd json 
- cmake -DBuildTests=OFF .
```

## __2) Сборка BOOST+Json:__
```
- sudo apt-get install checkinstall
- sudo checkinstall -D \
    -y --nodoc \
    --pkgname=libnlohmann-json-dev-custom \
    --pkgversion=2.1.1 \
    --pkgrelease=git \
    --deldesc=no
```

# GUI Part
Графический интерфейс реализован при помощи библиотеки GTK  

## __1) Установка GTK-3 на Ubuntu:__
```
- sudo apt-get install libgtk-3-dev
```

# Сборка проекта и запуск

## __1) Сборка build:__
```
- mkdir build && cd build
- cmake ..
```
## __2) Запуск сервера:__
```
- ./run_server localhost 8080 4
```

## __3) Запуск клиента:__
```
- ./run_client
```
