#include <bits/stdc++.h>
#include "httplib.h"
#include "game.h"

static std::string JsonEscape(const std::string& s) {
  std::string result;
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

    std::string enemy_name = enemy ? enemy->GetName() : "-";
    std::string enemy_color = enemy ? enemy->GetColor() : "-";
    int enemy_hp = enemy ? enemy->hp() : 0;
    int enemy_atk = enemy ? enemy->atk() : 0;

    std::string json_body = std::string("{") +
        "\"hero\":{\"name\":\"" + JsonEscape(game.hero().name()) +
        "\",\"hp\":" + std::to_string(game.hero().hp()) +
        ",\"atk\":" + std::to_string(game.hero().atk()) +
        ",\"exp\":" + std::to_string(game.hero().exp()) + "}," +
        "\"enemy\":{\"name\":\"" + JsonEscape(enemy_name) +
        "\",\"color\":\"" + JsonEscape(enemy_color) +
        "\",\"hp\":" + std::to_string(enemy_hp) +
        ",\"atk\":" + std::to_string(enemy_atk) + "}," +
        "\"question\":\"" + JsonEscape(game.last_question()) + "\"," +
        "\"info\":\"" + JsonEscape(game.last_info()) + "\"," +
        "\"history\":\"" + JsonEscape(game.full_history()) + "\"," +
        "\"finished\":" + (game.finished() ? "true" : "false") + "," +
        "\"win\":" + (game.win() ? "true" : "false") + "}";

    res.set_content(json_body, "application/json; charset=utf-8");
  });

  // Answer endpoint
  server.Post("/answer", [](const httplib::Request& req, httplib::Response& res) {
    std::string body = req.body;
    std::string key = "\"answer\"";
    size_t pos = body.find(key);
    std::string answer_value;

    if (pos != std::string::npos) {
      size_t colon_pos = body.find(':', pos);
      size_t quote_start = body.find('"', colon_pos + 1);
      size_t quote_end = body.find('"', quote_start + 1);
      if (quote_start != std::string::npos && quote_end != std::string::npos) {
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

  std::cout << "Server running on http://0.0.0.0:8080\n";
  server.listen("0.0.0.0", 8080);
  return 0;
}