# Используем базовый образ с Ubuntu
FROM ubuntu:20.04

# Устанавливаем рабочую директорию
WORKDIR /app

# Устанавливаем необходимые пакеты
RUN apt-get update && apt-get install -y \
    build-essential \
    apache2 \
    libjsoncpp-dev \
    cmake \
    g++ \
    curl \
    && apt-get clean

# Копируем исходники проекта в контейнер
COPY . /app

# Создаем директорию для CGI-скриптов
RUN mkdir -p /var/www/html/cgi-bin

# Компилируем проект с помощью CMake
RUN cmake -S . -B /app/build \
    && cmake --build /app/build \
    && cp /app/build/game.cgi /var/www/html/cgi-bin/

# Устанавливаем права на выполнение для CGI-скрипта
RUN chmod +x /var/www/html/cgi-bin/game.cgi

# Конфигурируем Apache для работы с CGI
COPY .htaccess /var/www/html/.htaccess

# Открываем порты для Apache
EXPOSE 80

# Запускаем Apache
CMD ["apache2ctl", "-D", "FOREGROUND"]
