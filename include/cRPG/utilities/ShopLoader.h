#pragma once
#include "Parser.h"
#include "ShopParameters.h"

class ShopLoader : public Parser<ShopParameters>
{
public:
    ShopLoader();
    ~ShopLoader() override;

    std::unique_ptr<ShopParameters> CreateShopParametersFromFile(const std::string &shop_filepath);
    std::shared_ptr<ShopParameters> CreateObjectFromFile(const std::string &objName) override;
};
