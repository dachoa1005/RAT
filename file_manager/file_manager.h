#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>

    bool create_file(std::string file_name,std::string file_path);
    bool delete_file(std::string file_name,std::string file_path);
    bool copy_file(std::string file_name, std::string file_path, std::string dest_path);
    bool move_file(std::string file_name,std::string file_path, std::string dest_path);

    bool create_folder(std::string folder_name, std::string folder_path);
    bool delete_folder(std::string folder_name,std::string folder_path);
    bool copy_folder(std::string folder_name, std::string folder_path, std::string dest_path);
    bool move_folder(std::string folder_name, std::string folder_path, std::string dest_path);
    bool list_folder(std::string folder_path);
    bool rename_folder(std::string folder_path, std::string old_name, std::string new_name);

    bool send_file(std::string filename, std::string file_path, int socket);
    bool recv_file(std::string filename, std::string file_path, int socket);
    bool kill_process(std::string process_name);
    

#endif