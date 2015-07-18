#ifndef I_FILE_OPERATIONS_MOCK_HPP
#define I_FILE_OPERATIONS_MOCK_HPP

#include "gmock/gmock.h"
#include "i_file_operations.hpp"

class IFileOperationsMock : public IFileOperations
{
public:
    MOCK_METHOD1(Read, std::shared_ptr<std::istringstream>(std::string));
};

#endif
