#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <memory>
#include <string>
#include <queue>
#include "i_file_operations.hpp"

class Tokenizer
{
public:
    Tokenizer(std::string, std::shared_ptr<IFileOperations>);
    std::string GetNext();

private:
    void GetTokens();

    bool _started;
    std::string _constructedToken;
    std::string _filename;
    std::queue<std::string> _tokens;
    std::shared_ptr<std::istringstream> _contents;
    std::shared_ptr<IFileOperations> _fileOperations;
};

#endif
