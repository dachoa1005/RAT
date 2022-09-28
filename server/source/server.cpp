#include "../../file_manager/file_manager.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>  // for types
#include <sys/socket.h> // for socket
#include <sys/sendfile.h> // for sendfile()
#include <sys/uio.h>    // for uio
#include <netinet/in.h> // for sockaddr_in
#include <errno.h>
#include <arpa/inet.h>
#include <fstream>

using namespace std;

// string SAVE_PLACE{"/home/dachoa1005/Desktop/RAT-project/server/"};


int main(int argc, const char **argv)
{
    // if (argc < 2)
    // {
    //     cerr << "Usage port" << endl;
    //     return 1;
    // }

    cout << "Waiting for client to connect" << endl;

    int portno = 8000; // set portnumber

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
    while (1)
    {
        cout << "--------------------------------" << endl;
        cout << "1. Send file to client" << endl;
        cout << "2. Get file from client" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();
        write(newSockDes, &choice, sizeof(choice));
        // cout << "byte_write: " << byte_write << endl;
        switch (choice)
        
        {
        case 1://send file to client 
        {
            filepath = "";
            // filename = "";
            cout << "Send file to client" << endl;
            memset(buffer, 0, sizeof(buffer));
            cout << "Enter file path: ";
            getline(cin, filepath);
            //get file name
            filename = filepath.substr(filepath.find_last_of("/\\")+1);
            // cout << filename << endl;
            //send file name
            send(newSockDes, filename.c_str(), sizeof(filename), 0);// send file name
            send_file(filename, filepath, newSockDes);
            cout << "Send file success" << endl;
            break;
        }
        
        case 2: //receive file from client 
        {
            filepath = "";
            cout << "Receive file from client" << endl;
            memset(buffer, 0, sizeof(buffer));
            cout << "Enter file path: " << endl;
            getline(cin, filepath);
            cout << "filepath: " << filepath << endl;
            int n = send(newSockDes, filepath.c_str(), 1024, 0); //send filepath to client
            // cout << "Byte sent: " << n << endl;
            filename = filepath.substr(filepath.find_last_of("/\\")+1);
            cout << "filename: " << filename << endl;
            string savepath = "/home/dachoa1005/Desktop/RAT/server/recv_files/" + filename;
            recv_file(filename, savepath, newSockDes);
            break;
        }
        case 3:
        {
            // exit
            cout << "Exit" << endl;
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