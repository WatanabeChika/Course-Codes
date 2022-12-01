#ifndef PET_H_INCLUDED
#define PET_H_INCLUDED

#include <vector>
#include <string>

enum PetType {W, L, G};
enum SkillName {Tackle, Leaf, Flame, Stream} skn;
enum Kind {Normal, Grass, Fire, Water};


class PetSkill {
public:
    PetSkill(SkillName name);

    Kind getSkillKind() const;
    int getPower() const;

private:
    Kind skillKind;
    int power;
};


class Pet {
public:
    Pet(PetType choose_type);
    Pet();
    ~Pet();
    void initializePet();

    int getHP() const;
    int getAtk() const;
    int getDfc() const;
    int getSkillNum() const;
    PetType getType() const;
    PetSkill* getSkill(int num) const;

    char printType() const;
    std::string printSkill(int num) const;
private:
    int HP, atk, dfc;
    PetType type;
    std::vector<PetSkill*> skills;
};

#endif