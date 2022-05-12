# Memeger
BMSTU_VK_Project | C/CPP Advanced Cource

# DataBase Part

## __1) Установка PostgreSQL на Ubuntu:__
```
- sudo apt update
- sudo apt install postgresql postgresql-contrib
```

## __2) Установка <pqxx/pqxx> -> libpqxx.so__
```
- sudo apt-get update -y
- sudo apt-get install -y libpqxx-dev
- sudo -u postgres psql -c “ALTER USER postgres PASSWORD ‘postgres’;”
```

## __3) Добавление расширения для генерации UUID внутри БД:__
```
- sudo -u postgres psql
- CREATE EXTENSION IF NOT EXISTS “uuid-ossp”
```