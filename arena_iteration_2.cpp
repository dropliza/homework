#include <iostream>

class Fighter {
private:
    const char* name;
    int health;
    int maxHealth;
    int damage;

public:
    Fighter(const char* name, int health, int damage)
        : name(name), health(health), maxHealth(health), damage(damage) {}

    void TakeDamage(int amount) {
        health -= amount;
        if (health < 0)
            health = 0;
    }

    void Attack(Fighter& target) const {
        target.TakeDamage(damage);
    }

    bool IsAlive() const {
        return health > 0;
    }

    void PrintStatus() const {
        std::cout << name << ": " << health << "/" << maxHealth
                  << " здоровья, урон: " << damage << '\n';
    }
};

void PrintFighter(const Fighter& fighter) {
    fighter.PrintStatus();
}

class Game {
private:
    Fighter player;
    Fighter enemies[3];
    Fighter* currentTarget = nullptr;

public:
    Game()
        : player("Игрок", 30, 8),
          enemies{{"Противник 1", 12, 3},
                  {"Противник 2", 18, 4},
                  {"Противник 3", 24, 5}} {}

    bool TrySelectTarget(int number, Fighter*& outTarget) {
        outTarget = nullptr;
        if (number < 1 || number > 3)
            return false;
        if (!enemies[number - 1].IsAlive())
            return false;
        outTarget = &enemies[number - 1];
        return true;
    }

    bool AllEnemiesDefeated() const {
        for (int i = 0; i < 3; i++)
            if (enemies[i].IsAlive())
                return false;
        return true;
    }

    void Run() {
        while (player.IsAlive() && !AllEnemiesDefeated()) {
            PrintFighter(player);
            for (int i = 0; i < 3; i++)
                PrintFighter(enemies[i]);

            std::cout << "Цель: ";
            if (currentTarget != nullptr)
                currentTarget->PrintStatus();
            else
                std::cout << "не выбрана\n";
            std::cout << "1 - выбрать цель, 2 - атаковать, 0 - выйти\n";

            int command;
            if (!(std::cin >> command))
                return;
            if (command == 0)
                return;

            if (command == 1) {
                std::cout << "Введите номер противника (1-3):\n";
                int number;
                if (!(std::cin >> number))
                    return;
                if (TrySelectTarget(number, currentTarget))
                    std::cout << "Цель выбрана\n";
                else
                    std::cout << "Нельзя выбрать эту цель\n";
            }
            else if (command == 2) {
                if (currentTarget == nullptr) {
                    std::cout << "Сначала выберите цель\n";
                    continue;
                }
                player.Attack(*currentTarget);
                if (currentTarget->IsAlive())
                    currentTarget->Attack(player);
                else
                    currentTarget = nullptr;
            }
            else {
                std::cout << "Неизвестная команда\n";
            }
        }

        PrintFighter(player);
        for (int i = 0; i < 3; i++)
            PrintFighter(enemies[i]);
        if (player.IsAlive())
            std::cout << "Победа!\n";
        else
            std::cout << "Поражение!\n";
    }
};

int main() {
    Game game;
    game.Run();
    return 0;
}
