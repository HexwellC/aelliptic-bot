# AElliptic Bot

AElliptic Bot - Telegram bot developed specially for [AEF 
group](https://t.me/aefoundation). The goal is to provide some new or already 
seen functionality but implemented in C++ ¯\\\_(ツ)\_/¯

## I wanna contribut this shit

Sure, you can try to contribute it. You're probably interested in 
following files:

* src/commands.hpp - command registration
* src/commands/ - commands implementations

Each new command's main file (both header and source files) should start 
with `c` symbol. If your command is huge, you probably want to create a 
subdirectory with the same name and divide it into multiple sources and 
headers. All files in `src/commands/` and subdirectories are added to 
CMake automagically. `cconvert` is a good example to start from. 
`src/commands/ccore.hpp` provides some useful utilities and pointer to 
TgBotAPI, which is needed to send messages, so you probably want to 
include it. Always try to minimize number of includes in headers.
Styling is simple: 80 symbols per line, snake_case prefered, license 
header required everywhere and, obviously, 4 spaces.
If you don't understand something, just look for an example or ask us on 
our Telegram group (link above).
