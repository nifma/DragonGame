#ifndef GAME_H
#define GAME_H

#include "enemy.h"
#include <vector>
#include <memory>
#include <string>

class Game {
public:
    static Game& GetInstance();
    void Reset();

    Enemy* GetCurrentEnemy();
    void AskNewQuestion();
    void ProcessAnswer(const std::string& answer);

    // Геттеры
    const Hero& hero() const;
    const std::string& last_question() const;
    const std::string& last_info() const;
    const std::string& full_history() const;
    bool finished() const;
    bool win() const;

private:
    Game() = default;
    ~Game() = default;

    Hero hero_{"Герой", 35, 7};
    std::vector<std::unique_ptr<Enemy>> enemies_;
    int current_enemy_ = 0;
    std::string last_question_;
    std::string last_info_;
    std::string full_history_;
    bool finished_ = false;
    bool win_ = false;
};

#endif