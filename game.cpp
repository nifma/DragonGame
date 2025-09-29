#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <json/json.h>

class Enemy {
public:
    virtual std::string question() = 0;
    virtual bool checkAnswer(const std::string& answer) = 0;
    virtual std::string getName() = 0;
};

class Dragon : public Enemy {
protected:
    std::string name;
    std::string question_text;
    std::string correct_answer;

public:
    Dragon(std::string n, std::string q, std::string a) : name(n), question_text(q), correct_answer(a) {}

    std::string question() override {
        return question_text;
    }

    bool checkAnswer(const std::string& answer) override {
        return answer == correct_answer;
    }

    std::string getName() override {
        return name;
    }
};

class Hero {
private:
    std::string name;

public:
    Hero(std::string n) : name(n) {}

    std::string getName() {
        return name;
    }
};

std::vector<Enemy*> enemies;
Hero* hero = nullptr;
int currentEnemyIndex = 0;

void setupGame(const std::string& hero_name) {
    hero = new Hero(hero_name);

    // Создание драконов
    enemies.push_back(new Dragon("Красный дракон", "Сколько будет 5 - 3?", "2"));
    enemies.push_back(new Dragon("Зелёный дракон", "Сколько будет 7 + 4?", "11"));
    enemies.push_back(new Dragon("Чёрный дракон", "Сколько будет 3 * 6?", "18"));
}

Json::Value getNextEnemy() {
    Json::Value result;

    if (currentEnemyIndex < enemies.size()) {
        Enemy* enemy = enemies[currentEnemyIndex++];
        result["enemy"] = enemy->getName();
        result["question"] = enemy->question();
    } else {
        result["enemy"] = Json::nullValue;
    }

    return result;
}

Json::Value submitAnswer(const std::string& answer, const std::string& enemy_name) {
    Json::Value result;

    Enemy* enemy = nullptr;
    for (auto e : enemies) {
        if (e->getName() == enemy_name) {
            enemy = e;
            break;
        }
    }

    if (enemy && enemy->checkAnswer(answer)) {
        result["correct"] = true;
    } else {
        result["correct"] = false;
    }

    return result;
}

int main() {
    std::cout << "Content-type: application/json\n\n";

    std::string query;
    std::getline(std::cin, query);

    std::string action = query.substr(query.find("action=") + 7, query.find("&") - query.find("action=") - 7);

    if (action == "next_enemy") {
        std::string hero_name = query.substr(query.find("hero_name=") + 10);
        setupGame(hero_name);
        Json::Value response = getNextEnemy();
        Json::StreamWriterBuilder writer;
        std::cout << Json::writeString(writer, response);
    } else if (action == "submit_answer") {
        std::string hero_name = query.substr(query.find("hero_name=") + 10);
        std::string enemy_name = query.substr(query.find("enemy=") + 6, query.find("&answer=") - query.find("enemy=") - 6);
        std::string answer = query.substr(query.find("answer=") + 7);

        Json::Value response = submitAnswer(answer, enemy_name);
        Json::StreamWriterBuilder writer;
        std::cout << Json::writeString(writer, response);
    }

    return 0;
}
