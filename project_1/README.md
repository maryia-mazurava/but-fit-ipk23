# IPK - Computer communications and networks
## Project 1
### Author: Maryia Mazurava (xmazur08)
### [xmazur08@stud.fit.vutbr.cz](mailto:xmazur08@stud.fit.vutbr.cz)

## Description
Implementation of a client for the IPK Calculator Protocol in C language. The client will use the appropriate protocol variant depending on the selected mode (binary for UDP, textual for TCP). It will read a list of commands from the standard input, one per line, and print the received responses on the standard output. 

## File compilation and running
Using Makefile in the root directory of the project (make command):
```
    $ ls
    
    ipkcpc.c Makefile Readme.md  
    
    $ make
    
    gcc -Wall -Werror ipkcpc.c -o ipkcpc
    
    $ ls
     
    ipkcpc  ipkcpc.c  Makefile Readme.md  
```
The client is started using: 
```
./ipkcpc -h <host> -p <port> -m <mode> 
```
```
host  - the IPv4 address of the server.
port  - the server port.
mode  - tcp/udp.
```
For help message type the following:
```
./ipkcpc --help
```

## Project structure
Client parses arguments in the **main()** function. Based on the 'mode' argument the functions **udpMode()** or **tcpMode()** are called. 

## Testing
```
./ipkcpc -h 127.0.0.1 -p 2023 -m udp
[Input] (+ 1 3)
[Output] OK: 4
./ipkcpc -h 127.0.0.1 -p 2023 -m udp
[Input] (+ 1 f)
[Output] ERR:Could not parse the message
```
```
./ipkcpc -h 127.0.0.1 -p 2023 -m tcp
[Input] HELLO
[Output] HELLO
[Input] SOLVE (+ 1 3)
[Output] RESULT 4
[Input] BYE
[Output] BYE
         Closing the connection...
         Connection closed.
```
## Used sources
- Lectures https://moodle.vut.cz/mod/folder/view.php?id=289124
- https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/?ref=lbp




