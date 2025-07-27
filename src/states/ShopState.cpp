#include "states/ShopState.h"
#include "states/StateMachine.h"
#include "Console.h"
#include "Party.h"
#include "Keyboard.h"
#include "Equipment.h"
#include "Item.h"
#include "utilities/crpg_utils.h"
#include "utilities/ShopLoader.h"
#include "utilities/ShopParameters.h"
#include "utilities/EquipmentLoader.h"
#include "utilities/ItemCreator.h"
#include <cassert>

using namespace std::placeholders;

ShopState::ShopState(Party& party, Console& console, StateMachine& stateMachine, Keyboard& keyboard, const std::string& shopFilepath)
    : m_Party(party), m_Console(console), m_StateMachine(stateMachine), m_Keyboard(keyboard)
    , m_pShopParameters{nullptr}
    , m_ShopChoiceSelector{
        console, keyboard, {L"BUY", L"SELL", L"EXIT"},
        SelectorParams{42, 10, 3}
    }
    , m_BuySellSelector{
        console, keyboard, {L"OK", L"CANCEL"},
        SelectorParams{80, 30, 2, 0, 0, 10}
    }
    , m_EquipmentSelector{
        console, keyboard, std::vector<std::shared_ptr<Equipment>>(),
        SelectorParams{30, 18, 1}
    }
    , m_ItemSelector{
        console, keyboard, std::vector<std::shared_ptr<Item>>(),
        SelectorParams{30, 18, 1} 
    }
    , m_Quantity{ 0 }
    , m_Price{ 0 }
    , m_ScreenWidth{ console.GetScreenWidth() }
    , m_ScreenHeight{ console.GetScreenHeight() }
    , m_CenterScreenW{ console.GetHalfWidth() }
    , m_PanelBarX{ m_CenterScreenW - (PANEL_BARS / 2) }
    , m_bInShopSelect{ true }
    , m_bInItemBuy{ false }
    , m_bInItemSell{ false }
    , m_bSetFuncs{ false }
    , m_bIsEquipmentShop{ false }
    , m_bBuySellItem{ false }
    , m_bExitShop{ false }
    , m_AvailableSellQuantity{0}
{
    ShopLoader shopLoader{};
    m_pShopParameters = std::move(shopLoader.CreateShopParametersFromFile(shopFilepath));
    
    // Checking to see if shop parameters are created
    if (!m_pShopParameters)
    { 
        CRPG_LOG("Failed to load shop parameters from [" + shopFilepath + "]");
        m_bExitShop = true;
    }

    switch (m_pShopParameters->shopType)
    {
    case ShopParameters::ShopType::WEAPON:
    case ShopParameters::ShopType::ARMOUR:
    case ShopParameters::ShopType::ACCESSORY:
        m_EquipmentSelector.SetData(m_pShopParameters->inventory->GetEquipment());
        m_bIsEquipmentShop = true;
        break;
    case ShopParameters::ShopType::ITEM:
        m_ItemSelector.SetData(m_pShopParameters->inventory->GetItems());
        m_bIsEquipmentShop = false;
        break;
    case ShopParameters::ShopType::NOT_A_SHOP:
        //assert(false, &"Shop type must be set for the shop state");
        break;
    }

    // Because these are wstrings, we don't have to set a draw function because wstring has a default draw function, but we have to set the selection for the index still
    m_ShopChoiceSelector.SetSelectionFunc(std::bind(&ShopState::OnShopMenuSelect, this, _1, _2));
    m_BuySellSelector.SetSelectionFunc(std::bind(&ShopState::BuySellOptions, this, _1, _2));

    m_BuySellSelector.HideCursor();

}



ShopState::~ShopState()
{
}

void ShopState::OnEnter()
{
    m_Console.ClearBuffer();
}

void ShopState::OnExit()
{
    m_Console.ClearBuffer();
}

