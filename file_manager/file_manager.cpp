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

bool create_file(string file_name, string file_path)
{
    string path = file_path + "/" + file_name;
    ofstream file(path);
    if (!file.is_open())
    {
        // cout << "Error creating file" << endl;
        return false;
    }
    // cout << "File created" << endl;
    file.close();
    return true;
}

bool create_file(string file_path)
{
    string path = file_path;
    ofstream file(path);
    if (!file.is_open())
    {
        // cout << "Error creating file" << endl;
        return false;
    }
    // cout << "File created" << endl;
    file.close();
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
        // cout << buffer;
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
    memset(buffer, 0, 1024);
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
