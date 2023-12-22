#pragma once

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

class Client {
public:
    Client();
    void connectToServer();
    void showMenu();
    void showSubMenu();
    void disconnect();
    void createExamRoom();
    void setNumberOfQuestions();
    void setExamDuration();
    void handleMenuChoice(int choice);
    ~Client();

private:
    int clientSocket;
    sockaddr_in serverAddress;
    bool isConnected;

    void login();
    void registerUser();
};