void ShopState::Update()
{
    if (m_bExitShop) 
    {
        m_StateMachine.PopState();
        return;
    }

    if (m_bBuySellItem)
    {
        if (m_bInItemBuy)
            UpdateBuyQuantity(m_Price);
        else if (m_bInItemSell)
            UpdateSellQuantity(m_AvailableSellQuantity);
    }
    
}

void ShopState::Draw()
{
    DrawShop();
    m_ShopChoiceSelector.Draw();

    // Check if the functions are set for buy and sell -- If the funcs are set, then we draw. IF not, then we first set the funcs
    if ((m_bInItemBuy || m_bInItemSell) && m_bSetFuncs)
    {
        if (m_bIsEquipmentShop)
            m_EquipmentSelector.Draw();
        else
            m_ItemSelector.Draw();
        
        DrawItemsBox();
    
    }

    if (m_bBuySellItem)
    {
        DrawBuyItems();
        m_BuySellSelector.Draw();
    }

    m_Console.Draw();
}

void ShopState::ProcessInputs()
{

    if (m_bInShopSelect)
    {
        if (m_Keyboard.IsKeyPressed(KEY_BACKSPACE))
        {
            m_StateMachine.PopState();
            return;
        }
        m_ShopChoiceSelector.ProcessInputs();
        return;
    }

    if (m_bInItemBuy && !m_bBuySellItem)
    {
        // Check if buy sell functions have been set - If false, then set them.
        if (!m_bSetFuncs)
        {
            if (m_bIsEquipmentShop)
            {
                // if yes, then set equip selectors
                m_EquipmentSelector.SetSelectionFunc(std::bind(&ShopState::OnBuyEquipmentSelect, this, _1, _2));
                m_EquipmentSelector.SetDrawFunc(std::bind(&ShopState::RenderBuyEquipment, this, _1, _2, _3));
                m_EquipmentSelector.SetData(m_pShopParameters->inventory->GetEquipment());

            }
            else
            {
                m_ItemSelector.SetSelectionFunc(std::bind(&ShopState::OnBuyItemSelect, this, _1, _2));
                m_ItemSelector.SetDrawFunc(std::bind(&ShopState::RenderBuyItems, this, _1, _2, _3));
                m_ItemSelector.SetData(m_pShopParameters->inventory->GetItems());
            }

            m_bSetFuncs = true;
        }

        if (m_Keyboard.IsKeyPressed(KEY_BACKSPACE))
        {
            ResetSelections();
            return;
        }
    }
    else if (m_bInItemSell && !m_bBuySellItem)
    {
        // Items sell, so we're not looking at party inventory

                // Check if buy sell functions have been set - If false, then set them.
        if (!m_bSetFuncs)
        {
            if (m_bIsEquipmentShop)
            {
                // if yes, then set equip selectors
                m_EquipmentSelector.SetSelectionFunc(std::bind(&ShopState::OnSellEquipmentSelect, this, _1, _2));
                m_EquipmentSelector.SetDrawFunc(std::bind(&ShopState::RenderSellEquipment, this, _1, _2, _3));
                m_EquipmentSelector.SetData(m_Party.GetInventory().GetEquipment());

            }
            else
            {
                m_ItemSelector.SetSelectionFunc(std::bind(&ShopState::OnSellItemSelect, this, _1, _2));
                m_ItemSelector.SetDrawFunc(std::bind(&ShopState::RenderSellItems, this, _1, _2, _3));
                m_ItemSelector.SetData(m_Party.GetInventory().GetItems());
            }

            m_bSetFuncs = true;
        }

        if (m_Keyboard.IsKeyPressed(KEY_BACKSPACE))
        {
            ResetSelections();
            return;
        }
    }

    if (!m_bBuySellItem && m_bIsEquipmentShop && (m_bInItemBuy || m_bInItemSell))
    {
        // then process equip selector
        m_EquipmentSelector.ProcessInputs();
    }
    else if (!m_bBuySellItem && !m_bIsEquipmentShop && (m_bInItemBuy || m_bInItemSell))
    {
        m_ItemSelector.ProcessInputs();
    }
    else if (m_bBuySellItem)
    {
        m_BuySellSelector.ProcessInputs();
    }

}

