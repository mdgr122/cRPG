#pragma once
#include "Parser.h"
#include "../Equipment.h"

class EquipmentLoader : public Parser<Equipment>
{
public:
    EquipmentLoader(const std::string &filepath, bool weapons = true);
    ~EquipmentLoader() override;

    std::shared_ptr<Equipment>
    CreateObjectFromFile(const std::string &objName) override;
    // Override bc virtual func from parser

private:
    std::string m_sFilepath;
    bool m_bWeaponLoader;

    WeaponProperties CreateWeaponProperties(tinyxml2::XMLElement *xmlElement);
    ArmourProperties CreateArmourProperties(tinyxml2::XMLElement *xmlElement);
    StatModifier CreateStatModifier(tinyxml2::XMLElement *xmlElement);
};
