#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>

    bool isFileExist(std::string filepath);
    bool create_file(std::string file_name,std::string file_path);
    bool create_file(std::string file_path);
   
    bool send_file(std::string filename, std::string file_path, int socket);
    bool recv_file(std::string filename, std::string file_path, int socket);
    

#endif