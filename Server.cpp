// Server.cpp
#include "Server.h"

Server::Server() : isConnected(true){
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1) {
        std::cerr << "Error creating socket\n";
        exit(EXIT_FAILURE);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8888);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error binding socket\n";
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 10) == -1) {
        std::cerr << "Error listening on socket\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port 8888...\n";
}

Server::~Server() {
    close(serverSocket);
}

void Server::start() {
    initializeDatabase();
    while (isConnected) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (!isConnected) {
            // Nếu kết nối đã đóng, thoát khỏi vòng lặp
            break;
        }
        // Send confirmation to the client
        const char* confirmationMessage = "Connection established. Welcome to the server!";
        send(clientSocket, confirmationMessage, strlen(confirmationMessage), 0);

        std::cout << "Sent confirmation to client\n";

        // Handle other client requests here...

        // Receive client request type
        int requestType;
        recv(clientSocket, &requestType, sizeof(requestType), 0);

        // Handle the request based on the type
        switch (requestType) {
            case 1:
                handleLogin(clientSocket);
                break;
            case 2:
                handleRegistration(clientSocket);
                break;
            case 4:
                handleCreateExamRoom(clientSocket);
                break;
            case 5:
                handleSetNumberOfQuestions(clientSocket);
                break;
            case 6:
                handleSetExamDuration(clientSocket);
                break; 
            default:
                std::cerr << "Invalid request type\n";
                break;
        }

        close(clientSocket);
    }
}
void Server::initializeDatabase() {
    std::string dbPath = "users.db"; 

    int rc = sqlite3_open(dbPath.c_str(), &db);

    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Create a users table if it doesn't exist
    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, username TEXT, password TEXT);";
    rc = sqlite3_exec(db, createTableSQL, NULL, 0, NULL);

    if (rc != SQLITE_OK) {
        std::cerr << "Can't create table: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
}



void Server::handleLogin(int clientSocket) {
    std::cout << __LINE__ << std::endl;

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // Receive username and password from the client
    
    // recv(clientSocket, buffer, sizeof(buffer), 0);
    // Nhận độ dài của dữ liệu
    std::cout << __LINE__ << std::endl;

    size_t dataLength;
    recv(clientSocket, &dataLength, sizeof(dataLength), 0);
    std::cout << __LINE__ << std::endl;


    // Nhận dữ liệu
    // char buffer[1024];
    recv(clientSocket, buffer, dataLength, 0);

    // Thêm ký tự null để tạo thành chuỗi C
    buffer[dataLength] = '\0';

    // std::string receivedData(buffer);


    // Extract username and password from the received string
    std::string credentials(buffer);
    size_t pos = credentials.find(':');
    std::string username = credentials.substr(0, pos);
    std::string password = credentials.substr(pos + 1);

    // Check if the user exists in the database
    std::string query = "SELECT * FROM users WHERE username='" + username + "' AND password='" + password + "';";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        std::cerr << "Can't execute query: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        const char* successMessage = "Ok";
        send(clientSocket, successMessage, strlen(successMessage), 0);
    } else {
        const char* errorMessage = "NotOk";
        send(clientSocket, errorMessage, strlen(errorMessage), 0);
    }

    sqlite3_finalize(stmt);
}

void Server::handleRegistration(int clientSocket) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // Receive new username and password from the client
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "|tin nhan tu regis server: " << buffer << std::endl;

    // Extract new username and password from the received string
    std::string credentials(buffer);
    size_t pos = credentials.find(':');
    std::string newUsername = credentials.substr(0, pos);
    std::string newPassword = credentials.substr(pos + 1);

    // Insert the new user into the database
    std::string query = "INSERT INTO users (username, password) VALUES ('" + newUsername + "', '" + newPassword + "');";
    int rc = sqlite3_exec(db, query.c_str(), NULL, 0, NULL);

    if (rc == SQLITE_OK) {
        const char* successMessage = "Registration successful";
        send(clientSocket, successMessage, strlen(successMessage), 0);
    } else {
        const char* errorMessage = "Registration failed";
        send(clientSocket, errorMessage, strlen(errorMessage), 0);
    }
}

void Server::handleCreateExamRoom(int clientSocket) {
    std::cout << "Handling create exam room request\n";

    const char* responseMessage = "Exam room created successfully";
    send(clientSocket, responseMessage, strlen(responseMessage), 0);
}

void Server::handleSetNumberOfQuestions(int clientSocket) {
    std::cout << "Handling set number of questions request\n";

    const char* responseMessage = "Number of questions set successfully";
    send(clientSocket, responseMessage, strlen(responseMessage), 0);
}

void Server::handleSetExamDuration(int clientSocket) {
    std::cout << "Handling set exam duration request\n";

    const char* responseMessage = "Exam duration set successfully";
    send(clientSocket, responseMessage, strlen(responseMessage), 0);
}



