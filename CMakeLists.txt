cmake_minimum_required(VERSION 3.16)

project(RestrictionMasterCpp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(CURL REQUIRED)

add_executable(RestrictionMasterCpp
    src/main.cpp

    src/commands/bot_controller.cpp
    src/commands/command_handler.cpp
    src/commands/commands.cpp

    src/config/config_loader.cpp

    src/core/access_control.cpp
    src/core/bot_manager.cpp
    src/core/bot_registry.cpp

    src/database/bot_storage.cpp
    src/database/database.cpp

    src/download/download_manager.cpp

    src/progress/progress_formatter.cpp

    src/telegram/bot_service.cpp
    src/telegram/callback_handler.cpp
    src/telegram/keyboards.cpp
    src/telegram/telegram.cpp

    src/transfer/transfer_manager.cpp

    src/upload/upload_manager.cpp
)

target_include_directories(RestrictionMasterCpp PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(RestrictionMasterCpp PRIVATE
    CURL::libcurl
)
