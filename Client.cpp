#include "Client.h"
void Client::showSubMenu() {
    int choice;

    while (true) {
        std::cout << "\n===== Menu =====\n";
        std::cout << "1. Log in\n";
        std::cout << "2. Register\n";
        std::cout << "3. Create new exam room\n";
        std::cout << "4. Set number of questions\n";
        std::cout << "5. Set exam duration\n";
        std::cout << "6. Quit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        handleMenuChoice(choice);
    }
}
void Client::handleMenuChoice(int choice) {
    switch (choice) {
        case 1:
            login();
            break;
        case 2:
            registerUser();
            break;
        case 3:
            createExamRoom();
            break;
        case 4:
            setNumberOfQuestions();
            break;
        case 5:
            setExamDuration();
            break;
        case 6:
            disconnect();
            exit(EXIT_SUCCESS);
        default:
            std::cerr << "Invalid choice. Please try again.\n";
            break;
    }
}
void Client::createExamRoom() {
    if (isConnected) {
        int  requestType = 4 ;
        send(clientSocket, &requestType, sizeof(requestType), 0);

        // handle processs
    } else {
        std::cerr << "Not connected to the server. Please connect first.\n";
    }
}

void Client::setNumberOfQuestions() {
    if (isConnected) {
        // Gửi yêu cầu thiết lập số câu hỏi
        int  requestType = 5;
        send(clientSocket, &requestType, sizeof(requestType), 0);

    } else {
        std::cerr << "Not connected to the server. Please connect first.\n";
    }
}

void Client::setExamDuration() {
    if (isConnected) {
        // Gửi yêu cầu thiết lập thời gian thi
        int  requestType = 6;
        send(clientSocket, &requestType, sizeof(requestType), 0);
    } else {
        std::cerr << "Not connected to the server. Please connect first.\n";
    }
}
Client::Client() : isConnected(false) {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == -1) {
        std::cerr << "Error creating socket\n";
        exit(EXIT_FAILURE);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8888);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);
}

void Client::connectToServer() {
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error connecting to server\n";
        exit(EXIT_FAILURE);
    }

    isConnected = true;

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // Receive confirmation from the server
    recv(clientSocket, buffer, sizeof(buffer), 0);

    std::cout << "Server response: " << buffer << std::endl;

    // Show the main menu after successful connection
    showMenu();
}

void Client::disconnect() {
    if (isConnected) {
        close(clientSocket);
        isConnected = false;
        std::cout << "Disconnected from server\n";
    }
    else
    {
        std::cout << "Thoat chuong trinh";
    }
}

void Client::showMenu() {
    int choice;

    while (true) {
        std::cout << "\n===== Menu =====\n";
        std::cout << "1. Log in\n";
        std::cout << "2. Register1\n";
        std::cout << "3. Quit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        send(clientSocket, &choice, sizeof(choice), 0);

        switch (choice) {
            case 1:
                login();
                break;
            case 2:
                registerUser();
                break;
            case 3:
                disconnect();
                break;
                // exit(EXIT_SUCCESS);
            default:
                std::cerr << "Invalid choice. Please try again.\n";
                break;
        }
    }
}

void Client::login() {
    std::cout << __LINE__ << std::endl;

    if (!isConnected) {
        // Tạo socket mới
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);

        if (clientSocket == -1) {
            std::cerr << "Error creating socket\n";
            exit(EXIT_FAILURE);
        }

        // Kết nối đến server
        if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
            std::cerr << "Error connecting to server\n";

            // Đóng socket mới nếu kết nối không thành công
            close(clientSocket);
            return;
        }

        isConnected = true;

        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));

        // Nhận phản hồi từ server
        recv(clientSocket, buffer, sizeof(buffer), 0);
        if (buffer == "Ok")
        {
            std::cout << "Server response: " << buffer << std::endl;
            showSubMenu();
        }

    }
    
    std::cout << "Enter username: ";
    std::string username;
    std::cin >> username;

    std::cout << "Enter password: ";
    std::string password;
    std::cin >> password;

    // Send request type
    // int requestType = 1;
    std::cout << __LINE__ << std::endl;

    // send(clientSocket, &requestType, sizeof(requestType), 0);
    std::cout << __LINE__ << std::endl;
    // Send username and password to the server
    // send(clientSocket, (username + ":" + password).c_str(), strlen((username + ":" + password).c_str()), 0);

    // Tính độ dài của chuỗi dữ liệu
    size_t dataLength = (username + ":" + password).length();
    std::cout << __LINE__ << (isConnected ? "true" :"false") << std::endl;

    // Gửi độ dài của dữ liệu
    send(clientSocket, &dataLength, sizeof(dataLength), 0);
    std::cout << __LINE__ << std::endl;


    // Gửi dữ liệu thực sự
    send(clientSocket, (username + ":" + password).c_str(), dataLength, 0);

    std::cout << __LINE__ << std::endl;

    // Receive server response
    char buffer[1024];
    std::cout << __LINE__ << std::endl;
    memset(buffer, 0, sizeof(buffer));
    std::cout << __LINE__<< std::endl;

    recv(clientSocket, buffer, sizeof(buffer), 0);

    std::cout << "Server response login: " << buffer << std::endl;
}

void Client::registerUser() {
    std::cout << "Enter new username: ";
    std::string newUsername;
    std::cin >> newUsername;

    std::cout << "Enter new password: ";
    std::string newPassword;
    std::cin >> newPassword;

    // Send request type
    // int requestType = 2 ;
    // send(clientSocket, &requestType, sizeof(requestType), 0);

    // Send new username and password to the server
    send(clientSocket, (newUsername + ":" + newPassword).c_str(), strlen((newUsername + ":" + newPassword).c_str()), 0);

    // Receive server response
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);

    std::cout << "Server response: " << buffer << std::endl;
}


Client::~Client() {
    disconnect();
}
