#include "attacker.h"

Attacker::Attacker(int hp, int atk) : health_(hp), attack_(atk) {}

bool Attacker::IsAlive() const { return health_ > 0; }

void Attacker::Hit(Attacker& target) const {
    target.health_ = std::max(0, target.health_ - attack_);
}

int Attacker::hp() const { return health_; }
int Attacker::atk() const { return attack_; }

Hero::Hero(std::string name, int hp, int atk)
    : Attacker(hp, atk), name_(std::move(name)) {}

bool Hero::IsEnemy() const { return false; }

const std::string& Hero::name() const { return name_; }
int Hero::exp() const { return experience_; }
void Hero::AddExp(int value) { experience_ += value; }