#ifndef I_FILE_OPERATIONS_HPP
#define I_FILE_OPERATIONS_HPP

#include <fstream>
#include <memory>

class IFileOperations
{
public:
    IFileOperations() {}
    IFileOperations(IFileOperations& other) {}
	virtual std::shared_ptr<std::istringstream> Read(std::string filename) = 0;
};

#endif
