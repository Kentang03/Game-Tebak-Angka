#include <iostream>
#include <winsock2.h>

int main()
{
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }

    // Create a socket for the server
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create server socket." << std::endl;
        WSACleanup();
        return 1;
    }

    // Bind the socket to an address and port
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(12345);

    if (bind(serverSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Failed to bind the server socket." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 1) == SOCKET_ERROR)
    {
        std::cerr << "Failed to listen on the server socket." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Waiting for a client to connect..." << std::endl;

    // Accept a client connection
    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to accept the client connection." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Generate a random number for the game
    // int targetNumber = rand() % 100 + 1;

    // Game logic here
    // Generate a random number for the game

    int targetNumber;

    std::cout << "Enter your guess: ";
    std::cin >> targetNumber;

    // Send the target number to the client
    send(clientSocket, reinterpret_cast<char *>(&targetNumber), sizeof(targetNumber), 0);

    int guess;
    bool gameOver = false;

    while (!gameOver)
    {
        // Receive the client's guess
        recv(clientSocket, reinterpret_cast<char *>(&guess), sizeof(guess), 0);

        // Check if the guess is correct
        if (guess == targetNumber)
        {
            send(clientSocket, "Congratulations! You guessed the correct number.", 49, 0);
            gameOver = true;
            if (gameOver)
            {
                std::cout << "Enter your guess: ";
                std::cin >> targetNumber;
                // Send the target number to the client
                send(clientSocket, reinterpret_cast<char *>(&targetNumber), sizeof(targetNumber), 0);
                gameOver = false;
            }
        }
        else if (guess < targetNumber)
        {
            send(clientSocket, "Too low! Try again.", 20, 0);
        }
        else
        {
            send(clientSocket, "Too high! Try again.", 21, 0);
        }
    }

    // Close sockets and cleanup
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
