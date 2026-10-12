#include "data_parser.hpp"
#include "../core/game.hpp"
#include <stdexcept>

DataParser::DataParser(Game* p_game) : m_game(p_game) {}

std::unordered_map<std::string, DataBlock> DataParser::loadData(bool& pr_successful, std::string p_filePath, bool p_extensionIsInPath)
{
    std::string l_filePath = p_filePath;
    if (!p_extensionIsInPath) l_filePath += ".data";

    std::vector<std::string> l_fileData = m_game->getAssetManager()->getTextFromFile(l_filePath);

    std::unordered_map<std::string, DataBlock> lr_blocks = getBlocksFromFileData(pr_successful, l_fileData);
    if (!pr_successful)
    {
        std::cerr << "ERROR: Failed to parse file for data. File path is '" << l_filePath << "'\n";
        return {};
    }

    return lr_blocks;
}

std::unordered_map<std::string, DataBlock> DataParser::getBlocksFromFileData(bool& pr_successful, std::vector<std::string>& p_fileData)
{
    std::unordered_map<std::string, DataBlock> lr_blocks;

    int l_indentationSpaces = 0;

    std::string l_currBlock = "none";
    std::vector<std::string> l_currBlockLines;

    for (auto i_line : p_fileData)
    {
        i_line = i_line.substr(l_indentationSpaces);

        if (l_currBlock == "none")
        {
            auto l_openBracket = i_line.find(" {");
            if (l_openBracket == std::string::npos)
            {
                std::cerr << "ERROR: Cannot parse file for data. A line outside of a block must start a block.\n";
                pr_successful = false;
                return {};
            }

            l_currBlock = i_line.substr(0, l_openBracket);
            continue;
        }

        if (i_line == "}")
        {
            lr_blocks[l_currBlock] = extractBlockFromData(pr_successful, l_currBlock, l_currBlockLines);
            if (!pr_successful) return {};

            l_currBlock = "none";
            l_currBlockLines.clear();
            continue;
        }

        l_currBlockLines.emplace_back(i_line);
    }

    return lr_blocks;
}

DataBlock DataParser::extractBlockFromData(bool& pr_successful, std::string p_blockName, std::vector<std::string> p_blockLines)
{
    DataBlock lr_block = {p_blockName};

    int l_indentationSpaces = 4;

    std::string l_currEntry = "none";
    std::vector<std::string> l_currEntryLines;

    for (auto i_line : p_blockLines)
    {
        i_line = i_line.substr(l_indentationSpaces);

        if (l_currEntry == "none")
        {
            auto l_openBracket = i_line.find(" {");
            if (l_openBracket == std::string::npos)
            {
                std::cerr << "ERROR: Cannot parse file for data. A line inside a block and outside an entry must start an entry.\n";
                pr_successful = false;
                return {};
            }

            l_currEntry = i_line.substr(0, l_openBracket);
            continue;
        }

        if (i_line == "}")
        {
            lr_block.entries[l_currEntry] = extractEntryFromData(pr_successful, l_currEntryLines);
            if (!pr_successful) return {};

            l_currEntry = "none";
            l_currEntryLines.clear();
            continue;
        }

        l_currEntryLines.emplace_back(i_line);
    }

    return lr_block;
}

DataEntry DataParser::extractEntryFromData(bool& pr_successful, std::vector<std::string> p_entryLines)
{
    DataEntry lr_entry;

    int l_indentationSpaces = 4;

    std::string l_currVariable = "none";
    std::string l_currValue = "";

    for (auto i_line : p_entryLines)
    {
        i_line = i_line.substr(l_indentationSpaces);

        std::string l_operatorString = " = ";
        auto l_operator = i_line.find(l_operatorString);
        if (l_operator == std::string::npos)
        {
            std::cerr << "ERROR: Cannot parse file for data. A line inside an entry must set a variable with ' = '.\n";
            pr_successful = false;
            return {};
        }

        l_currVariable = i_line.substr(0, l_operator);
        l_currValue = i_line.substr(l_operator + l_operatorString.size());

        lr_entry.variables[l_currVariable] = l_currValue;
    }

    return lr_entry;
}