#include "../../file_manager/file_manager.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <netinet/in.h> // for sockaddr_in
#include <errno.h>
#include <arpa/inet.h>
#include <fstream>
#include <netdb.h>

using namespace std;

int main(int argc, char const *argv[])
{
    // if (argc < 2)
    // {
    //     cout << "Err: Losing portnumber" << endl;
    //     exit(0);
    // }

    // int portno = 8001; // portnumber
    char const *serverIP = "127.0.0.1";
    int portno{0};
    cout << "Enter port number: ";
    cin >> portno;
    struct hostent *host = gethostbyname(serverIP);

    // get file save location
    system("pwd > pwd.txt");
    ifstream pwdFile("pwd.txt");
    string pwd;
    pwdFile >> pwd;
    string FILE_SAVE_PLACE = pwd + "/recv_files/";
    pwdFile.close();
    // cout << FILE_SAVE_PLACE << endl;

    sockaddr_in sendSockAddr;
    bzero((char *)&sendSockAddr, sizeof(sendSockAddr));

    sendSockAddr.sin_family = AF_INET;
    sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *)*host->h_addr_list));
    sendSockAddr.sin_port = htons(portno);

    // tạo socket mới dùng để giao tiếp với server
    int clientSockfd = socket(AF_INET, SOCK_STREAM, 0);

    // kiểm tra kết nối
    int status = connect(clientSockfd, (sockaddr *)&sendSockAddr, sizeof(sendSockAddr));
    if (status < 0)
    {
        cout << "Error connecting to socket!" << endl;
        exit(0);
    }
    cout << "Connected to the server!" << endl;

    // Sau khi connect thành công, bắt đầu giao tiếp
    int choice{0};
    char msg[1024];
    string filename{""};
    string filepath{""};
    string savepath{""};
    int flag = 0; // flag = 1: file exist || flag = 0: file not exist || flag = -1: cancel
    while (1)
    {
        cout << "----------------------------------------" << endl;
        cout << "Waiting for command" << endl;
        memset(msg, 0, sizeof(msg));
        read(clientSockfd, &choice, sizeof(choice));
        // cout << "Choice: " << choice+1 << endl;
        switch (choice)
        {
        case 1:
        {

            memset(msg, 0, sizeof(msg));
            cout << "Receive file from server" << endl;
            recv(clientSockfd, msg, sizeof(msg), 0); // receive file name
            if (strcmp(msg, "cancel") == 0 || strcmp(msg, "") == 0)
            {
                cout << "Cancel receive file" << endl;
                break;
            }
            cout << "File name: " << msg << endl;
            filename = string(msg);
            savepath = FILE_SAVE_PLACE + filename;
            // memset(&msg,0,sizeof(msg)); //clear buffer before recive file
            recv_file(filename, savepath, clientSockfd);
            cout << "File " << filename << " received." << endl;
            break;
        }
        case 2:
        {
            memset(msg, 0, sizeof(msg));
            cout << "Send file to server" << endl;
            flag = 0;
            while (flag == 0)
            {
                recv(clientSockfd, msg, sizeof(msg), 0); // receive file path
                // cout << "Byte received: " << n << endl;
                if (strcmp(msg, "cancel") == 0)
                {
                    flag = -1;
                    write(clientSockfd, &flag, sizeof(flag));
                    cout << "Cancel send file" << endl;
                    break;
                }
                filepath = string(msg);
                if (isFileExist(filepath) == false)
                {
                    // cout << "File not exist" << endl;
                    flag = 0;
                    write(clientSockfd, &flag, sizeof(flag));
                }
                else
                {
                    flag = 1;
                    write(clientSockfd, &flag, sizeof(flag));
                    break;
                }
            }

            if (flag == 1)
            {
                cout << "File path: " << msg << endl;
                filepath = string(msg);
                filename = filepath.substr(filepath.find_last_of("/\\") + 1);
                cout << "File name: " << filename << endl;
                send_file(filename, filepath, clientSockfd);
                cout << "File " << filename << " sent." << endl;
            }
            break;
        }

        case 3:
        {
            cout << "Terminal" << endl;
            fstream file;
            string command;
            while (true)
            {
                file.open("temp.txt");
                memset(msg, 0, sizeof(msg));
                recv(clientSockfd, msg, sizeof(msg), 0);
                if (strcmp(msg, "exit") == 0)
                {
                    cout << "Exit command" << endl;
                    break;
                }

                command = string(msg);
                cout << "Command: " << command << endl;
                command = command + " > temp.txt";
                system(command.c_str());
                string line;
                while (getline(file, line))
                {
                    cout << line << endl;
                }
                // file << "";
                file.close();

                send_file("temp.txt", "temp.txt", clientSockfd);
            }
            break;
        }
        case 0:
        {

            cout << "Server end the session" << endl;
            close(clientSockfd);
            exit(0);
        }
        default:
        {
            cout << "Server: " << choice << endl;
            break;
        }
        }
    }
    close(clientSockfd);

    return 0;
}
