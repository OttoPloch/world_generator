#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class Game;

// struct DataEntry
// {
//     std::unordered_map<std::string, std::string> variables;
// };

// struct DataBlock
// {
//     std::string name;
//     std::unordered_map<std::string, DataEntry> entries;
// };

struct DataEntry
{
    std::unordered_map<std::string, std::string> variables;
};

struct DataObject
{
    std::string name;
    std::unordered_map<std::string, DataEntry> entries;
};

struct DataWord
{
    DataWord(std::string p_type, std::string p_contents);

    std::string type;
    std::string contents;
};

class DataParser
{
public:
    DataParser(Game* p_game);

    void loadData(bool& pr_successful, std::unordered_map<std::string, DataObject>& pr_objects, std::string p_filePath, bool p_extensionIsInPath = false);
private:
    void getObjectsFromFileData(bool& pr_successful, std::unordered_map<std::string, DataObject>& pr_objects, std::string& p_fileData);

    void getWordsFromString(std::vector<DataWord>& pr_words, std::string& p_string);

    bool charEndsWord(std::string p_expectedWordType, char p_char, const std::string& p_wordContents);

    void setExpectedWordType(std::string& pr_expectedWordType, std::string p_previousWordType);

    // DataBlock extractBlockFromData(bool& pr_successful, std::string p_blockName, std::vector<std::string> p_blockLines);

    // DataEntry extractEntryFromData(bool& pr_successful, std::vector<std::string> p_entryLines);

    

    Game* m_game;

    // std::unordered_map<std::string, DataBlock> m_dataBlocks;
};