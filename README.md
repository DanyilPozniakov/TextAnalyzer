# Text Parser

# User documentation

## Client side
Client side is external executable file with directional communicating.
Working with this part of the program going through some commands.
You can write some commands to the client session.
If a command wasn't recognized - you will get an error.

### Commands
There is will be described all possible commands.


#### connect
This command will connect a client to the specified server.
```bash
connect <ip>:<port> 
```

Example:
```bash
connect 127.0.0.1:8888
```

**Success**: Client was connected to the server: \<ip\>:\<port\>

**Error**: Error: <error_message>

#### disconnect
This command will disconnect you from the connected server. If connection wasn't estabilished - nothing happen.
```bash
disconnect
```

**Success**: Client was disconnected from the server: \<ip\>:\<port\>

**Error**: No connection


#### status
Will show you connection status and server will be pinged.
```bash
status
```

**Output**: 
Connection: \<ip\>:\<port\>
Connection status: active|not active


#### setText
Send inputed text to the connected server.
Warning: you must estabilish a connection to the server.
```bash
setText <your_text>
```
Possible result to the console:

**Success**: text was sent to the server. text metrics...!!!!!!

**Error**: Error: <error_message>


#### setFileText
Set file path to read it and send to the connected server.
Warning: you must estabilish a connection to the server.
```bash
setFileText <path_to_file>
```
Possible result to the console:

**Success**: file was sent to the server. text metrics...!!!!!!

**Error**: Error: <error_message>


#### help
Will show all commands with a description
```bash
help
```


#### exit
Just exit from the clinet. The active console will be closed all, process will be interrupted.

# Techinal documentation:

## Client side

## Server side
