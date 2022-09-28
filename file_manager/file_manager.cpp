#include "file_manager.h"
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <unistd.h>
#include <dirent.h>
#include <cstring>
#include <sys/stat.h>
#include <filesystem>

using namespace std;

bool is_exist(string file_name, string file_path) // check if file_name exits in file_path
{

    return true;
}

bool create_file(string file_name, string file_path)
{
    string path = file_path + "/" + file_name;
    ofstream file(path);
    if (!file.is_open())
    {
        cout << "Error creating file" << endl;
        return false;
    }
    cout << "File created" << endl;
    file.close();
    return true;
}

bool create_file(string file_path)
{
    string path = file_path;
    ofstream file(path);
    if (!file.is_open())
    {
        cout << "Error creating file" << endl;
        return false;
    }
    cout << "File created" << endl;
    file.close();
    return true;
}

bool delete_file(string file_name, string file_path)
{
    string path = file_path + "/" + file_name;
    if (remove(path.c_str()) != 0)
    {
        cout << "Error deleting file" << endl;
        return false;
    }
    cout << "File deleted" << endl;
    return true;
}

bool copy_file(string file_name, string file_path, string dest_path)
{
    string src = file_path + "/" + file_name;
    string dest = dest_path + "/" + file_name;
    ifstream src_file(src);
    if (!src_file.is_open())
    {
        cout << "Source file not found" << endl;
        return false;
    }
    if (!create_file(file_name, dest_path))
    {
        cout << "Error creating destination file" << endl;
        return false;
    };
    ofstream dest_file(dest);
    string data;
    while (!src_file.eof())
    {
        getline(src_file, data);
        dest_file << data << endl;
    }
    cout << "Copy file done" << endl;
    src_file.close();
    dest_file.close();
    return true;
}

bool move_file(string file_name, string file_path, string dest_path)
{
    string src_path = file_path + "/" + file_name;
    if (!copy_file(file_name, file_path, dest_path))
    {
        cout << "Error copying file" << endl;
        return false;
    };
    if (!delete_file(file_name, file_path))
    {
        cout << "Error deleting old file" << endl;
        return false;
    };
    cout << "File moved" << endl;
    return true;
}

bool create_folder(string folder_name, string folder_path)
{
    string path = folder_path + "/" + folder_name;
    mkdir(path.c_str(), 0777);
    if (errno == EEXIST)
    {
        cout << "Folder already exists" << endl;
        return false;
    }
    else
    {
        cout << "Folder created" << endl;
        return true;
    }
}

bool delete_folder(string folder_name, string folder_path)
{
    string path = folder_path + "/" + folder_name;
    if (!rmdir(path.c_str()))
    {
        cout << "Error deleting folder" << endl;
        return false;
    }
    cout << "Folder deleted " << endl;
    return true;
}

bool copy_folder(string folder_name, string folder_path, string dest_path)
{
    return true;
}

bool move_folder(string folder_name, string folder_path, string dest_path)
{
    return true;
}

bool list_folder(string folder_path)
{
    return true;
}

bool rename_folder(string folder_path, string dest_path)
{
    return true;
}

bool send_file(string file_name, string file_path, int socket)
{
    // ifstream source(file_path);
    FILE *file = fopen(file_path.c_str(), "r");
    char buffer[1024];
    memset(buffer, 0, 1024);
    while (fgets(buffer, 1024, file) != nullptr)
    {
        // cout << "Sending data..." << endl;
        if (send(socket, buffer, sizeof(buffer), 0) <= 0)
        {
            cout << "Error sending file." << endl;
            return false;
        }
        memset(buffer, 0, 1024);
    }
    strcpy(buffer, "\e");
    send(socket, buffer, sizeof(buffer), 0);
    // cout << "Sent file." << endl;
    memset(buffer, 0, sizeof(buffer));
    fclose(file);
    return true;
}

bool recv_file(string file_name, string file_path, int socket)
{
    ofstream file(file_path);
    char buffer[1024];
    while (true)
    {
        if (recv(socket, buffer, 1024, 0) <= 0)
        {
            break;
        }
        if (strcmp(buffer, "\e") == 0)
        {
            break;
        }
        file << buffer;
        // cout << buffer;
        memset(buffer, 0, 1024);
    }
    file.close();
    return true;
}

bool kill_process(string process_name)
{
    string command = "killall " + process_name;
    system(command.c_str());
    cout << "Process killed" << endl;
    return true;
}
