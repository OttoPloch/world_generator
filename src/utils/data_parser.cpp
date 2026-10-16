#include "data_parser.hpp"
#include "../core/game.hpp"
#include "utils.hpp"
#include <stdexcept>

DataWord::DataWord(std::string p_type, std::string p_contents) : type(p_type), contents(p_contents) {}

DataParser::DataParser(Game* p_game) : m_game(p_game) {}

void DataParser::loadData(bool& pr_successful, std::unordered_map<std::string, DataObject>& pr_objects, std::string p_filePath, bool p_extensionIsInPath)
{
    std::string l_filePath = p_filePath;
    if (!p_extensionIsInPath) l_filePath += ".data";

    std::string l_fileData = m_game->getAssetManager()->getTextFromFile(l_filePath);

    getObjectsFromFileData(pr_successful, pr_objects, l_fileData);
    if (!pr_successful)
    {
        std::cerr << "ERROR: Failed to parse file for data. File path is '" << l_filePath << "'\n";
        return;
    }
}

void DataParser::getObjectsFromFileData(bool& pr_successful, std::unordered_map<std::string, DataObject>& pr_objects, std::string& p_fileData)
{
    std::vector<DataWord> l_words;

    getWordsFromString(l_words, p_fileData);

    for (auto& i_word : l_words)
    {
        std::cout << "WORD TYPE: " << i_word.type << "\nWORD CONTENTS: \"" << i_word.contents << "\"\n- - - - - - - - - - - -\n";
    }

    // std::unordered_map<std::string, DataBlock> lr_blocks;

    // int l_indentationSpaces = 0;

    // std::string l_currBlock = "none";
    // std::vector<std::string> l_currBlockLines;

    // for (auto i_line : p_fileData)
    // {
    //     i_line = i_line.substr(l_indentationSpaces);

    //     if (l_currBlock == "none")
    //     {
    //         auto l_openBracket = i_line.find(" {");
    //         if (l_openBracket == std::string::npos)
    //         {
    //             std::cerr << "ERROR: Cannot parse file for data. A line outside of a block must start a block.\n";
    //             pr_successful = false;
    //             return {};
    //         }

    //         l_currBlock = i_line.substr(0, l_openBracket);
    //         continue;
    //     }

    //     if (i_line == "}")
    //     {
    //         lr_blocks[l_currBlock] = extractBlockFromData(pr_successful, l_currBlock, l_currBlockLines);
    //         if (!pr_successful) return {};

    //         l_currBlock = "none";
    //         l_currBlockLines.clear();
    //         continue;
    //     }

    //     l_currBlockLines.emplace_back(i_line);
    // }

    // return lr_blocks;
}

void DataParser::getWordsFromString(std::vector<DataWord>& pr_words, std::string& p_string)
{
    std::string l_previousWordType;
    std::string l_expectedWordType = "type name";
    std::string l_currWordContents;
    for (auto i_char : p_string)
    {
        if (charEndsWord(l_expectedWordType, i_char, l_currWordContents))
        {
            pr_words.emplace_back(l_expectedWordType, l_currWordContents);

            l_currWordContents.clear();

            l_previousWordType = l_expectedWordType;
            setExpectedWordType(l_expectedWordType, l_previousWordType);
        }
        else
        {
            l_currWordContents += i_char;
        }
    }
}

