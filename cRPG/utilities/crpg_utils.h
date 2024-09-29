#pragma once
#include<vector>
#include <string>
#include <Windows.h>
#include "../Item.h"
#include "../Equipment.h"

// Template function to remove an element from a vector at a specific index.
// 'T' represents the type of elements stored in the vector.
template <typename T>
void remove(std::vector<T>& vec, size_t index)
{
    // Check if the index is within the valid range (0 <= index < vec.size()).
    if (index >= vec.size()) {
        // If the index is out of bounds, do nothing or you may throw an exception.
        return;
    }

    // The erase function removes an element at the specified position.
    // vec.begin() gives an iterator to the first element of the vector.
    // Adding 'index' to vec.begin() advances the iterator to the element at 'index'.
    vec.erase(vec.begin() + index);

    // After erasing, all elements after the removed one shift to the left,
    // and the vector size decreases by one.
}

std::wstring CharToWide(const char* src);
std::string WideToStr(const std::wstring& wstr);
std::wstring PadNumbers(int num);

Item::ItemType ItemTypeFromString(const std::string& item_type);
Equipment::EquipType EquipTypeFromString(const std::string& equip_type);
WeaponProperties::WeaponType WeaponTypeFromString(const std::string& weapon_type);
ArmourProperties::ArmourType ArmourTypeFromString(const std::string& armour_type);
StatModifier::ModifierType ModifierTypeFromString(const std::string& modifier_type);
StatModifier::ElementalType ElementalTypeFromString(const std::string& elemental_type);