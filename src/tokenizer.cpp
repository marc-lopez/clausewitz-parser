#include <sstream>
#include <string>
#include "tokenizer.hpp"

Tokenizer::Tokenizer(std::string filename, std::shared_ptr<IFileOperations> fileOperations) :
    _started(false), _constructedToken(), _filename(filename), _contents(), _fileOperations(fileOperations)
{

}

std::string Tokenizer::GetNext()
{
    if (!_started)
    {
        GetTokens();
        _started = true;
    }

    auto tokenToRetrieve = _tokens.empty() ? "" : _tokens.front();
    if (!_tokens.empty())
    {
        _tokens.pop();
    }

    return tokenToRetrieve;
}

void Tokenizer::GetTokens()
{
    _contents = _fileOperations->Read(_filename);

    std::string line;
    *_contents >> line;

    for (auto ch = line.begin(); ch != line.end(); ++ch)
    {
        if (*ch == '=' || _constructedToken == "=")
        {
            _tokens.push(_constructedToken);
            _constructedToken.clear();
        }

        _constructedToken += *ch;
    }

    _tokens.push(_constructedToken);
}
