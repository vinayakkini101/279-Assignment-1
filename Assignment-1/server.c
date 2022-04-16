// Server side C/C++ program to demonstrate Socket programming

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>

#define PORT 80
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // Show ASLR
    printf("execve=0x%p\n", execve);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attaching socket to port 80
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR ,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 80
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("\njj\n");
    if((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0)
    {
        
        perror("accept");
        exit(EXIT_FAILURE);
    }
     
    struct passwd * userNobody; 
    // forking the current process
    pid_t thisProcess = fork();

    //If the current process is the child process then drop privileges
    if(thisProcess==0){
        printf("User id of child process : %d \n", getuid());

        // get user id for the 'nobody' user
        if((userNobody=getpwnam("nobody"))==NULL){
            printf("Cannot find nobody user details \n"); 
        }

        // Dropping the privilege
        int newUserID = setuid(userNobody->pw_uid);

        //Succesfully dropped privilege
        if(newUserID==0)
        {
            printf("user id of child after dropping privilege: %d \n", getuid());
            valread = read(new_socket, buffer, 1024);
            send(new_socket, hello, strlen(hello), 0);
            printf("Hello message sent \n");
        }
        else{
            printf("Privilege drop is unsuccesfull. Exiting \n %d ", newUserID);
            return 0;
        }
    }
    return 0;
}
