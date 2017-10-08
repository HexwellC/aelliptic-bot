#include "irc_handler.hpp"
#include "logging.hpp"
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <functional>
#include "commands/start.hpp"
#include "commands/base64.hpp"

namespace aelliptic { namespace irc {
    IRCCommandHandler ircCommandTable[NUM_IRC_CMDS] = {
    { "PRIVMSG",            &IRCClient::HandlePrivMsg                   },
    { "NOTICE",             &IRCClient::HandleNotice                    },
    { "JOIN",               &IRCClient::HandleChannelJoinPart           },
    { "PART",               &IRCClient::HandleChannelJoinPart           },
    { "NICK",               &IRCClient::HandleUserNickChange            },
    { "QUIT",               &IRCClient::HandleUserQuit                  },
    { "353",                &IRCClient::HandleChannelNamesList          },
    { "433",                &IRCClient::HandleNicknameInUse             },
    { "001",                &IRCClient::HandleServerMessage             },
    { "002",                &IRCClient::HandleServerMessage             },
    { "003",                &IRCClient::HandleServerMessage             },
    { "004",                &IRCClient::HandleServerMessage             },
    { "005",                &IRCClient::HandleServerMessage             },
    { "250",                &IRCClient::HandleServerMessage             },
    { "251",                &IRCClient::HandleServerMessage             },
    { "252",                &IRCClient::HandleServerMessage             },
    { "253",                &IRCClient::HandleServerMessage             },
    { "254",                &IRCClient::HandleServerMessage             },
    { "255",                &IRCClient::HandleServerMessage             },
    { "265",                &IRCClient::HandleServerMessage             },
    { "266",                &IRCClient::HandleServerMessage             },
    { "366",                &IRCClient::HandleServerMessage             },
    { "372",                &IRCClient::HandleServerMessage             },
    { "375",                &IRCClient::HandleServerMessage             },
    { "376",                &IRCClient::HandleServerMessage             },
    { "439",                &IRCClient::HandleServerMessage             },
    };

    void IRCClient::handle_ctcp(IRCMessage message) {
        std::string to = message.parameters.at(0);
        std::string text = message.parameters.at(message.parameters.size() - 1);

        // Remove '\001' from start/end of the string
        text = text.substr(1, text.size() - 2);

        std::cout  << "[" + message.prefix.nick << " requested CTCP " 
                   << text << "]";

        if (to == _nick) {
            if (text == "VERSION") { // Respond to CTCP VERSION
                send_raw("NOTICE " + message.prefix.nick + 
                         ":\001v1 - AElliptic Bot\001");
                return;
            }

            // CTCP not implemented
            send_raw("NOTICE " + message.prefix.nick + " :\001ERRMSG " + 
                text + ":Not implemented\001");
        }
    }

    static std::unordered_map<std::string, 
                              std::function<void(std::string&,
                                                 std::vector<std::string>&,
                                                 irc::IRCClient*)>>
        commands
        {
            {"!start", commands::start},
            {"!base64", commands::base64}
        };
       
    void IRCClient::HandlePrivMsg(IRCMessage message) {
        std::string to = message.parameters.at(0);
        std::string text = message.parameters.at(message.parameters.size() - 1);

        // Handle Client-To-Client Protocol
        if (text[0] == '\001') {
            handle_ctcp(message);
            return;
        }
        
        if (text[0] != '!') return;

        if (to[0] == '#') {
            std::cout << "From " + message.prefix.nick << " @ " + to + ": " 
                      << text << std::endl;
        } else {
            std::cout << "From " + message.prefix.nick << ": " 
                      << text << std::endl;
            to = message.prefix.nick;
        }
        
        message.parameters = split(text, ' ');
        
        auto it = commands.find(message.parameters[0]);
        if (it == commands.end()) return;
        it->second(to, message.parameters, this);
    }

    void IRCClient::HandleNotice(IRCMessage message) {
        std::string from = message.prefix.nick != "" ? 
                           message.prefix.nick : 
                           message.prefix.prefix;
        std::string text;

        if( !message.parameters.empty() )
            text = message.parameters.at(message.parameters.size() - 1);

        if (!text.empty() && text[0] == '\001') {
            text = text.substr(1, text.size() - 2);
            if (text.find(" ") == std::string::npos) {
                std::cout << "[Invalid " << text << " reply from " 
                          << from << "]" << std::endl;
                return;
            }
            std::string ctcp = text.substr(0, text.find(" "));
            std::cout << "[" << from << " " << ctcp << " reply]: " 
                      << text.substr(text.find(" ") + 1) << std::endl;
        } else std::cout << "-" << from << "- " << text << std::endl;
    }

    void IRCClient::HandleChannelJoinPart(IRCMessage message) {
        std::string channel = message.parameters.at(0);
        std::string action = message.command == "JOIN" ? "joins" : "leaves";
        std::cout << message.prefix.nick << " " << action << " " 
                  << channel << std::endl;
    }

    void IRCClient::HandleUserNickChange(IRCMessage message) {
        std::string newNick = message.parameters.at(0);
        std::cout << message.prefix.nick << " changed his nick to " 
                  << newNick << std::endl;
    }

    void IRCClient::HandleUserQuit(IRCMessage message) {
        std::string text = message.parameters.at(0);
        std::cout << message.prefix.nick << " quits (" << text << ")" 
                  << std::endl;
    }

    void IRCClient::HandleChannelNamesList(IRCMessage message) {
        std::string channel = message.parameters.at(2);
        std::string nicks = message.parameters.at(3);
        std::cout << "People on " << channel << ":" << std::endl 
                  << nicks << std::endl;
    }

    void IRCClient::HandleNicknameInUse(IRCMessage message) {
        std::cout << message.parameters.at(1) << " " 
                  << message.parameters.at(2) << std::endl;
    }

    void IRCClient::HandleServerMessage(IRCMessage message) {
        if(message.parameters.empty()) return;

        std::vector<std::string>
                            ::const_iterator itr = message.parameters.begin();
        ++itr; // skip the first parameter (our nick)
        for (; itr != message.parameters.end(); ++itr) std::cout << *itr << " ";
        std::cout << std::endl;
    }
}}
