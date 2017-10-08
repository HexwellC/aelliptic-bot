// AElliptic Bot - bot for Ungovernable IRC
// Copyright (C) 2017  HexwellC
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or any
// later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// Based on René Nyffenegger's work
// Source, obviously, altered.

#include "base64.hpp"
#include <vector>
#include <boost/algorithm/string.hpp>

static const std::string chars = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, 
                          unsigned int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + 
                              ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + 
                              ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            for(i = 0; (i <4) ; i++) ret += chars[char_array_4[i]];
            i = 0;
        }
    }
    if (i) {
        for(j = i; j < 3; j++)
        char_array_3[j] = '\0';
        char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) +
                          ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) +
                          ((char_array_3[2] & 0xc0) >> 6);
        for (j = 0; (j < i + 1); j++) ret += chars[char_array_4[j]];
        while((i++ < 3)) ret += '=';
    }
  return ret;
}

std::string base64_decode(std::string const& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret = "";
    while (in_len-- && 
          (encoded_string[in_] != '=') && 
          is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_];
        in_++;
        if (i ==4) {
            for (i = 0; i <4; i++) 
                char_array_4[i] = chars.find(char_array_4[i]);
            char_array_3[0] = (char_array_4[0] << 2) + 
                              ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + 
                              ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            for (i = 0; (i < 3); i++) ret += char_array_3[i];
            i = 0;
        }
    }
    if (i) {
        for (j = 0; j < i; j++)
            char_array_4[j] = chars.find(char_array_4[j]);
        char_array_3[0] = (char_array_4[0] << 2) + 
                          ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + 
                          ((char_array_4[2] & 0x3c) >> 2);
        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }
  return ret;
}

namespace aelliptic { namespace commands {
    void base64(std::string& from, std::vector<std::string>& params,
                irc::IRCClient* client) {
        if (params.size() < 3) {
            client->send_msg(from, "Syntax: /base64 "
                                   "<encode/decode> <string>");
            return;
        }
        
        std::string response;
        if(params[1] == "decode") {
            response = base64_decode(params[2]);
        } else if (params[1] == "encode") {
            response = base64_encode(reinterpret_cast
                                    <const unsigned char*>(params[2].c_str()), 
                                     params[2].length());
        } else {
            client->send_msg(from, "Syntax: base64 "
                                   "<encode/decode> <string>");
            return;
        }
        
        client->send_msg(from, response);
    }
}}
