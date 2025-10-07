// game.cpp
#include "game.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

Game& Game::GetInstance() {
  static Game instance;
  return instance;
}

void Game::Reset() {
  srand(static_cast<unsigned>(time(nullptr)));
  hero_ = Hero("Герой", 35, 7);
  enemies_.clear();
  current_enemy_ = 0;
  finished_ = false;
  win_ = false;
  last_info_.clear();
  full_history_.clear();

  std::vector<std::unique_ptr<Enemy>> all_enemies;
  // Добавляем драконов
  all_enemies.push_back(std::unique_ptr<Enemy>(new YellowDragon()));
  all_enemies.push_back(std::unique_ptr<Enemy>(new BlueDragon()));
  all_enemies.push_back(std::unique_ptr<Enemy>(new PurpleDragon()));
  all_enemies.push_back(std::unique_ptr<Enemy>(new GreenDragon()));
  all_enemies.push_back(std::unique_ptr<Enemy>(new RedDragon()));
  // Добавляем гоблинов
  all_enemies.push_back(std::unique_ptr<Enemy>(new MathGoblin()));
  all_enemies.push_back(std::unique_ptr<Enemy>(new RiddleGoblin()));
  all_enemies.push_back(std::unique_ptr<Enemy>(new TrickGoblin()));

  // Перемешиваем врагов в случайном порядке
  for (int i = all_enemies.size() - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    std::swap(all_enemies[i], all_enemies[j]);
  }

  // Берем первых 4 врагов для игры
  for (int i = 0; i < std::min(4, static_cast<int>(all_enemies.size())); i++) {
    enemies_.push_back(std::move(all_enemies[i]));
  }

  AskNewQuestion();
}

Enemy* Game::GetCurrentEnemy() {
  if (current_enemy_ < static_cast<int>(enemies_.size())) {
    return enemies_[current_enemy_].get();
  }
  return nullptr;
}

void Game::AskNewQuestion() {
  if (finished_) return;
  Enemy* enemy = GetCurrentEnemy();
  if (!enemy) {
    finished_ = true;
    win_ = true;
    last_question_ = "Все враги повержены!";
    return;
  }
  last_question_ = enemy->GetName() + " спрашивает: " + enemy->GenerateQuestion();
  full_history_ += "➤ " + last_question_ + "\n";
}

void Game::ProcessAnswer(const std::string& answer) {
  if (finished_) return;
  Enemy* enemy = GetCurrentEnemy();
  if (!enemy) {
    finished_ = true;
    win_ = true;
    return;
  }

  std::string action;
  bool is_correct = enemy->CheckAnswer(answer);
  if (is_correct) {
    hero_.Hit(*enemy);
    hero_.AddExp(5);
    action = "✅ Верно! Герой атакует (" + std::to_string(hero_.atk()) + ")";
    full_history_ += "✅ Герой отвечает верно и атакует врага!\n";
    if (!enemy->IsAlive()) {
      action += " и побеждает врага!";
      full_history_ += "🎯 Герой побеждает " + enemy->GetName() + "!\n";
      ++current_enemy_;
    }
  } else {
    enemy->Hit(hero_);
    action = "❌ Неверно. " + enemy->GetName() + " атакует героя (" +
             std::to_string(enemy->atk()) + ")";
    full_history_ += "❌ Неверный ответ! " + enemy->GetName() + " атакует героя!\n";
    if (!hero_.IsAlive()) {
      finished_ = true;
      win_ = false;
      last_question_.clear();
      full_history_ += "💀 Герой пал в бою... Игра окончена.\n";
    }
  }

  last_info_ = action + "\n";

  if (!finished_) {
    AskNewQuestion();
  } else if (win_) {
    last_question_ = "Победа! Все враги повержены.";
    full_history_ += "🎉 ПОБЕДА! Все враги повержены! 🎉\n";
  } else {
    last_question_ = "Поражение. Герой пал.";
  }
}

// Геттеры
const Hero& Game::hero() const { return hero_; }
const std::string& Game::last_question() const { return last_question_; }
const std::string& Game::last_info() const { return last_info_; }
const std::string& Game::full_history() const { return full_history_; }
bool Game::finished() const { return finished_; }
bool Game::win() const { return win_; }