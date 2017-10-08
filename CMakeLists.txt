# AElliptic Bot - bot for Ungovernable IRC
# Copyright (C) 2017  HexwellC
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or any
# later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
cmake_minimum_required(VERSION 2.8)
project(aelliptic-bot)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(Boost_USE_MULTITHREADED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system iostreams REQUIRED)
find_package(SQLite3 REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR} 
                    src ${SQLITE3_INCLUDE_DIRS})
set(BOT_SOURCES 
    src/main.cpp
    src/logging.cpp
    src/irc_client.cpp
    src/irc_handler.cpp
    src/irc_socket.cpp
    src/database.cpp)
set(BOT_COMMANDS 
    src/commands/start.cpp 
    src/commands/base64.cpp)
add_executable(aelliptic-bot ${BOT_SOURCES} ${BOT_COMMANDS})
target_include_directories(aelliptic-bot PRIVATE 
                           tgbot-cpp/include
                           ${SQLITE3_INCLUDE_DIRS})
target_link_libraries(aelliptic-bot ${CMAKE_THREAD_LIBS_INIT}
                      ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES}
                      ${SQLITE3_LIBRARIES})
