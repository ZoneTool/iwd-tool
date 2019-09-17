![license](https://img.shields.io/github/license/ZoneTool/iwd-tool.svg)
![stars](https://img.shields.io/github/stars/ZoneTool/iwd-tool.svg)
[![discord](https://discordapp.com/api/guilds/219514629703860235/widget.png)](https://discord.gg/a6JM2Tv)
<p align="center"><img src="plutonium_logo.jpg" alt="Plutonium"/>

# iwd-tool
iwd-tool, a command-line tool for generating iwd files. The main purpose of this tool is to filter out duplicated images between Call of Duty: Modern Warfare 2 and Call of Duty: Online.

## Folder structure
iwd-tool\
|- iw4_images\
|- codo_images\
|- iwd_tool.exe\
|- dump.cfg

## Example cfg file
```
dumpraw images/white.iwi
dumpraw images/black.iwi
```

## Usage
First you need to create a .cfg file with all the images you want to include in your IWD. IW4x can generate this file automatically by starting the game with -dump. Execute the following command to find all the required images: ``./iwd-tool.exe <cfg file>``. For example: ``./iwd-tool.exe dump.cfg``
