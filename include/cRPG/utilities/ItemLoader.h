#pragma once
#include "Parser.h"
#include "../Item.h"

class ItemLoader : public Parser<Item>
{
public:
    ItemLoader(const std::string &filepath);
    ~ItemLoader() override;

    std::shared_ptr<Item>
    CreateObjectFromFile(const std::string &objName) override;

private:
    std::string m_sFilepath;
};
