#ifndef ATTACKER_H
#define ATTACKER_H

#include <string>

class Attacker {
public:
    Attacker(int hp, int atk);
    virtual ~Attacker() = default;

    bool IsAlive() const;
    void Hit(Attacker& target) const;

    int hp() const;
    int atk() const;

    virtual bool IsEnemy() const = 0;

protected:
    int health_;
    int attack_;
};

class Hero : public Attacker {
public:
    Hero(std::string name, int hp = 30, int atk = 6);
    bool IsEnemy() const override;

    const std::string& name() const;
    int exp() const;
    void AddExp(int value);

private:
    std::string name_;
    int experience_ = 0;
};

#endif