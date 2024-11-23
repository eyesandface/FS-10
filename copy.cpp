#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>

void handleError(const std::string &message) 
{
    std::cerr << message << std::endl;
    exit(EXIT_FAILURE);
}

void copyFileWithHoles(const char* sourcePath, const char* destPath) 
{
    int srcFd = open(sourcePath, O_RDONLY);
    if (srcFd == -1) 
        handleError("Error opening source file: " + std::string(strerror(errno)));

    int destFd = open(destPath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (destFd == -1) 
       handleError("Error opening destination file: " + std::string(strerror(errno)));

    struct stat srcStats;
    if (fstat(srcFd, &srcStats) == -1) 
        handleError("Error getting file stats: " + std::string(strerror(errno)));

    off_t srcSize = srcStats.st_size;

    char buffer[4096];
    off_t totalCopied = 0;
    off_t dataCopied = 0; 
    off_t holesCopied = 0;

    off_t offset = 0;
    while (offset < srcSize) 
    {
        ssize_t bytesRead = pread(srcFd, buffer, sizeof(buffer), offset);
        if (bytesRead == -1)
            handleError("Error reading from source file: " + std::string(strerror(errno)));
        

        if (bytesRead == 0)
            break;

        ssize_t bytesWritten = pwrite(destFd, buffer, bytesRead, offset);
        if (bytesWritten == -1) {
            handleError("Error writing to destination file: " + std::string(strerror(errno)));
        }

        totalCopied += bytesWritten;
        dataCopied += (bytesRead > 0 ? bytesWritten : 0);
        holesCopied += (bytesRead == 0 ? bytesWritten : 0);

        offset += bytesRead;
    }

    std::cout << "Successfully copied " << totalCopied << " bytes (data: " << dataCopied
              << ", hole: " << holesCopied << ")." << std::endl;

    close(srcFd);
    close(destFd);
}


int main(int argc, char* argv[]) 
{
    if (argc != 3)
        handleError("Usage: ./copy <source-file> <destination-file>");
    

    const char* sourcePath = argv[1];
    const char* destPath = argv[2];

    copyFileWithHoles(sourcePath, destPath);

    return 0;
}
