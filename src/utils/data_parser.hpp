#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class Game;

struct DataEntry
{
    std::unordered_map<std::string, std::string> variables;
};

struct DataBlock
{
    std::string name;
    std::unordered_map<std::string, DataEntry> entries;
};

class DataParser
{
public:
    DataParser(Game* p_game);

    std::unordered_map<std::string, DataBlock> loadData(bool& pr_successful, std::string p_filePath, bool p_extensionIsInPath = false);
private:
    std::unordered_map<std::string, DataBlock> getBlocksFromFileData(bool& pr_successful, std::vector<std::string>& p_fileData);

    DataBlock extractBlockFromData(bool& pr_successful, std::string p_blockName, std::vector<std::string> p_blockLines);

    DataEntry extractEntryFromData(bool& pr_successful, std::vector<std::string> p_entryLines);

    Game* m_game;

    std::unordered_map<std::string, DataBlock> m_dataBlocks;
};