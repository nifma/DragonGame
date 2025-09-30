#include <bits/stdc++.h>
#include "httplib.h"

using namespace std;

class Attacker {
protected:
    int _health;
    int _attack;
public:
    Attacker(int hp, int atk) : _health(hp), _attack(atk) {}
    virtual ~Attacker() = default;

    bool is_alive() const { return _health > 0; }
    void hit(Attacker& target) const { target._health = max(0, target._health - _attack); }
    int hp() const { return _health; }
    int atk() const { return _attack; }

    virtual bool IsEnemy() const = 0;
};

class Enemy : public Attacker {
public:
    using Attacker::Attacker;
    bool IsEnemy() const final { return true; }

    virtual string name() const = 0;
    virtual string color() const = 0;
    virtual string question() = 0;
    virtual bool check_answer(const string& answer) const = 0;
};

class Hero : public Attacker {
    string _name;
    int _experience = 0;
public:
    Hero(string name, int hp=30, int atk=6) : Attacker(hp, atk), _name(std::move(name)) {}
    bool IsEnemy() const override { return false; }
    const string& name() const { return _name; }
    int exp() const { return _experience; }
    void add_exp(int v){ _experience += v; }
};

class Dragon : public Enemy {
protected:
    string _color;
    mutable int _a, _b, _ans;
public:
    Dragon(string clr, int hp=15, int atk=5) : Enemy(hp, atk), _color(std::move(clr)) {}
    string color() const override { return _color; }
    string name() const override { return _color + " Dragon"; }
};

struct YellowDragon : public Dragon {
    YellowDragon() : Dragon("Yellow") {}
    string question() override {
        _a = rand() % 5 + 1;
        _b = rand() % 3 + 2;
        _ans = _a * _b;
        return "Вычислите производную функции f(x) = " + to_string(_a) + "x^" + to_string(_b);
    }
    bool check_answer(const string& s) const override {
        try { return stoi(s) == _ans; } catch(...) { return false; }
    }
};

struct BlueDragon : public Dragon {
    BlueDragon() : Dragon("Blue") {}
    string question() override {
        _a = rand() % 10 + 3;
        _b = rand() % 4 + 2;
        _ans = pow(_a, _b);
        return "Вычислите " + to_string(_a) + " в степени " + to_string(_b);
    }
    bool check_answer(const string& s) const override {
        try { return stoi(s) == _ans; } catch(...) { return false; }
    }
};

struct PurpleDragon : public Dragon {
    PurpleDragon() : Dragon("Purple") {}
    string question() override {
        _a = rand() % 50 + 1;
        _b = rand() % 20 + 1;
        _ans = (_a + _b) / 2;
        return "Вычислите среднее значение между числами " + to_string(_a) + " и " + to_string(_b);
    }
    bool check_answer(const string& s) const override {
        try { return stoi(s) == _ans; } catch(...) { return false; }
    }
};

struct GreenDragon : public Dragon {
    GreenDragon() : Dragon("Green") {}
    string question() override {
        _a = rand() % 20 + 5;  // от 5 до 24
        _b = rand() % 10 + 1;  // от 1 до 10
        _ans = _a % _b;
        return "Найдите остаток от деления " + to_string(_a) + " на " + to_string(_b);
    }
    bool check_answer(const string& s) const override {
        try { return stoi(s) == _ans; } catch(...) { return false; }
    }
};

struct RedDragon : public Dragon {
    RedDragon() : Dragon("Red") {}
    string question() override {
        _a = rand() % 91 + 10; // от 10 до 100
        _b = rand() % 21 + 5;  // от 5 до 25
        _ans = _a - _b;
        return "Решите уравнение: x + " + to_string(_b) + " = " + to_string(_a) + ". Найдите x";
    }
    bool check_answer(const string& s) const override {
        try { return stoi(s) == _ans; } catch(...) { return false; }
    }
};

// ========= ИГРА =========
struct Game {
    Hero hero{"Герой", 35, 7};
    vector<unique_ptr<Enemy>> enemies;
    int cur = 0;
    string last_question;
    string last_info;
    string full_history;
    bool finished = false, win = false;

    static Game& instance() { static Game g; return g; }

    void reset() {
        srand((unsigned)time(nullptr));
        hero = Hero("Герой", 35, 7);
        enemies.clear(); cur = 0; finished = false; win = false; last_info.clear(); full_history.clear();

        vector<unique_ptr<Enemy>> all_dragons;
        all_dragons.emplace_back(make_unique<YellowDragon>());
        all_dragons.emplace_back(make_unique<BlueDragon>());
        all_dragons.emplace_back(make_unique<PurpleDragon>());
        all_dragons.emplace_back(make_unique<GreenDragon>());
        all_dragons.emplace_back(make_unique<RedDragon>());

        random_device rd;
        mt19937 g(rd());
        shuffle(all_dragons.begin(), all_dragons.end(), g);

        for (int i = 0; i < min(3, static_cast<int>(all_dragons.size())); i++) {
            enemies.push_back(move(all_dragons[i]));
        }

        ask_new();
    }

