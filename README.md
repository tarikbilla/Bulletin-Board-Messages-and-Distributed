# Bulletin Board Messages and Distributed
To complete your project successfully, you need to follow these steps:

# Project Structure

bulletin_board_project/

├── src/

│ ├── main.c

│ ├── server.c

│ ├── server.h

│ ├── sync.c

│ ├── sync.h

│ └── config.txt

├── build/

└── Makefile

#Install & Run Commands

## Install And Update WSL in Window OS
Insall WSL: 

    - wsl --install

Update WSL: 

    -  wsl --Update

Set WSL 2 as a default version: 

    - wsl --set-default-version 2

Open ubantoo and set user name & password

 ## Install GCC (GNU Compiler Collection) in Linux Ubuntu
    - sudo apt update

    - sudo apt install build-essential

## Install the WSL extension in VSCode and remote access
Extension: Remote Development (microsoft)

Open project in VSCode:

    - In Visual Studio Code, press F1
    
    - Type "Remote-WSL: New Window" and press Enter

## Install WSL and a Linux Distribution (Windows PC)
    - dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart

    - dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart

## Update Package Lists and Install Essential Packages (Linux PC)
    - sudo apt update

    - sudo apt upgrade

sudo apt install build-essential coreutils

Verify Shell using command: 
    - echo $SHELL

## C program to develop this project

File structure:

bulletin_board_project/
├── src/
│   ├── main.c
│   ├── server.c
│   ├── server.h
│   ├── sync.c
│   ├── sync.h
│   └── config.txt
├── build/
└── Makefile



Compile: make 
Run : ./build/bbserv



# Project Phases and Tasks

## Phase 1: A Bulletin Board Server

### Tasks
- **Construct a Bulletin Board Server:**
  - Accept one-line messages from clients.
  - Store messages in a local file named by the `bbfile` parameter.
  - Serve stored messages back to clients on request.
  
- **Implement Concurrency Control:**
  - Ensure the server can handle multiple client requests concurrently.

### Key Points
- **Application Protocol:**
  - Use a strict protocol for client-server communication.
  - Commands and responses consist of a single line of text terminated by `\n` or `\r\n`.

- **Configuration Parameters:**
  - Manage parameters like `bp` (client-server communication port), `sp` (inter-server communication port), `bbfile` (bulletin board file name), and others.

- **Testing:**
  - Use telnet or any client capable of sending and receiving plain text to test your server.

## Phase 2: Data Replication

### Tasks
- **Implement Data Replication:**
  - Replicate and synchronize the bulletin board file across multiple servers.
  - Use the `peers` parameter to list other servers participating in synchronization.
  - Listen on `sp` port for synchronization requests in addition to `bp` port for client requests.
  
- **Use Two-Phase Commit Protocol:**
  - **Precommit Phase:**
    - Master server broadcasts a “precommit” message to peers and waits for acknowledgments.
  - **Commit Phase:**
    - Master server sends a “commit” message to peers if all acknowledgments are positive, followed by the necessary data for the operation.

## Implementation and Testing

### Tasks
- **Configuration:**
  - Set up configuration files with necessary parameters.
  
- **Debugging:**
  - Implement debugging facilities controlled by the `D` parameter.
  
- **Testing Environment:**
  - Ensure your code works in the specified reference and staging computing environments.

## Submission

### Requirements
- **Code:**
  - Submit your working code with a suitable makefile to build the executable `bbserv`.
  
- **Documentation:**
  - Include a brief user guide, algorithm descriptions, and necessary comments in the code.
  
- **README File:**
  - Provide a README file with your collaborators' names and email addresses, and any additional information about your implementation.
  
- **Submission Method:**
  - Submit your work electronically to the specified server (`linux.ubishops.ca`) without creating any archive.

## Grading

### Marking Scheme
- The marking scheme reflects all requirements, including proper implementation, documentation, and functionality of the server.

By following these steps and ensuring each phase and requirement is met, you can successfully complete your project. If you have any specific questions or need further details, feel free to ask!

