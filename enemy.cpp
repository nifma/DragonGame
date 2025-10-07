#include "enemy.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

bool Enemy::IsEnemy() const { return true; }

Dragon::Dragon(std::string color, int hp, int atk)
    : Enemy(hp, atk), color_(std::move(color)) {}

std::string Dragon::GetColor() const { return color_; }
std::string Dragon::GetName() const { return color_ + " Dragon"; }

// YellowDragon
YellowDragon::YellowDragon() : Dragon("Yellow") {}

std::string YellowDragon::GenerateQuestion() {
  a_ = rand() % 5 + 1;
  b_ = rand() % 3 + 2;
  answer_ = a_ * b_;
  return "Вычислите коэффициент производной функции f(x) = " +
         std::to_string(a_) + "x^" + std::to_string(b_);
}

bool YellowDragon::CheckAnswer(const std::string& input) const {
  try {
    return std::stoi(input) == answer_;
  } catch (...) {
    return false;
  }
}

// BlueDragon
BlueDragon::BlueDragon() : Dragon("Blue") {}

std::string BlueDragon::GenerateQuestion() {
  a_ = rand() % 10 + 3;
  b_ = rand() % 4 + 2;
  answer_ = std::pow(a_, b_);
  return "Вычислите " + std::to_string(a_) + " в степени " + std::to_string(b_);
}

bool BlueDragon::CheckAnswer(const std::string& input) const {
  try {
    return std::stoi(input) == answer_;
  } catch (...) {
    return false;
  }
}

// PurpleDragon
PurpleDragon::PurpleDragon() : Dragon("Purple") {}

std::string PurpleDragon::GenerateQuestion() {
  a_ = rand() % 50 + 1;
  b_ = rand() % 20 + 1;
  answer_ = (a_ + b_) / 2;
  return "Вычислите среднее значение между числами " + std::to_string(a_) +
         " и " + std::to_string(b_);
}

bool PurpleDragon::CheckAnswer(const std::string& input) const {
  try {
    return std::stoi(input) == answer_;
  } catch (...) {
    return false;
  }
}

// GreenDragon
GreenDragon::GreenDragon() : Dragon("Green") {}

std::string GreenDragon::GenerateQuestion() {
  a_ = rand() % 20 + 5;
  b_ = rand() % 10 + 1;
  answer_ = a_ % b_;
  return "Найдите остаток от деления " + std::to_string(a_) + " на " +
         std::to_string(b_);
}

bool GreenDragon::CheckAnswer(const std::string& input) const {
  try {
    return std::stoi(input) == answer_;
  } catch (...) {
    return false;
  }
}

// RedDragon
RedDragon::RedDragon() : Dragon("Red") {}

std::string RedDragon::GenerateQuestion() {
  a_ = rand() % 91 + 10;
  b_ = rand() % 21 + 5;
  answer_ = a_ - b_;
  return "Решите уравнение: x + " + std::to_string(b_) + " = " +
         std::to_string(a_) + ". Найдите x";
}

bool RedDragon::CheckAnswer(const std::string& input) const {
  try {
    return std::stoi(input) == answer_;
  } catch (...) {
    return false;
  }
}

// Goblin
Goblin::Goblin(std::string type, int hp, int atk)
    : Enemy(hp, atk), type_(std::move(type)) {}

std::string Goblin::GetColor() const { return "Green"; }
std::string Goblin::GetName() const { return type_ + " Goblin"; }

// MathGoblin
MathGoblin::MathGoblin() : Goblin("Math") {}

std::string MathGoblin::GenerateQuestion() {
  a_ = rand() % 10 + 1;
  b_ = rand() % 10 + 1;
  answer_ = a_ + b_;
  return "Сколько будет " + std::to_string(a_) + " + " + std::to_string(b_) + "?";
}

bool MathGoblin::CheckAnswer(const std::string& input) const {
  try {
    return std::stoi(input) == answer_;
  } catch (...) {
    return false;
  }
}

// RiddleGoblin
RiddleGoblin::RiddleGoblin() : Goblin("Riddle") {}

std::string RiddleGoblin::GenerateQuestion() {
  a_ = rand() % 10 + 1;
  b_ = rand() % 10 + 1;
  answer_ = a_ * b_;
  return "У меня " + std::to_string(a_) + " мешков, в каждом " +
         std::to_string(b_) + " золотых. Сколько всего золотых?";
}

bool RiddleGoblin::CheckAnswer(const std::string& input) const {
  try {
    return std::stoi(input) == answer_;
  } catch (...) {
    return false;
  }
}

// TrickGoblin
TrickGoblin::TrickGoblin() : Goblin("Trick") {}

std::string TrickGoblin::GenerateQuestion() {
  a_ = rand() % 8 + 3;
  b_ = rand() % 5 + 2;
  answer_ = a_ - b_;
  return "Было " + std::to_string(a_) + " печенек, я съел " +
         std::to_string(b_) + ". Сколько осталось?";
}

bool TrickGoblin::CheckAnswer(const std::string& input) const {
  try {
    return std::stoi(input) == answer_;
  } catch (...) {
    return false;
  }
}