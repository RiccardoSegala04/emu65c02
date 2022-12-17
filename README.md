<img width="652" alt="Screenshot 2022-12-17 alle 14 55 02" src="https://user-images.githubusercontent.com/72670063/208245402-521f799d-3daf-4b04-85df-282abc8ad797.png">

## Introduction
`emu65c02` is a cross-platform emulator and debugger for a custom 65c02 based board computer.

`emu65c02` allows to try out programs written for a custom board, and view the value of registers, memory and I/O  without loading it on the phisical rom.

Running `emu65c02` requires `ncurses` libraries, typically named libncurses(w).

`emu65c02` is written in C.

## Build instructions

This program requires `ncurses` libraries to work.

Install these and other required packages for C development from your package manager.

#### Debian
```
sudo apt install libncursesw5-dev build-essential
```
#### Fedora
```
sudo dnf install ncurses-devel gcc
```
#### Archlinux
```
sudo pacman -S ncurses gcc
```
#### macOS
```
brew install ncurses
```

### Build from source
To compile from source, download from the Git repository (git clone or downloads from GitHub releases), then run:
```
Make
```
The executable `emu` will be in the `rel` folder

## Usage
Type
```
./rel/emu <path_to_rom>
```
The emulator will start executing the rom passed by command arguments
