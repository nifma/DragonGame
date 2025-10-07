#include <bits/stdc++.h>
#include "httplib.h"

using namespace std;

class Attacker {
 public:
  Attacker(int hp, int atk) : health_(hp), attack_(atk) {}
  virtual ~Attacker() = default;

  bool IsAlive() const { return health_ > 0; }

  void Hit(Attacker& target) const {
    target.health_ = max(0, target.health_ - attack_);
  }

  int hp() const { return health_; }
  int atk() const { return attack_; }

  virtual bool IsEnemy() const = 0;

 protected:
  int health_;
  int attack_;
};

class Enemy : public Attacker {
 public:
  using Attacker::Attacker;

  bool IsEnemy() const final { return true; }

  virtual string GetName() const = 0;
  virtual string GetColor() const = 0;
  virtual string GenerateQuestion() = 0;
  virtual bool CheckAnswer(const string& answer) const = 0;
};

class Hero : public Attacker {
 public:
  Hero(string name, int hp = 30, int atk = 6)
      : Attacker(hp, atk), name_(std::move(name)) {}

  bool IsEnemy() const override { return false; }

  const string& name() const { return name_; }
  int exp() const { return experience_; }
  void AddExp(int value) { experience_ += value; }

 private:
  string name_;
  int experience_ = 0;
};

class Dragon : public Enemy {
 public:
  Dragon(string color, int hp = 15, int atk = 5)
      : Enemy(hp, atk), color_(std::move(color)) {}

  string GetColor() const override { return color_; }
  string GetName() const override { return color_ + " Dragon"; }

 protected:
  string color_;
  mutable int a_, b_, answer_;
};

class YellowDragon : public Dragon {
 public:
  YellowDragon() : Dragon("Yellow") {}

  string GenerateQuestion() override {
    a_ = rand() % 5 + 1;
    b_ = rand() % 3 + 2;
    answer_ = a_ * b_;
    return "Вычислите коэффициент производной функции f(x) = " +
           to_string(a_) + "x^" + to_string(b_);
  }

  bool CheckAnswer(const string& input) const override {
    try {
      return stoi(input) == answer_;
    } catch (...) {
      return false;
    }
  }
};

class BlueDragon : public Dragon {
 public:
  BlueDragon() : Dragon("Blue") {}

  string GenerateQuestion() override {
    a_ = rand() % 10 + 3;
    b_ = rand() % 4 + 2;
    answer_ = pow(a_, b_);
    return "Вычислите " + to_string(a_) + " в степени " + to_string(b_);
  }

  bool CheckAnswer(const string& input) const override {
    try {
      return stoi(input) == answer_;
    } catch (...) {
      return false;
    }
  }
};

class PurpleDragon : public Dragon {
 public:
  PurpleDragon() : Dragon("Purple") {}

  string GenerateQuestion() override {
    a_ = rand() % 50 + 1;
    b_ = rand() % 20 + 1;
    answer_ = (a_ + b_) / 2;
    return "Вычислите среднее значение между числами " + to_string(a_) +
           " и " + to_string(b_);
  }

  bool CheckAnswer(const string& input) const override {
    try {
      return stoi(input) == answer_;
    } catch (...) {
      return false;
    }
  }
};

class GreenDragon : public Dragon {
 public:
  GreenDragon() : Dragon("Green") {}

  string GenerateQuestion() override {
    a_ = rand() % 20 + 5;  // от 5 до 24
    b_ = rand() % 10 + 1;  // от 1 до 10
    answer_ = a_ % b_;
    return "Найдите остаток от деления " + to_string(a_) + " на " +
           to_string(b_);
  }

  bool CheckAnswer(const string& input) const override {
    try {
      return stoi(input) == answer_;
    } catch (...) {
      return false;
    }
  }
};

class RedDragon : public Dragon {
 public:
  RedDragon() : Dragon("Red") {}

  string GenerateQuestion() override {
    a_ = rand() % 91 + 10;
    b_ = rand() % 21 + 5;
    answer_ = a_ - b_;
    return "Решите уравнение: x + " + to_string(b_) + " = " + to_string(a_) +
           ". Найдите x";
  }

  bool CheckAnswer(const string& input) const override {
    try {
      return stoi(input) == answer_;
    } catch (...) {
      return false;
    }
  }
};

class Goblin : public Enemy {
 public:
  Goblin(string type, int hp = 12, int atk = 4)
      : Enemy(hp, atk), type_(std::move(type)) {}