bool ShopState::Exit()
{
    return false;
}

void ShopState::DrawShop()
{
    // Draw Opening Bar
    m_Console.DrawPanelHorz(m_PanelBarX - 1, 1, PANEL_BARS + 2, BLUE);

    if (m_pShopParameters->shopType == ShopParameters::ShopType::ITEM)
    {
        const int pos_x = m_CenterScreenW - 25;
        m_Console.Write(pos_x, 3, L" ___ _                   ____  _                 ", GREEN);
        m_Console.Write(pos_x, 4, L"|_ _| |_ ___ _ __ ___   / ___|| |__   ___  _ __  ", GREEN);
        m_Console.Write(pos_x, 5, L" | || __/ _ \\ '_ ` _ \\  \\___ \\| '_ \\ / _ \\| '_ \\ ", GREEN);
        m_Console.Write(pos_x, 6, L" | || ||  __/ | | | | |  ___) | | | | (_) | |_) |", GREEN);
        m_Console.Write(pos_x, 7, L"|___|\\__\\___|_| |_| |_| |____/|_| |_|\\___/| .__/ ", GREEN);
        m_Console.Write(pos_x, 8, L"                                          |_|   ", GREEN);
    }
    else if (m_pShopParameters->shopType == ShopParameters::ShopType::WEAPON)
    {
        const int pos_x = m_CenterScreenW - 33;
        m_Console.Write(pos_x, 3, L"__        __                             ____  _                 ", GREEN);
        m_Console.Write(pos_x, 4, L"\\ \\      / /__  __ _ _ __   ___  _ __   / ___|| |__   ___  _ __  ", GREEN);
        m_Console.Write(pos_x, 5, L" \\ \\ /\\ / / _ \\/ _` | '_ \\ / _ \\| '_ \\  \\___ \\| '_ \\ / _ \\| '_ \\ ", GREEN);
        m_Console.Write(pos_x, 6, L"  \\ V  V /  __/ (_| | |_) | (_) | | | |  ___) | | | | (_) | |_) |", GREEN);
        m_Console.Write(pos_x, 7, L"   \\_/\\_/ \\___|\\__,_| .__/ \\___/|_| |_| |____/|_| |_|\\___/| .__/ ", GREEN);
        m_Console.Write(pos_x, 8, L"                    |_|                                   |_|   ", GREEN);
    }
    else if (m_pShopParameters->shopType == ShopParameters::ShopType::ARMOUR)
    {
        const int pos_x = m_CenterScreenW - 31;
        m_Console.Write(pos_x, 3, L"    _                                ____  _                 ", GREEN);
        m_Console.Write(pos_x, 4, L"   / \\   _ __ _ __ ___   ___  _ __  / ___|| |__   ___  _ __  ", GREEN);
        m_Console.Write(pos_x, 5, L"  / _ \\ | '__| '_ ` _ \\ / _ \\| '__| \\___ \\| '_ \\ / _ \\| '_ \\ ", GREEN);
        m_Console.Write(pos_x, 6, L" / ___ \\| |  | | | | | | (_) | |     ___) | | | | (_) | |_) |", GREEN);
        m_Console.Write(pos_x, 7, L"/_/   \\_\\_|  |_| |_| |_|\\___/|_|    |____/|_| |_|\\___/| .__/ ", GREEN);
        m_Console.Write(pos_x, 8, L"                                                      |_|   ", GREEN);
    }

    m_Console.DrawPanelHorz(m_PanelBarX - 1, 9, PANEL_BARS + 2, BLUE);
    m_Console.DrawPanelHorz(m_PanelBarX - 1, 11, PANEL_BARS + 2, BLUE);

    // Draw Closing Bar
    m_Console.DrawPanelHorz(m_PanelBarX - 1, (m_ScreenHeight - 2), PANEL_BARS + 2, BLUE);

    // Draw Right bar
    m_Console.DrawPanelVert(m_PanelBarX - 1, 2, 44, BLUE);
    m_Console.DrawPanelVert(m_PanelBarX + PANEL_BARS, 2, 44, BLUE);
}