bool DataParser::charEndsWord(std::string p_expectedWordType, char p_char, const std::string& p_wordContents)
{
    if (p_expectedWordType == "type name")
    {
        return p_char == ' ';
    }
    else if (p_expectedWordType == "identifier")
    {
        return !std::isalnum(p_char);
    }
    else if (p_expectedWordType == "braces")
    {
        if (p_char != '}') return false;

        unsigned int l_openBraces = countOccurencesInString(p_wordContents, "{");
        unsigned int l_closedBraces = countOccurencesInString(p_wordContents, "}");
        return l_closedBraces == l_openBraces - 1;
    }
    else if (p_expectedWordType == "variable name")
    {
        return p_char == ' ';
    }
    else if (p_expectedWordType == "equals")
    {
        return p_char == '=';
    }
    else if (p_expectedWordType == "value")
    {
        if (p_char != ';') return false;
        if (p_wordContents[0] != '\'') return true;

        return p_wordContents[p_wordContents.size() - 1] == '\'';
    }
    else if (p_expectedWordType == "brackets")
    {
        if (p_char != ']') return false;

        unsigned int l_openBrackets = countOccurencesInString(p_wordContents, "[");
        unsigned int l_closedBrackets = countOccurencesInString(p_wordContents, "]");
        return l_closedBrackets == l_openBrackets - 1;
    }
    else if (p_expectedWordType == "parentheses")
    {
        if (p_char != ')') return false;

        unsigned int l_openParentheses = countOccurencesInString(p_wordContents, "(");
        unsigned int l_closedParentheses = countOccurencesInString(p_wordContents, ")");
        return l_closedParentheses == l_openParentheses - 1;
    }

    return false;
}

void DataParser::setExpectedWordType(std::string& pr_expectedWordType, std::string p_previousWordType)
{
    if (p_previousWordType == "type name")
    {
        pr_expectedWordType = "identifier";
    }
    else if (p_previousWordType == "identifier")
    {
        pr_expectedWordType = "braces";
    }
    else if (p_previousWordType == "braces")
    {
        pr_expectedWordType = "type name";
    }
    else if (p_previousWordType == "variable name")
    {
        pr_expectedWordType = "equals";
    }
    else if (p_previousWordType == "equals")
    {
        pr_expectedWordType = "value";
    }
    else if (p_previousWordType == "value")
    {
        pr_expectedWordType = "variable name";
    }
}

// DataBlock DataParser::extractBlockFromData(bool& pr_successful, std::string p_blockName, std::vector<std::string> p_blockLines)
// {
//     DataBlock lr_block = {p_blockName};

//     int l_indentationSpaces = 4;

//     std::string l_currEntry = "none";
//     std::vector<std::string> l_currEntryLines;

//     for (auto i_line : p_blockLines)
//     {
//         i_line = i_line.substr(l_indentationSpaces);

//         if (l_currEntry == "none")
//         {
//             auto l_openBracket = i_line.find(" {");
//             if (l_openBracket == std::string::npos)
//             {
//                 std::cerr << "ERROR: Cannot parse file for data. A line inside a block and outside an entry must start an entry.\n";
//                 pr_successful = false;
//                 return {};
//             }

//             l_currEntry = i_line.substr(0, l_openBracket);
//             continue;
//         }

//         if (i_line == "}")
//         {
//             lr_block.entries[l_currEntry] = extractEntryFromData(pr_successful, l_currEntryLines);
//             if (!pr_successful) return {};

//             l_currEntry = "none";
//             l_currEntryLines.clear();
//             continue;
//         }

//         l_currEntryLines.emplace_back(i_line);
//     }

//     return lr_block;
// }

// DataEntry DataParser::extractEntryFromData(bool& pr_successful, std::vector<std::string> p_entryLines)
// {
//     DataEntry lr_entry;

//     int l_indentationSpaces = 4;

//     std::string l_currVariable = "none";
//     std::string l_currValue = "";

//     for (auto i_line : p_entryLines)
//     {
//         i_line = i_line.substr(l_indentationSpaces);

//         std::string l_operatorString = " = ";
//         auto l_operator = i_line.find(l_operatorString);
//         if (l_operator == std::string::npos)
//         {
//             std::cerr << "ERROR: Cannot parse file for data. A line inside an entry must set a variable with \" = \".\n";
//             pr_successful = false;
//             return {};
//         }

//         l_currVariable = i_line.substr(0, l_operator);
//         l_currValue = i_line.substr(l_operator + l_operatorString.size());

//         lr_entry.variables[l_currVariable] = l_currValue;
//     }

//     return lr_entry;
// }