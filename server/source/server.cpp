#include "../../file_manager/file_manager.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>    // for types
#include <sys/socket.h>   // for socket
#include <sys/sendfile.h> // for sendfile()
#include <sys/uio.h>      // for uio
#include <netinet/in.h>   // for sockaddr_in
#include <errno.h>
#include <arpa/inet.h>
#include <fstream>

using namespace std;

int main(int argc, const char **argv)
{
    // if (argc < 2)
    // {
    //     cerr << "Usage port" << endl;
    //     return 1;
    // }
    // int portno = 8001; // set portnumber
    int portno{0};
    cout << "Enter port number: ";
    cin >> portno;
    char const *serverIP = "127.0.0.1";

    // get file save location
    system("pwd > pwd.txt");
    ifstream pwdFile("pwd.txt");
    string pwd;
    pwdFile >> pwd;
    string FILE_SAVE_PLACE = pwd + "/recv_files/";
    pwdFile.close();
    // cout << FILE_SAVE_PLACE << endl;

    cout << "Waiting for client to connect" << endl;

    struct sockaddr_in servAddr;

    memset((char *)&servAddr, 0, sizeof(servAddr)); // set tat ca gia tri trong servAddr thanh 0;

    // setup servAddr-----------------
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(portno);
    //-------------------------------

    // create socket------------
    int servSockfd = socket(AF_INET, SOCK_STREAM, 0); // servSockfd = server Socket file descriptor
    if (servSockfd < 0)
    {
        // thiết lập socket lỗi
        cerr << "Socket establishing error" << endl;
        exit(0);
    }
    //-------------------------

    // bind-----------------
    int bindStat = bind(servSockfd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    // hàm bind cho servSockfd biết local address của servAddr
    if (bindStat < 0)
    {
        // binding lỗi
        cerr << "Error binding socket to local address" << endl;
        exit(0);
    }
    // ---------------------

    // listen for 5 request at a time, receive a request from client
    listen(servSockfd, 5);
    // need a new address to connect with client
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    // accept and create new socket descriptor to handle the new connection with client
    int newSockDes = accept(servSockfd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if (newSockDes < 0)
    {
        cerr << "Error accepting request from client!" << endl;
        exit(0);
    }

    cout << "Connected to Client" << endl;

    int choice{0}; // selection
    char buffer[1024] = {};
    string filename{""};
    string filepath{""};
    string savepath{""};
    char tempBuffer[1024] = {};
    int flag = 0; // to decide whether to send file or not
                  // flag = 1: send file || flag = -1: cancel sending file || flag = 0: file not found

    // check if file exist or not

    while (1)
    {
        cout << "--------------------------------" << endl;
        cout << "1. Send file to client" << endl;
        cout << "2. Get file from client" << endl;
        cout << "3. Terminal" << endl;
        cout << "0. Exit" << endl;

        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();
        write(newSockDes, &choice, sizeof(choice));
        // cout << "byte_write: " << byte_write << endl;
        switch (choice)

        {
        case 1: // send file to client
        {
            filepath = "";
            // filename = "";
            cout << "Send file to client" << endl;
            memset(buffer, 0, sizeof(buffer));
            do
            {
                cout << "Enter file path: ";
                getline(cin, filepath);
                if (filepath == "cancel")
                {
                    flag = -1;
                    cout << "Cancel sending file" << endl;
                    strcpy(tempBuffer, "cancel");
                    send(newSockDes, tempBuffer, sizeof(tempBuffer), 0);
                    break;
                }
                if (isFileExist(filepath) == false)
                {
                    flag = 0;
                    cout << "File not found! Enter cancel to break" << endl;
                }
                else
                {
                    flag = 1;
                }
            } while (flag == 0);

            // get file name

            if (flag == 1)
            {
                filename = filepath.substr(filepath.find_last_of("/\\") + 1);
                // cout << filename << endl;
                // send file name
                memset(tempBuffer, 0, sizeof(tempBuffer));
                strcpy(tempBuffer, filename.c_str());
                send(newSockDes, tempBuffer, sizeof(tempBuffer), 0); // send file name
                send_file(filename, filepath, newSockDes);
                cout << "File " << filename << " sent." << endl;
            }
            break;
        }

        case 2: // receive file from client
        {
            filepath = "";
            cout << "Receive file from client" << endl;
            memset(buffer, 0, sizeof(buffer));
            flag = 0;
            while (flag == 0)
            {
                cout << "Enter file path: ";
                getline(cin, filepath);

                // cancel receiving file
                if (filepath == "cancel")
                {
                    flag = -1;
                    cout << "Cancel receiving file" << endl;
                    strcpy(tempBuffer, "cancel");
                    send(newSockDes, tempBuffer, sizeof(tempBuffer), 0);
                    break;
                }

                memset(tempBuffer, 0, sizeof(tempBuffer));
                strcpy(tempBuffer, filepath.c_str());
                send(newSockDes, tempBuffer, sizeof(tempBuffer), 0); // send filepath to client
                read(newSockDes, &flag, sizeof(flag));
                if (flag == 0)
                {
                    cout << "File not found! Enter cancel to break" << endl;
                } else if (flag == 1)
                {
                    cout << "File found!" << endl;
                    break;
                }
            }

            // cout << "Byte sent: " << n << endl;
            if (flag == 1)
            {
                filename = filepath.substr(filepath.find_last_of("/\\") + 1);
                savepath = FILE_SAVE_PLACE + filename;
                recv_file(filename, savepath, newSockDes);
                cout << "File " << filename << " received." << endl;
            }
            break;
        }

        case 3: // terminal
        {
            string command;
            while (true)
            {
                cout << ">";
                getline(cin, command);
                if (command == "exit")
                {
                    strcpy(tempBuffer, command.c_str());
                    send(newSockDes, tempBuffer, sizeof(tempBuffer), 0);
                    break;
                }
                strcpy(tempBuffer, command.c_str());
                send(newSockDes, tempBuffer, sizeof(tempBuffer), 0);
                memset(tempBuffer, 0, sizeof(tempBuffer));

                recv_file("temp.txt", "temp.txt", newSockDes);
                ifstream fin("temp.txt");
                string line;
                while (getline(fin, line))
                {
                    cout << line << endl;
                }
                fin.close();
            }
            break;
        }

        case 0:
        {
            // exit
            cout << "Server end the session" << endl;
            close(newSockDes);
            close(servSockfd);
            exit(0);
            break;
        }

        default:
        {
            cout << "Invalid choice" << endl;
            break;
        }
        }
    }
    close(newSockDes);
    close(servSockfd);
    return 0;
}