void ShopState::DrawBuyItems()
{
    m_Console.Write(80, 25, L"QUANTITY:");
    m_Console.Write(90, 25, std::to_wstring(m_Quantity));
    m_Console.Write(80, 26, L"TOTAL:");
    m_Console.Write(90, 26, std::to_wstring(m_Price * m_Quantity));

    // Draw the box -- X, Y, SIZE_T, COLOUR (UP/DOWN, LEFT/RIGHT, SIZE_T, COLOR)
    m_Console.DrawPanelHorz(75, 23, 26, LIGHT_YELLOW); // TOP
    m_Console.DrawPanelVert(75, 24, 9, LIGHT_YELLOW); // LEFT
    m_Console.DrawPanelVert(100, 24, 9, LIGHT_YELLOW); // RIGHT
    m_Console.DrawPanelHorz(75, 32, 26, LIGHT_YELLOW); // BOTTOM
}

void ShopState::DrawSellItems()
{
}

void ShopState::DrawItemsBox()
{
    m_Console.DrawPanelHorz(25, 14, 41, BLUE);
    m_Console.Write(30, 15, L"ITEM:");
    m_Console.Write(50, 15, L"PRICE:");

    // Draw the box
    m_Console.DrawPanelHorz(26, 16, 40, BLUE);
    m_Console.DrawPanelHorz(26, 40, 40, BLUE);
    m_Console.DrawPanelVert(65, 15, 27, BLUE);
    m_Console.DrawPanelVert(25, 15, 27, BLUE);
    m_Console.DrawPanelHorz(25, 42, 41, BLUE);

    const auto& goldStr = std::to_wstring(m_Party.GetGold());
    m_Console.Write(80, 15, L"GOLD: " + goldStr);
}

void ShopState::ResetSelections()
{
    // Resetting everything back to original faluse
    m_bInItemBuy = false;
    m_bInItemSell = false;
    m_bSetFuncs = false;
    m_bBuySellItem = false;
    m_bInShopSelect = true;

    m_ItemSelector.HideCursor();
    m_EquipmentSelector.HideCursor();
    m_ShopChoiceSelector.ShowCursor();
    m_Console.ClearBuffer();
}

void ShopState::BuyEquipment()
{
    int itemIndex = m_EquipmentSelector.GetIndex();
    const auto& item = m_EquipmentSelector.GetData()[itemIndex];

    // Get item count
    if (item->GetCount() + m_Quantity - 1 > item->GetMaxCount())
        return;


    // Create new item and add to inventory
    auto newItem = ItemCreator::CreateEquipment(
        item->GetType(),
        item->GetWeaponProperties(),
        item->GetArmourProperties(),
        item->GetStatModifier(),
        item->GetName(),
        item->GetDescription(),
        item->GetBuyPrice(),
        item->GetSellPrice()
    );
    assert(newItem && "Failed to create new item");

    if (!newItem)
    {
        CRPG_ERROR("Failed to Create Item");
        return;
    }

    // Get ref to inventory so we can add it
    newItem->Add(m_Quantity - 1);
    auto& inventory = m_Party.GetInventory();

    for (const auto& it : inventory.GetEquipment())
    {
        // Loop trhough equipment and check if we're at max count
        if (newItem->GetName() != it->GetName())
            continue;
        if (newItem->GetCount() + it->GetCount() > it->GetMaxCount())
        {
            return;
        }
        break;
    }

    // Buy the new item
    if (!m_Party.BuyEquipment(m_Quantity * m_Price, std::move(newItem)))
    {
        m_Console.Write(80, 34, L"Failed to buy equipment", RED);
        return;
    }
}

