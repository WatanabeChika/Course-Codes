#include "pet.h"

PetSkill::PetSkill(SkillName name)
: skillKind(Kind(name)), power(20)
{
    
}

Kind PetSkill::getSkillKind() const
{
    return skillKind;
}

int PetSkill::getPower() const
{
    return power;
}



Pet::Pet(PetType choose_type)
: type(choose_type)
{
    initializePet();
}

Pet::Pet()
{

}

void Pet::initializePet()
{   
    PetSkill* tkl = new PetSkill(Tackle);
    PetSkill* lf = new PetSkill(Leaf);
    PetSkill* flm = new PetSkill(Flame);
    PetSkill* strm = new PetSkill(Stream);
    switch(type)
    {
    case W:
        HP = 110; atk = 10; dfc = 10; skills = {tkl, lf};
        break;
    case L:
        HP = 100; atk = 11; dfc = 10; skills = {tkl, flm};
        break;
    default:
        HP = 100; atk = 10; dfc = 11; skills = {tkl, strm};
        break;
    }
}

int Pet::getHP() const
{
    return HP;
}

int Pet::getAtk() const
{
    return atk;
}

int Pet::getDfc() const
{
    return dfc;
}

int Pet::getSkillNum() const
{
    return skills.size();
}

PetType Pet::getType() const
{
    return type;
}

PetSkill* Pet::getSkill(int num) const
{
    return skills[num-1];
}

char Pet::printType() const
{
    switch (type)
    {
    case PetType::W: return 'W';
    case PetType::L: return 'L';
    default: return 'G';
    }
}

std::string Pet::printSkill(int num) const
{
    switch (skills[num]->getSkillKind())
    {
        case SkillName::Tackle: return "Tackle";
        case SkillName::Leaf: return "Leaf";
        case SkillName::Flame: return "Flame";
        default: return "Stream";
    }
}

Pet::~Pet()
{
    for (PetSkill* i : skills)
        delete i;
}