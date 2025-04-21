# DataPulse: SSH/SFTP File Synchronization Tool

## Matrials
- [Video Demo Here](https://www.youtube.com/watch?v=4Xhb-ykn9PM)
- [Github Repo](https://github.com/Franz-Kredo/DataPulse)
- [Libssh docs](https://www.libssh.org/get-it/)


## Disclaimer
- Has only been tested on Debian-based distros (bare metal & wsl). It should compile fine on most unix based systems.
- The `libssh` library needs to be installed. 
    - Fedora: `dnf install libssh-devel`
    - Debian/Ubuntu: `apt-get install libssh-dev`
    - If still unsure how to install, please follow the [docs](https://www.libssh.org/get-it/)

- why is this not available for windows:
    - because this is made as a bash application utility much like rsync
    - However, this should work with WSL on Windows


## Overview
DataPulse is a command-line utility that synchronizes files between local and remote directories over SSH/SFTP. It features conflict detection, resolution strategies, and checksum verification to ensure data integrity.

## Features
- File transfer using SSH/SFTP
- Automatic detection of files to synchronize
- Conflict resolution with multiple strategies
- MD5 checksum verification
- Some colored terminal interface

## Installation

### Dependencies
- libssh
- openssl/crypto

### Building
```bash
make
```

## Usage

### Basic Synchronization
```bash
./datap <host> <username> <remote_path> <local_path>
```

### With SSH Private Key
```bash
./datap <host> <username> <remote_path> <local_path> <priv_key_path>
```

### With Merge Mode (Conflict Resolution)
```bash
./datap -m <host> <username> <remote_path> <local_path>
```

### With Merge Mode (Conflict Resolution) and priv key
```bash
./datap -m <host> <username> <remote_path> <local_path> <priv_key_path>
```

### View Help
```bash
./datap -h
```

## Conflict Resolution
When conflicts are detected (files exist in both locations with different content), you can:
1. Keep the newer file
2. Keep the older file
3. Keep both files (auto-rename)
4. Skip synchronization of conflicting files

Add 'a' suffix to any option (e.g., '1a') to apply the same resolution to all remaining conflicts.

## Project Structure
- NetworkLogic: Manages SSH connections and SFTP sessions
- FileLogic: Handles file reading/writing operations
- DataLogic: Identifies syncable files and verifies checksums
- ConflictLogic: Resolves file conflicts
- IOHandler: Provides the user interface

## Authors
- Alexander Máni Einarsson
- Franz Ísak Bergmann Ingólfsson
- Tómas Orri Kristinsson