void ShopState::SellEquipment()
{
    int itemIndex = m_EquipmentSelector.GetIndex();
    const auto& item = m_EquipmentSelector.GetData()[itemIndex];
    const auto& count = item->GetCount();

    auto totalSellPrice = item->GetSellPrice() * m_Quantity;
    m_Party.AddGold(totalSellPrice);

    if (count - m_Quantity > 0)
    {
        item->Decrement(m_Quantity);
    }
    else
    {
        remove(m_EquipmentSelector.GetData(), itemIndex);
        remove(m_Party.GetInventory().GetEquipment(), itemIndex);
    }
    // Add gold from the sold item
}

void ShopState::BuyItems()
{
    int itemIndex = m_ItemSelector.GetIndex();
    const auto& item = m_ItemSelector.GetData()[itemIndex];

    // Get item count
    if (item->GetCount() + m_Quantity - 1 > item->GetMaxCount())
        return;

    // Create new item and add to inventory
    auto newItem = ItemCreator::CreateItem(
        item->GetType(),
        item->GetItemName(),
        item->GetItemDescription(),
        item->GetItemValue(), // health, etc
        item->GetBuyPrice()
    );


    // Get ref to inventory so we can add it
    newItem->AddItem(m_Quantity - 1);
    auto& inventory = m_Party.GetInventory();

    for (const auto& it : inventory.GetItems())
    {
        // Loop through equipment and check if we're at max count
        if (newItem->GetItemName() != it->GetItemName())
            continue;
        if (newItem->GetCount() + it->GetCount() > it->GetMaxCount())
        {
            return;
        }
        break;
    }

    // Buy the new item
    if (!m_Party.BuyItem(m_Quantity * m_Price, std::move(newItem)))
    {
        m_Console.Write(80, 34, L"Failed to buy item", RED);
        return;
    }
}

void ShopState::SellItems()
{
    int itemIndex = m_ItemSelector.GetIndex();
    const auto& item = m_ItemSelector.GetData()[itemIndex];
    const auto& count = item->GetCount();

    auto totalSellPrice = item->GetSellPrice() * m_Quantity;
    m_Party.AddGold(totalSellPrice);
    if (count - m_Quantity > 0)
    {
        item->Decrement(m_Quantity);
    }
    else
    {
        remove(m_ItemSelector.GetData(), itemIndex);
        remove(m_Party.GetInventory().GetItems(), itemIndex);
    }
    // Add gold from the sold item
    //if (!m_Party.AddGold(m_Quantity * m_Price))
    //{
    //    m_Console.Write(80, 34, L"Failed to buy item", RED);
    //    return;
    //}
}

void ShopState::OnShopMenuSelect(int index, std::vector<std::wstring> data)
{
    if (index > data.size() - 1 || data.empty())
    {
        return;
    }

    switch (index)
    {
    case 0: // BUY 
        m_bInItemBuy = true;
        break;
    case 1: // SELL
        m_bInItemSell = true;
        break;
    case 3: // EXIT
        m_bExitShop = true;
        break;
    default:
        return;
    }

    m_bInShopSelect = false;
    m_ShopChoiceSelector.HideCursor();

    if (m_bIsEquipmentShop)
        m_EquipmentSelector.ShowCursor();
    else
        m_ItemSelector.ShowCursor();
}

void ShopState::BuySellOptions(int index, std::vector<std::wstring> data)
{
    // Two options: OK [index 0] or Cancel [index 1]
    if (index == 1 || m_Quantity <= 0)
    {
        m_bBuySellItem = false;
        m_BuySellSelector.HideCursor();
        m_Quantity = 0;
        m_Price = 0;
        m_AvailableSellQuantity = 0;
        m_Console.ClearBuffer();
        return;
    }


    // otherwise, check if we're buying or selling
    if (m_bInItemBuy)
    {
        // check if we're in equipment or item shop
        if (m_bIsEquipmentShop)
            BuyEquipment();
        else
            BuyItems();
    }
    else if (m_bInItemSell)
    {
        if (m_bIsEquipmentShop)
            SellEquipment();
        else
            SellItems();
    }
    m_bBuySellItem = false;
    m_BuySellSelector.HideCursor();
    m_Quantity = 0;
    m_Price = 0;
    m_AvailableSellQuantity = 0;
    m_Console.ClearBuffer();
    return;

}

