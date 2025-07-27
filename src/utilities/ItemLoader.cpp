#include "utilities/ItemLoader.h"
#include <cassert>
#include "Logger.h"
#include "Potion.h"
#include "utilities/ItemCreator.h"
#include "utilities/crpg_utils.h"

using namespace tinyxml2;

ItemLoader::ItemLoader(const std::string &filepath)
    : m_sFilepath{filepath}
{
    m_pXMLDoc = std::make_unique<tinyxml2::XMLDocument>();
}

ItemLoader::~ItemLoader()
{}


std::shared_ptr<Item> ItemLoader::CreateObjectFromFile(const std::string &objName)
{
    if (LoadFile(m_sFilepath) != XML_SUCCESS)
    {
        std::string error{m_pXMLDoc->ErrorStr()};
        CRPG_ERROR("Failed to load Item file. " + m_sFilepath + error);
        return nullptr;
    }

    // Get root element first.
    XMLElement *pRootElement = m_pXMLDoc->RootElement();
    if (!pRootElement)
    {
        std::string error{m_pXMLDoc->ErrorStr()};
        CRPG_ERROR("Failed to get the root element "+ error);
        return nullptr;
    }

    // Get the <Items>
    XMLElement *pItems = pRootElement->FirstChildElement("Items");

    if (!pItems)
    {
        std::string error{m_pXMLDoc->ErrorStr()};
        CRPG_ERROR("Failed to get the items " + error);
        return nullptr;
    }

    // Now we are looking at the <Item>'s, so we will wantto loop until the next sibling is nullptr
    XMLElement *pItem = pItems->FirstChildElement("Item");
    if (!pItem)
    {
        std::string error{m_pXMLDoc->ErrorStr()};
        CRPG_ERROR("Failed to get first item.  " + error);
        return nullptr;
    }

    std::shared_ptr<Item> newItem{nullptr};

    while (pItem)
    {
        XMLElement *pName = pItem->FirstChildElement("Name");
        if (!pName)
        {
            std::string error{m_pXMLDoc->ErrorStr()};
            CRPG_ERROR("Failed to get the item name.  " + error);
            return nullptr;
        }

        // Get the item name
        std::string name{pName->GetText()};

        if (name != objName)
        {
            // Then move to next sibling element
            pItem = pItem->NextSiblingElement("Item");
            continue;
        }

        // If same, then get item type
        XMLElement *pType = pItem->FirstChildElement("Type");
        if (!pType)
        {
            std::string error{m_pXMLDoc->ErrorStr()};
            CRPG_ERROR("Failed to get first item type.  " + error);
            return nullptr;
        }

        Item::ItemType type = ItemTypeFromString(pType->GetText());

        // Get the item description
        XMLElement *pDesc = pItem->FirstChildElement("Description");
        if (!pDesc)
        {
            std::string error{m_pXMLDoc->ErrorStr()};
            CRPG_ERROR("Failed to get item description.  " + error);
            return nullptr;
        }

        std::wstring desc = CharToWide(pDesc->GetText());

        // Get  the item value
        XMLElement *pValue = pItem->FirstChildElement("Value");
        if (!pValue)
        {
            std::string error{m_pXMLDoc->ErrorStr()};
            CRPG_ERROR("Failed to get the item value.  " + error);
            return nullptr;
        }

        int value = atoi(pValue->GetText());

        if (value < -1)
        {
            CRPG_ERROR("Value of item cannot be negative");
            return nullptr;
        }

        // Get the item buy price
        XMLElement *pBuyPrice = pItem->FirstChildElement("BuyPrice");
        if (!pBuyPrice)
        {
            std::string error{m_pXMLDoc->ErrorStr()};
            CRPG_ERROR("Failed to get the item buy price.  " + error);
            return nullptr;
        }

        int buy_price = atoi(pBuyPrice->GetText());
        if (buy_price < 1)
        {
            CRPG_ERROR("Item buy price cannot be negative or 0");
            return nullptr;
        }

        newItem = ItemCreator::CreateItem(type, CharToWide(name.c_str()), desc, value, buy_price);
        return newItem;
    }

    // If we get to this point, then ObjName does not exist!
    assert(newItem);

    return nullptr;
}
