FROM gcc:12.2.0

WORKDIR /app

# Забираем single-header HTTP сервер
RUN curl -L -o httplib.h https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h

# Копируем исходники
COPY attacker.h attacker.cpp enemy.h enemy.cpp game.h game.cpp main.cpp /app/
COPY www /app/www

# Сборка
RUN g++ -O2 -std=gnu++17 -pthread attacker.cpp enemy.cpp game.cpp main.cpp -o app

EXPOSE 8080
CMD ["./app"]