  string GetColor() const override { return "Green"; }
  string GetName() const override { return type_ + " Goblin"; }

 protected:
  string type_;
  mutable int a_, b_, c_, answer_;
};

class MathGoblin : public Goblin {
 public:
  MathGoblin() : Goblin("Math") {}

  string GenerateQuestion() override {
    a_ = rand() % 10 + 1;
    b_ = rand() % 10 + 1;
    answer_ = a_ + b_;
    return "Сколько будет " + to_string(a_) + " + " + to_string(b_) + "?";
  }

  bool CheckAnswer(const string& input) const override {
    try {
      return stoi(input) == answer_;
    } catch (...) {
      return false;
    }
  }
};

class RiddleGoblin : public Goblin {
 public:
  RiddleGoblin() : Goblin("Riddle") {}

  string GenerateQuestion() override {
    a_ = rand() % 10 + 1;
    b_ = rand() % 10 + 1;
    answer_ = a_ * b_;
    return "У меня " + to_string(a_) + " мешков, в каждом " + to_string(b_) +
           " золотых. Сколько всего золотых?";
  }

  bool CheckAnswer(const string& input) const override {
    try {
      return stoi(input) == answer_;
    } catch (...) {
      return false;
    }
  }
};

class TrickGoblin : public Goblin {
 public:
  TrickGoblin() : Goblin("Trick") {}

  string GenerateQuestion() override {
    a_ = rand() % 8 + 3;
    b_ = rand() % 5 + 2;
    answer_ = a_ - b_;
    return "Было " + to_string(a_) + " печенек, я съел " + to_string(b_) +
           ". Сколько осталось?";
  }

  bool CheckAnswer(const string& input) const override {
    try {
      return stoi(input) == answer_;
    } catch (...) {
      return false;
    }
  }
};

class Game {
 public:
  static Game& GetInstance() {
    static Game instance;
    return instance;
  }

  void Reset() {
    srand(static_cast<unsigned>(time(nullptr)));
    hero_ = Hero("Герой", 35, 7);
    enemies_.clear();
    current_enemy_ = 0;
    finished_ = false;
    win_ = false;
    last_info_.clear();
    full_history_.clear();

    vector<unique_ptr<Enemy>> all_enemies;
    // Добавляем драконов
    all_enemies.push_back(unique_ptr<Enemy>(new YellowDragon()));
    all_enemies.push_back(unique_ptr<Enemy>(new BlueDragon()));
    all_enemies.push_back(unique_ptr<Enemy>(new PurpleDragon()));
    all_enemies.push_back(unique_ptr<Enemy>(new GreenDragon()));
    all_enemies.push_back(unique_ptr<Enemy>(new RedDragon()));
    // Добавляем гоблинов
    all_enemies.push_back(unique_ptr<Enemy>(new MathGoblin()));
    all_enemies.push_back(unique_ptr<Enemy>(new RiddleGoblin()));
    all_enemies.push_back(unique_ptr<Enemy>(new TrickGoblin()));

    // Перемешиваем врагов в случайном порядке
    for (int i = all_enemies.size() - 1; i > 0; i--) {
      int j = rand() % (i + 1);
      swap(all_enemies[i], all_enemies[j]);
    }

    // Берем первых 4 врагов для игры
    for (int i = 0; i < min(4, static_cast<int>(all_enemies.size())); i++) {
      enemies_.push_back(move(all_enemies[i]));
    }

    AskNewQuestion();
  }

  Enemy* GetCurrentEnemy() {
    if (current_enemy_ < static_cast<int>(enemies_.size())) {
      return enemies_[current_enemy_].get();
    }
    return nullptr;
  }

