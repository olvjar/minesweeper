# `minesweeper`

[![GitHub version](https://badge.fury.io/gh/olvjar%2Fminesweeper.svg)](https://github.com/olvjar/minesweeper) [![GitHub latest commit](https://badgen.net/github/last-commit/olvjar/minesweeper)](https://github.com/olvjar/minesweeper/commit) [![GPL license](https://img.shields.io/badge/License-GPLv2.0-blue.svg)](https://spdx.org/licenses/GPL-2.0-only.html) [![GitHub forks](https://img.shields.io/github/forks/olvjar/minesweeper.svg?style=social&label=Fork&maxAge=2592000)](https://github.com/olvjar/minesweeper)

The classic Minesweeper game written in C  
by [olvjar](https://github.com/olvjar) and [marieltmdg](https://github.com/marieltmdg)

A collaborative university machine project turned small passion project. This was made with a focus on learning C programming and CLI interactions. It is a simple ASCII command line version of minesweeper.

### Features

- Arrow key controls and input
- Create, edit, and play custom game levels
- Create player profiles
- Persistent player data
- Player leaderboard based on time won 🏆🎉

### To-do

No longer bounded by restrictions, we are revamping the project with these goals in mind:

- [X] Key inputs for flag and inspecting on gameplay
- [ ] Key inputs for level editing
- [ ] Arrow keys for selecting profiles and custom levels
- [ ] Persistent game timer
- [ ] New leaderboard system
- [ ] Rewriting more efficient modules
- [ ] Make How to Play page
- [ ] Make consistent instructions for each page

## Documentation

### Installation

Ensure [gcc]() and [make]() is installed. These are the only dependencies required to run this project.

In your `~/path/` clone the repository

```
git clone https://github.com/olvjar/minesweeper.git
```

then change directory into the cloned repository

```
cd minesweeper
```

To run the game:

```
gcc -Wall main.c -o main
```

## License

minesweeper by Gem Grino and Mariel Tamondong is licensed under GNU General Public License v2.0 only. To view a copy of this license, visit [https://spdx.org/licenses/GPL-2.0-only.html](https://spdx.org/licenses/GPL-2.0-only.html)
