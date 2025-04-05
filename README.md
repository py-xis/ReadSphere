#### Directory Structure

```
.
├── README.md
├── client
│   ├── include
│   │   ├── authenticationClient.h
│   │   ├── bookManagementClient.h
│   │   ├── client.h
│   │   └── tokenizeString.h
│   └── src
│       ├── authenticationClient.c
│       ├── authenticationClient.o
│       ├── bookManagementClient.c
│       ├── bookManagementClient.o
│       ├── client.c
│       ├── client.o
│       ├── tokenizeString.c
│       └── tokenizeString.o
└── server
    ├── data
    │   ├── book.dat
    │   └── user.dat
    ├── include
    │   ├── authentication.h
    │   ├── bookManagement.h
    │   ├── completeRequest.h
    │   ├── components.h
    │   ├── lock.h
    │   ├── server.h
    │   └── tokenizeString.h
    └── src
        ├── authentication.c
        ├── bookManagement.c
        ├── completeRequest.c
        ├── lock.c
        ├── server.c
        └── tokenizeString.c
```

### Make sure to change the paths of the user.dat and book.dat files in authentication.c and bookManagement.c in the server directory.
### Make sure that there are no file other than .c file while compiling in the src directories of either the client or the server.

#### Instructions to run the server
1. Open the project directory.
2. cd server
3. gcc src/* -o server -I include
4. ./server

#### Instructions to run the client
1. Open the project directory
2. cd client
3. gcc src/* -o client -I include
4. ./client

# ReadSphere
