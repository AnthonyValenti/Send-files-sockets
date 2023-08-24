/* A simple echo server using TCP */
#include <stdio.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <strings.h>


#define SERVER_TCP_PORT 3002    /* well-known port */
#define BUFLEN        100    /* buffer length: maximum length of 100 */
#define FILESIZE      1500   //max size of ethernet byte is 1500 bytes to that should be max file size

int echod(int);
int sendFile(int,FILE *);
void reaper(int);

int main(int argc, char **argv)
{
    int     sd, new_sd, client_len, port;
    struct    sockaddr_in server, client;
    
    //Uncomment for lab use
    /*
    switch(argc){
    case 1:
        port = SERVER_TCP_PORT;
        break;
    case 2:
        port = atoi(argv[1]);
        break;
    default:
        fprintf(stderr, "Usage: %s [port]\n", argv[0]);
        exit(1);
    }
     */
    //Comment out for lab use
    port=SERVER_TCP_PORT; //for home use
    
    /* Create a stream socket    */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "Can't creat a socket\n");
        exit(1);
    }
    
    /* Bind an address to the socket    */
    bzero((char *)&server, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1){
        fprintf(stderr, "Can't bind name to socket\n");
        exit(1);
    }
    
    /* queue up to 5 connect requests  */
    listen(sd, 5);
    
    (void) signal(SIGCHLD, reaper);
    
    while(1) {
        client_len = sizeof(client);
        new_sd = accept(sd, (struct sockaddr *)&client, &client_len);
        if(new_sd < 0){
            fprintf(stderr, "Can't accept client \n");
            exit(1);
        }
        switch (fork()){
        case 0:        /* child */
            (void) close(sd);
            exit(echod(new_sd));
        default:        /* parent */
            (void) close(new_sd);
            break;
        case -1:
            fprintf(stderr, "fork: error\n");
        }
    }
}

/*    echod program    */
int echod(int sd)
{
    char    buf[BUFLEN];
    int     n;
    while(n = read(sd, buf, BUFLEN)){
        break;
    }
    char path[n+sizeof("/Users/anthonyvalenti/Desktop/")];  //replace string with own directory
    snprintf(path, sizeof(path), "%s%s","/Users/anthonyvalenti/Desktop/",buf);  //replace string with own directory
    FILE *file;
    file = fopen(path, "r");
    if(file==NULL){
        printf("File does not exist \n");
        write(sd,"failure",sizeof("failure"));
        close(sd);
    }
    else{
        printf("Sending the file \n");
        write(sd,"success",sizeof("success"));
        if(sendFile(sd, file)==1){
            printf("File sent!\n");
        }else{
            fprintf(stderr, "Error sending file\n");
            exit(1);
        }
        
    }
    close(sd);
    return(0);
}

int sendFile(int socket, FILE *f){
    int i=0;
    char data[100];
    char c;
    do {
        c = fgetc(f);
        if( feof(f) ) {
            break ;
        }
        data[i]=c;
        i++;
        if(i>99){
            write(socket, data, 100);
            bzero(data, 100);
            i=0;
        }
    } while(1);
    write(socket, data, sizeof(data));
    fclose(f);
    return 1;
}


/*    reaper        */
void    reaper(int sig)
{
    int    status;
    while(wait3(&status, WNOHANG, (struct rusage *)0) >= 0);
}

