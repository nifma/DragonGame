# Минимальный образ с компилятором
FROM ubuntu:24.04

RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ make curl ca-certificates && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app
# Забираем single-header HTTP сервер
RUN curl -L -o httplib.h https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h

# Копируем исходники
COPY main.cpp /app/main.cpp
COPY www /app/www

# Сборка
RUN g++ -O2 -std=gnu++17 main.cpp -o app

EXPOSE 8080
CMD ["./app", "-port", "8080"]
