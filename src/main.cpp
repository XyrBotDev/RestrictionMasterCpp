#include <arpa/inet.h>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#include "../include/config_loader.hpp"
#include "../include/access_control.hpp"
#include "../include/bot_manager.hpp"
#include "../include/bot_service.hpp"

namespace {

int getPort() {
    const char* value = std::getenv("PORT");

    if (value == nullptr || *value == '\0') {
        return 10000;
    }

    try {
        const int port = std::stoi(value);

        if (port > 0 && port <= 65535) {
            return port;
        }
    } catch (...) {
    }

    return 10000;
}

class HealthServer {
public:
    explicit HealthServer(int port)
        : port_(port) {}

    ~HealthServer() {
        stop();
    }

    bool start() {
        if (running_) {
            return true;
        }

        serverSocket_ = socket(
            AF_INET,
            SOCK_STREAM,
            0
        );

        if (serverSocket_ < 0) {
            std::cerr
                << "Failed to create health server socket."
                << std::endl;

            return false;
        }

        int option = 1;

        setsockopt(
            serverSocket_,
            SOL_SOCKET,
            SO_REUSEADDR,
            &option,
            sizeof(option)
        );

        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = htonl(INADDR_ANY);
        address.sin_port = htons(
            static_cast<uint16_t>(port_)
        );

        if (bind(
                serverSocket_,
                reinterpret_cast<sockaddr*>(&address),
                sizeof(address)
            ) < 0) {

            std::cerr
                << "Failed to bind health server to port "
                << port_
                << "."
                << std::endl;

            close(serverSocket_);
            serverSocket_ = -1;

            return false;
        }

        if (listen(serverSocket_, 16) < 0) {
            std::cerr
                << "Failed to listen on health server port."
                << std::endl;

            close(serverSocket_);
            serverSocket_ = -1;

            return false;
        }

        running_ = true;

        thread_ = std::thread(
            &HealthServer::loop,
            this
        );

        std::cout
            << "Health server listening on port "
            << port_
            << "."
            << std::endl;

        return true;
    }

    void stop() {
        if (!running_) {
            return;
        }

        running_ = false;

        if (serverSocket_ >= 0) {
            shutdown(
                serverSocket_,
                SHUT_RDWR
            );

            close(serverSocket_);
            serverSocket_ = -1;
        }

        if (thread_.joinable()) {
            thread_.join();
        }
    }

private:
    void loop() {
        while (running_) {
            sockaddr_in clientAddress{};
            socklen_t clientLength =
                sizeof(clientAddress);

            const int clientSocket =
                accept(
                    serverSocket_,
                    reinterpret_cast<sockaddr*>(
                        &clientAddress
                    ),
                    &clientLength
                );

            if (clientSocket < 0) {
                if (running_) {
                    std::this_thread::sleep_for(
                        std::chrono::milliseconds(100)
                    );
                }

                continue;
            }

            char request[2048]{};

            recv(
                clientSocket,
                request,
                sizeof(request) - 1,
                0
            );

            const char response[] =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain; "
                "charset=utf-8\r\n"
                "Content-Length: 2\r\n"
                "Connection: close\r\n"
                "\r\n"
                "OK";

            send(
                clientSocket,
                response,
                sizeof(response) - 1,
                0
            );

            shutdown(
                clientSocket,
                SHUT_RDWR
            );

            close(clientSocket);
        }
    }

    int port_;
    int serverSocket_ = -1;
    bool running_ = false;
    std::thread thread_;
};

}

int main() {
    const int port = getPort();

    HealthServer healthServer(port);

    if (!healthServer.start()) {
        std::cerr
            << "Health server failed to start."
            << std::endl;

        return 1;
    }

    AppConfig config;

    if (!ConfigLoader::load("", config)) {
        std::cerr
            << "Failed to load environment configuration."
            << std::endl;

        healthServer.stop();

        return 1;
    }

    AccessControl accessControl(
        config.owner_id
    );

    BotManager botManager;

    Bot mainBot;
    mainBot.number = 1;
    mainBot.bot_id = "main";
    mainBot.token = config.bot_token;
    mainBot.owner_id = config.owner_id;
    mainBot.added_by = config.owner_id;

    if (!botManager.addBot(mainBot)) {
        std::cerr
            << "Failed to register main bot."
            << std::endl;

        healthServer.stop();

        return 1;
    }

    BotService botService(
        mainBot.number,
        mainBot.token,
        botManager,
        accessControl
    );

    if (!botService.start()) {
        std::cerr
            << "Failed to start bot service."
            << std::endl;

        healthServer.stop();

        return 1;
    }

    std::cout
        << "RestrictionMasterCpp started successfully!"
        << std::endl;

    std::cout
        << "Main bot service is running."
        << std::endl;

    while (botService.isRunning()) {
        std::this_thread::sleep_for(
            std::chrono::seconds(1)
        );
    }

    healthServer.stop();

    return 0;
}
