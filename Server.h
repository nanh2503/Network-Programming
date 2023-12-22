// Server.h
#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sqlite3.h>

class Server {
public:
    Server();
    void start();
     void handleCreateExamRoom(int clientSocket);
    void handleSetNumberOfQuestions(int clientSocket);
    void handleSetExamDuration(int clientSocket);
    void handleLogin(int clientSocket);
    void handleRegistration(int clientSocket);
    void initializeDatabase();
    void closeConnected()
    {
        isConnected = false;
    }
    ~Server();

private:
    bool isConnected;
    int serverSocket;
    sockaddr_in serverAddress;
    sqlite3* db;

    // User data structure
    struct UserData {
        std::string username;
        std::string password;
    };

    // Vector to store user data
    std::vector<UserData> users;

    // Helper functions
    bool loginUser(const std::string& username, const std::string& password);
    bool registerUser(const std::string& username, const std::string& password);
};
