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

    // Create a socket for the client
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create client socket." << std::endl;
        WSACleanup();
        return 1;
    }

    // Connect to the server
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("192.168.0.8"); // Replace with the actual IP address of Computer A
    serverAddr.sin_port = htons(12345);

    if (connect(clientSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Failed to connect to the server." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cerr << "Connected";

    // Game logic here
    int guess;
    char serverMessage[100];
    bool gameOver = false;

    while (!gameOver)
    {

        // Receive the server's response
        recv(clientSocket, serverMessage, sizeof(serverMessage), 0);
        std::cout << serverMessage << std::endl;

        std::cout << "Enter your guess: ";
        std::cin >> guess;

        // Send the guess to the server
        send(clientSocket, reinterpret_cast<char *>(&guess), sizeof(guess), 0);
        // Check if the guess is correct
        if (strstr(serverMessage, "Congratulations") != nullptr)
        {
            recv(clientSocket, serverMessage, sizeof(serverMessage), 0);

            // gameOver = true;
        }
    }

    // Close the client socket and cleanup
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
