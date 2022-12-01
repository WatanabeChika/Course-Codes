#include "engine.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

vector<Pet*> playerPets, enemyPets;
Pet* player;
Pet* enemy;


void initializeGame()
{
    cout << "Welcome to Battle of Pets!" << endl;
    cout << "You have W, L and G. So does enemy." << endl;
    cout << "Select your starting pet (1 for W, 2 for L, 3 for G): ";

    playerPets.push_back(new Pet(PetType::W));
    playerPets.push_back(new Pet(PetType::L));
    playerPets.push_back(new Pet(PetType::G));
    enemyPets.push_back(new Pet(PetType::W));
    enemyPets.push_back(new Pet(PetType::L));
    enemyPets.push_back(new Pet(PetType::G));

    int fircho, enecho;
    cin >> fircho;
    while (fircho > 3 || fircho < 1) {
        cout << "Select your starting pet (1 for W, 2 for L, 3 for G): ";
        cin >> fircho;
    }
    player = playerPets[fircho-1];
    srand(time(nullptr));
    enecho = rand()%3;
    enemy = enemyPets[enecho];

    cout << "You start with " << player->printType() << endl;
    cout << "Enemy starts with " << enemy->printType() << endl;
    cout << "Battle starts!" << endl;
}

void endGame()
{
    if (playerPets.size() > 0) {
        cout << "You Win" << endl;
        for (Pet* i : playerPets)
            delete i;
    }
    else {
        cout << "You Lose" << endl;
        for (Pet* i : enemyPets)
            delete i;
    }
    
}

bool judgeOutRange(int cn, int range)
{
    if (cn < 0 || cn > range) return true;
    else return false;
}

void printHP() 
{
    cout << "Your " << player->printType() << ": HP " << player->getHP();
    cout << " || ";
    cout << "Enemy's " << enemy->printType() << ": HP " << enemy->getHP() << endl;
}

bool changePet()
{
    int choice = -1;
    bool temp = false;

    do {
    cout << "Select your next pet (";
    temp = false;
    for (int i = 0; i < playerPets.size(); ++i) {
        if (playerPets[i]->getType() != player->getType()) {
            if (temp) cout << ", ";
            cout << i+1 << " for " << playerPets[i]->printType();
            temp = true;
        }
    }
    cout << "):";
    cin >> choice;
    }
    while (choice == int(player->getType())+1 || judgeOutRange(choice, playerPets.size()) || choice == 0);

    player = playerPets[choice-1];
    cout << "You send " << player->printType() << endl;

    return false;
}

bool useSkill(int& choice)
{
    bool temp = false;

    do {
        cout << "Select your skill (";
        temp = false;
        for (int i = 0; i < player->getSkillNum(); ++i) {
            if (temp) cout << ", ";
            cout << i+1 << " for " << player->printSkill(i);
            temp = true;
        }
        cout << "):";
        cin >> choice;
    }
    while (judgeOutRange(choice, player->getSkillNum()) || choice == 0);

    return true;
}

int resOrNot()
{
    if (player->getType() == PetType::W) {
        if (enemy->getType() == PetType::G) return -1;
        else if (enemy->getType() == PetType::L) return 1;
        else return 0;
    }
    else if (player->getType() == PetType::G) {
        if (enemy->getType() == PetType::G) return 0;
        else if (enemy->getType() == PetType::L) return -1;
        else return 1;
    }
    else {
        if (enemy->getType() == PetType::G) return 1;
        else if (enemy->getType() == PetType::L) return 0;
        else return -1;
    }
}

bool enemyAction1(int& choice)
{
    if (resOrNot() == 1) choice = 2;
    else choice = 1;
    return true;
}

void doingAttack(bool player, bool enemy)
{

}

void play()
{
    initializeGame();
    cout << "--------------------------------------------------" << endl;

    int round = 1;
    int action = -1;
    int playerskl = -1, enemyskl = -1;
    bool playeratk, enemyatk;

    while(playerPets.size() > 0 || enemyPets.size() > 0)
    {
        enemyatk = enemyAction1(enemyskl);

        cout << "Round " << round << endl;
        while (judgeOutRange(action, playerPets.size()-1)) {
            cout << "Select your action (0 for change, 1 for skill): ";
            cin >> action;
        }

        if (action == 0) {
            if (playerPets.size() > 0) 
                playeratk = changePet();
            else
                cout << "No pet to change" << endl;
        }
        else playeratk = useSkill(playerskl);

        doingAttack(playeratk, enemyatk);
        afterAttack();
        printHP();

        cout << "--------------------------------------------------" << endl;
        ++round;
    }
    endGame();
}