void ShopState::OnBuyItemSelect(int index, std::vector<std::shared_ptr<Item>> data)
{
    if (index > data.size() - 1 || data.empty())
        return;

    const auto& item = data[index];
    const auto& price = item->GetBuyPrice();
    m_bBuySellItem = true;
    m_BuySellSelector.ShowCursor();
    m_Price = price;
}

void ShopState::OnBuyEquipmentSelect(int index, std::vector<std::shared_ptr<Equipment>> data)
{
    if (index > data.size() - 1 || data.empty())
        return;

    const auto& item = data[index];
    const auto& price = item->GetBuyPrice();
    m_bBuySellItem = true;
    m_BuySellSelector.ShowCursor();
    m_Price = price;
}

void ShopState::OnSellItemSelect(int index, std::vector<std::shared_ptr<Item>> data)
{
    if (index > data.size() - 1 || data.empty())
        return;

    const auto& item = data[index];
    const auto& price = item->GetSellPrice();
    m_bBuySellItem = true;
    m_BuySellSelector.ShowCursor();
    m_Price = price;
    m_AvailableSellQuantity = item->GetCount();
}

void ShopState::OnSellEquipmentSelect(int index, std::vector<std::shared_ptr<Equipment>> data)
{
    if (index > data.size() - 1 || data.empty())
        return;

    const auto& item = data[index];
    const auto& price = item->GetSellPrice();
    m_bBuySellItem = true;
    m_BuySellSelector.ShowCursor();
    m_Price = price;
    m_AvailableSellQuantity = item->GetCount();

}

void ShopState::RenderBuyItems(int x, int y, std::shared_ptr<Item> item)
{
    const std::wstring& name = item->GetItemName();
    const std::wstring& price = std::to_wstring(item->GetBuyPrice());

    m_Console.Write(x, y, name);
    m_Console.Write(x + 25, y, price);
}

void ShopState::RenderSellItems(int x, int y, std::shared_ptr<Item> item)
{
    const std::wstring& name = item->GetItemName();
    const std::wstring& price = std::to_wstring(item->GetSellPrice());

    m_Console.Write(x, y, name);
    m_Console.Write(x + 25, y, price);
}

void ShopState::RenderBuyEquipment(int x, int y, std::shared_ptr<Equipment> item)
{
    const std::wstring& name = item->GetName();
    const std::wstring& price = std::to_wstring(item->GetBuyPrice());

    m_Console.Write(x, y, name);
    m_Console.Write(x + 25, y, price);
}

void ShopState::RenderSellEquipment(int x, int y, std::shared_ptr<Equipment> item)
{

    const std::wstring& name = item->GetName();
    const std::wstring& price = std::to_wstring(item->GetBuyPrice());

    m_Console.Write(x, y, name);
    m_Console.Write(x + 25, y, price);
}

void ShopState::UpdateBuyQuantity(int price)
{
    const auto& gold = m_Party.GetGold();

    
    if (m_Keyboard.IsKeyPressed(KEY_W) && gold >= (m_Quantity + 1) * price)
    {
        m_Quantity++;
    }
    else if (m_Keyboard.IsKeyPressed(KEY_S) && m_Quantity > 0)
    {
        m_Quantity--;
        m_Quantity = std::clamp(m_Quantity, 0, 99);
    }
    else
    {
        CRPG_LOG("Else Reached in UpdateBuyQuantity!"); 
        CRPG_LOG("Gold: " + std::to_string(gold) + " -- m_Quantity*price: " + std::to_string(m_Quantity * price));
    }

}

void ShopState::UpdateSellQuantity(int totalAvailable)
{
    const auto& gold = m_Party.GetGold();
    if (m_Keyboard.IsKeyPressed(KEY_W) && m_Quantity < totalAvailable)
    {
        m_Quantity++;
    }
    else if (m_Keyboard.IsKeyPressed(KEY_S) && m_Quantity > 0)
    {
        m_Quantity--;
        m_Quantity = std::clamp(m_Quantity, 0, 99);
    }
}
