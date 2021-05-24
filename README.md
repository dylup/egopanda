# egopanda
CSC-842 Cycle1

## Description
- Course: CSC842 Security Tool Development
- Developer: Dylan Johnson
- Language: C
- Scope: Offensive Security/Penetration Testing


## Description
Linux x86-64 based shell/remote administration tool. This project is intended to be used after successful exploitation of a host during a penetration test/offensive security assesment. Kind of a pet project at the moment. 

## Capabilities
EGOPANDA is capable of running commands at the same level as the compromised user of a remote host. The shell can attempt to hide itself by deleting the original binary from disk, copying itself to other locations, and renaming the running process to evade detection. There is also functionality for persistence in the form of CRON jobs. 

### Functionality
The primary function of this software is to remotely interact with a compromised system.

## Future Work
This is a very basic shell. Future work includes adding additional functionality such as file transfer, privelege escalation, screenshots, audio recording, etc. Building a dedicated administration platform for the operator is also an option.