    Enemy* current() {
        if (cur < static_cast<int>(enemies.size())) return enemies[cur].get();
        return nullptr;
    }

    void ask_new() {
        if (finished) return;
        auto* e = current();
        if (!e) { finished = true; win = true; last_question = "Все враги повержены!"; return; }
        last_question = e->name() + " спрашивает: " + e->question();
        // Добавляем вопрос в историю
        full_history += "➤ " + last_question + "\n";
    }

    // В структуре Game
    void step_answer(const string& ans) {
        if (finished) return;
        auto* e = current();
        if (!e) { finished = true; win = true; return; }

        // Добавляем информацию о ходе в историю
        string action;
        bool ok = e->check_answer(ans);
        if (ok) {
            hero.hit(*e);
            hero.add_exp(5);
            action = "✅ Верно! Герой атакует (" + to_string(hero.atk()) + ")";
            full_history += "✅ Герой отвечает верно и атакует дракона!\n";
            if (!e->is_alive()) {
                action += " и побеждает врага!";
                full_history += "🎯 Герой побеждает " + e->name() + "!\n";
                ++cur;
            }
        } else {
            e->hit(hero);
            action = "❌ Неверно. " + e->name() + " атакует героя (" + to_string(e->atk()) + ")";
            full_history += "❌ Неверный ответ! " + e->name() + " атакует героя!\n";
            if (!hero.is_alive()) {
                finished = true; win = false;
                last_question.clear();
                full_history += "💀 Герой пал в бою... Игра окончена.\n";
            }
        }
        // Сохраняем все действия в истории
        last_info += action + "\n";

        if (!finished) ask_new();
        else if (win) {
            last_question = "Победа! Все враги повержены.";
            full_history += "🎉 ПОБЕДА! Все драконы повержены! 🎉\n";
        }
        else last_question = "Поражение. Герой пал.";
    }
};

// ========= СЕРВЕР =========
static string json_escape(const string& s) {
    string out; out.reserve(s.size() + 8);
    for (char c : s) {
        switch (c) {
            case '\"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default: out += c;
        }
    }
    return out;
}

int main() {
    Game::instance().reset();

    httplib::Server svr;

    // CORS
    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Headers", "Content-Type"},
        {"Access-Control-Allow-Methods", "GET,POST,OPTIONS"}
    });
    svr.Options(R"(.*)", [](const httplib::Request& req, httplib::Response& res) { res.status = 200; });

    // Статика (фронт)
    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("<meta http-equiv='refresh' content='0; url=/index.html'>", "text/html");
    });

    // Состояние
    svr.Get("/state", [](const httplib::Request&, httplib::Response& res) {
        auto& G = Game::instance();
        auto* e = G.current();
        string ename = e ? e->name() : string("-");
        string ecolor = e ? e->color() : string("-");
        int ehp = e ? e->hp() : 0;
        int eatk = e ? e->atk() : 0;
        string body = string("{") +
            "\"hero\":{\"name\":\"" + json_escape(G.hero.name()) + "\",\"hp\":" + to_string(G.hero.hp()) + ",\"atk\":" + to_string(G.hero.atk()) + ",\"exp\":" + to_string(G.hero.exp()) + "}," +
            "\"enemy\":{\"name\":\"" + json_escape(ename) + "\",\"color\":\"" + json_escape(ecolor) + "\",\"hp\":" + to_string(ehp) + ",\"atk\":" + to_string(eatk) + "}," +
            "\"question\":\"" + json_escape(G.last_question) + "\"," +
            "\"info\":\"" + json_escape(G.last_info) + "\"," +
            "\"history\":\"" + json_escape(G.full_history) + "\"," + // Полная история
            "\"finished\":" + (G.finished ? "true" : "false") + "," +
            "\"win\":" + (G.win ? "true" : "false") +
        "}";
        res.set_content(body, "application/json; charset=utf-8");
    });

    // Ответ игрока
    svr.Post("/answer", [](const httplib::Request& req, httplib::Response& res) {
        string s = req.body;
        string key = "\"answer\"";
        size_t p = s.find(key);
        string val;
        if (p != string::npos) {
            size_t q = s.find(':', p);
            size_t a = s.find('"', q + 1);
            size_t b = s.find('"', a + 1);
            if (a != string::npos && b != string::npos) val = s.substr(a + 1, b - a - 1);
        }
        Game::instance().step_answer(val);
        res.set_content("{\"ok\":true}", "application/json");
    });

    svr.Post("/newgame", [](const httplib::Request&, httplib::Response& res) {
        Game::instance().reset();
        res.set_content("{\"started\":true}", "application/json");
    });

    svr.set_mount_point("/", "./www");

    cout << "Server on http://0.0.0.0:8080\n";
    svr.listen("0.0.0.0", 8080);
    return 0;
}