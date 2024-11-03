# f1_22_telemetry
A telemetry parser for Codemaster's game F1 22 written in C++

## Features
- Parses udp telemetry packets obtained from the game.
- Processes some of the information from the data and display it as a scoreboard on the gui window.
- Optional functionality to connect to an external server socket. Used in-house to connect to a discord bot and upload race results to that discord bot.
- Same optional feature as above also offers exposing an api endpoint on localhost:8080 (can be configured)

## Server usage

- Host url and port for the socket client can be found in constants.cpp.
- Similarly the api server port and host can be configured in constants.cpp.
- RaceData can be found on "host/api/raceData"
- The client socket sends data as a json object which behaves like a python dict with the key 'raceData'.

## Dependencies

- Qt Framework (Core, Gui, Network, Widgets) 6.8 [https://www.qt.io/download-open-source#contributing]
- Nhlohmann's Json library [https://github.com/nlohmann/json]

## Building/Compiling

- I would personally prefer and recommend CMake to build your project.
- Use the CMakeLists from the project for comparison. Path to nlohmann::json needs to be changed but ideally Qt can be stored as an environment variable and accessed directly.
- The code assumes that you build into **f1_22_telemetry/build** otherwise the assets will break. Asset paths can be changed from constants.cpp

## Credits

- The logos and the tyre icons in the assets were obtained from [pngegg.com](#pngegg).
- The Open Source version of Qt Framework was used for development of this project. It is licensed under GNU GPL. Hence if code from this project which includes usage of Qt Framework is used in any other project, you are bound to provide appropriate credit to the Qt foundation and keep the code open source. (I would appreciate if you credit me too =D)
- The JSON library used from this repository: Lohmann, N. (2023). JSON for Modern C++ (Version 3.11.3) [Computer software]. https://github.com/nlohmann
