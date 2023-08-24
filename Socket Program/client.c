/* A simple echo client using TCP */
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>



#define SERVER_TCP_PORT 3002    /* well-known port */
#define BUFLEN        100    /* buffer length: Maximum bytes is 100 */
#define FILESIZE      1500   //max size of ethernet byte is 1500 bytes to that should be max file size
int getFile(int,char[],int);


int main(int argc, char **argv)
{
    int     n,i;
    int     sd, port;
    struct    hostent        *hp;
    struct    sockaddr_in server,reg_addr;
    char    *host;
    
    //Uncomment for lab use
    /*
    switch(argc){
    case 2:
        host = argv[1];
        port = SERVER_TCP_PORT;
        break;
    case 3:
        host = argv[1];
        port = atoi(argv[2]);
        break;
    default:
        fprintf(stderr, "Usage: %s host [port]\n", argv[0]);
        exit(1);
    }
     */
    
    port=SERVER_TCP_PORT; //For home use
    
    /* Create a stream socket    */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "Can't creat a socket\n");
        exit(1);
    }
    
    bzero((char *)&server, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    //Comment out for lab use
    server.sin_addr.s_addr=INADDR_ANY; //for home use
    
    //Uncomment for lab use
    /*
    if (hp = gethostbyname(host))
        bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);
    else if ( inet_aton(host, (struct in_addr *) &server.sin_addr) ){
        fprintf(stderr, "Can't get server's address\n");
        exit(1);
    }
    */
    /* Setup TCP socket */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "Can't create a socket\n");
        exit(EXIT_FAILURE);
    }
    bzero((char*)&reg_addr, sizeof(struct sockaddr_in));
    reg_addr.sin_family = AF_INET;
    reg_addr.sin_port = htons(0);
    reg_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (bind(sd,(struct sockaddr*)&reg_addr, sizeof(reg_addr)) == -1){
        fprintf(stderr, "Can't bind name to socket\n");
        exit(EXIT_FAILURE);
    }
    

    
    /* Connecting to the server */
    if (connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1){
        fprintf(stderr, "Can't connect \n");
        exit(1);
    }
    printf("Enter filename with extension: ");
    char name[BUFLEN];
    char buf[BUFLEN];
    while(n=read(0, name, BUFLEN)){
        break;
    }
    name[n-1]='\0';
    write(sd, name, n);
    while (i=read(sd, buf, BUFLEN)) {
        break;
    }
    buf[i-1]='\0';
    if(strcmp(buf, "success")==0 && getFile(sd,name,n)==1){
        printf("File succesfully received\n");
    }else{
        printf("Error receiving file\n");
    }
    close(sd);
    return(0);
}

int getFile(int socket,char filename[],int namesize){
    char buf[FILESIZE];
    int n;
    char path[namesize+sizeof("/Users/anthonyvalenti/Documents/Fourth Year/COE768/Lab3Client/")];  //replace string with own directory
    snprintf(path, sizeof(path), "%s%s","/Users/anthonyvalenti/Documents/Fourth Year/COE768/Lab3Client/",filename);  //replace string with own directory
    FILE *file;
    file=fopen(path, "w");
    if(file==NULL){
        fprintf(stderr, "Error creating file\n");
        return -1;
    }
    while (1) {
        n=read(socket, buf, 100);
        if(n<=0){
            break;
        }
        fprintf(file, "%s",buf);
        bzero(buf, FILESIZE);
        
    }
    fclose(file);
    fopen(path, "r");
    printf("Displaying file contents from %s\n",path);
    while(fgets(buf, FILESIZE, file)){
        printf("%s",buf);
    }
    return 1;
    
}