  void AskNewQuestion() {
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

  void ProcessAnswer(const string& answer) {
    if (finished_) return;
    Enemy* enemy = GetCurrentEnemy();
    if (!enemy) {
      finished_ = true;
      win_ = true;
      return;
    }

    string action;
    bool is_correct = enemy->CheckAnswer(answer);
    if (is_correct) {
      hero_.Hit(*enemy);
      hero_.AddExp(5);
      action = "✅ Верно! Герой атакует (" + to_string(hero_.atk()) + ")";
      full_history_ += "✅ Герой отвечает верно и атакует врага!\n";
      if (!enemy->IsAlive()) {
        action += " и побеждает врага!";
        full_history_ += "🎯 Герой побеждает " + enemy->GetName() + "!\n";
        ++current_enemy_;
      }
    } else {
      enemy->Hit(hero_);
      action = "❌ Неверно. " + enemy->GetName() + " атакует героя (" +
               to_string(enemy->atk()) + ")";
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
  const Hero& hero() const { return hero_; }
  const string& last_question() const { return last_question_; }
  const string& last_info() const { return last_info_; }
  const string& full_history() const { return full_history_; }
  bool finished() const { return finished_; }
  bool win() const { return win_; }

 private:
  Game() = default;
  ~Game() = default;

  Hero hero_{"Герой", 35, 7};
  vector<unique_ptr<Enemy>> enemies_;
  int current_enemy_ = 0;
  string last_question_;
  string last_info_;
  string full_history_;
  bool finished_ = false;
  bool win_ = false;
};

static string JsonEscape(const string& s) {
  string result;
  result.reserve(s.size() + 8);
  for (char c : s) {
    switch (c) {
      case '\"':
        result += "\\\"";
        break;
      case '\\':
        result += "\\\\";
        break;
      case '\n':
        result += "\\n";
        break;
      case '\r':
        result += "\\r";
        break;
      case '\t':
        result += "\\t";
        break;
      default:
        result += c;
    }
  }
  return result;
}

int main() {
  Game::GetInstance().Reset();

  httplib::Server server;

  // CORS headers
  server.set_default_headers({
      {"Access-Control-Allow-Origin", "*"},
      {"Access-Control-Allow-Headers", "Content-Type"},
      {"Access-Control-Allow-Methods", "GET,POST,OPTIONS"},
  });

  server.Options(R"(.*)", [](const httplib::Request& req, httplib::Response& res) {
    res.status = 200;
  });

  // Static files
  server.Get("/", [](const httplib::Request&, httplib::Response& res) {
    res.set_content("<meta http-equiv='refresh' content='0; url=/index.html'>",
                    "text/html");
  });

  // Game state endpoint
  server.Get("/state", [](const httplib::Request&, httplib::Response& res) {
    Game& game = Game::GetInstance();
    Enemy* enemy = game.GetCurrentEnemy();

    string enemy_name = enemy ? enemy->GetName() : "-";
    string enemy_color = enemy ? enemy->GetColor() : "-";
    int enemy_hp = enemy ? enemy->hp() : 0;
    int enemy_atk = enemy ? enemy->atk() : 0;

    string json_body = string("{") +
        "\"hero\":{\"name\":\"" + JsonEscape(game.hero().name()) +
        "\",\"hp\":" + to_string(game.hero().hp()) +
        ",\"atk\":" + to_string(game.hero().atk()) +
        ",\"exp\":" + to_string(game.hero().exp()) + "}," +
        "\"enemy\":{\"name\":\"" + JsonEscape(enemy_name) +
        "\",\"color\":\"" + JsonEscape(enemy_color) +
        "\",\"hp\":" + to_string(enemy_hp) +
        ",\"atk\":" + to_string(enemy_atk) + "}," +
        "\"question\":\"" + JsonEscape(game.last_question()) + "\"," +
        "\"info\":\"" + JsonEscape(game.last_info()) + "\"," +
        "\"history\":\"" + JsonEscape(game.full_history()) + "\"," +
        "\"finished\":" + (game.finished() ? "true" : "false") + "," +
        "\"win\":" + (game.win() ? "true" : "false") + "}";

    res.set_content(json_body, "application/json; charset=utf-8");
  });

  // Answer endpoint
  server.Post("/answer", [](const httplib::Request& req, httplib::Response& res) {
    string body = req.body;
    string key = "\"answer\"";
    size_t pos = body.find(key);
    string answer_value;

    if (pos != string::npos) {
      size_t colon_pos = body.find(':', pos);
      size_t quote_start = body.find('"', colon_pos + 1);
      size_t quote_end = body.find('"', quote_start + 1);
      if (quote_start != string::npos && quote_end != string::npos) {
        answer_value = body.substr(quote_start + 1, quote_end - quote_start - 1);
      }
    }

    Game::GetInstance().ProcessAnswer(answer_value);
    res.set_content("{\"ok\":true}", "application/json");
  });

  // New game endpoint
  server.Post("/newgame", [](const httplib::Request&, httplib::Response& res) {
    Game::GetInstance().Reset();
    res.set_content("{\"started\":true}", "application/json");
  });

  // Serve static files
  server.set_mount_point("/", "./www");

  cout << "Server running on http://0.0.0.0:8080\n";
  server.listen("0.0.0.0", 8080);
  return 0;
}
