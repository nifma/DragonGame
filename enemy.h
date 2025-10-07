// enemy.h
#ifndef ENEMY_H
#define ENEMY_H

#include "attacker.h"
#include <string>
#include <memory>

class Enemy : public Attacker {
 public:
  using Attacker::Attacker;
  bool IsEnemy() const final;

  virtual std::string GetName() const = 0;
  virtual std::string GetColor() const = 0;
  virtual std::string GenerateQuestion() = 0;
  virtual bool CheckAnswer(const std::string& answer) const = 0;
};

class Dragon : public Enemy {
 public:
  Dragon(std::string color, int hp = 15, int atk = 5);
  std::string GetColor() const override;
  std::string GetName() const override;

 protected:
  std::string color_;
  mutable int a_, b_, answer_;
};

class YellowDragon : public Dragon {
 public:
  YellowDragon();
  std::string GenerateQuestion() override;
  bool CheckAnswer(const std::string& input) const override;
};

class BlueDragon : public Dragon {
 public:
  BlueDragon();
  std::string GenerateQuestion() override;
  bool CheckAnswer(const std::string& input) const override;
};

class PurpleDragon : public Dragon {
 public:
  PurpleDragon();
  std::string GenerateQuestion() override;
  bool CheckAnswer(const std::string& input) const override;
};

class GreenDragon : public Dragon {
 public:
  GreenDragon();
  std::string GenerateQuestion() override;
  bool CheckAnswer(const std::string& input) const override;
};

class RedDragon : public Dragon {
 public:
  RedDragon();
  std::string GenerateQuestion() override;
  bool CheckAnswer(const std::string& input) const override;
};

class Goblin : public Enemy {
 public:
  Goblin(std::string type, int hp = 12, int atk = 4);
  std::string GetColor() const override;
  std::string GetName() const override;

 protected:
  std::string type_;
  mutable int a_, b_, c_, answer_;
};

class MathGoblin : public Goblin {
 public:
  MathGoblin();
  std::string GenerateQuestion() override;
  bool CheckAnswer(const std::string& input) const override;
};

class RiddleGoblin : public Goblin {
 public:
  RiddleGoblin();
  std::string GenerateQuestion() override;
  bool CheckAnswer(const std::string& input) const override;
};

class TrickGoblin : public Goblin {
 public:
  TrickGoblin();
  std::string GenerateQuestion() override;
  bool CheckAnswer(const std::string& input) const override;
};

#endif