#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <ctype.h>
#include <cstring>
#include <string>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>


using namespace std;


void doprocessing (int sock) {
    int n;
    char buffer[256];
    bzero(buffer,256);
    n = read(sock,buffer,255);

    if (n < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }

    printf("Here is the message: %s\n",buffer);
    n = write(sock,"I got your message",18);

    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }

}

int main(int argc, char** argv) {
    string ip;
    uint16_t port = 0;
    string dir;

    int opt;

    while ((opt = getopt(argc, argv, "h:p:d:")) != -1) {
        switch (opt) {
            case 'h':
                ip = optarg;
                break;
            case 'p':
                port = (uint16_t) atoi(optarg);
                break;
            case 'd':
                dir = optarg;
                break;
            default: /* '?' */
                fprintf(stderr, "Usage: -h <ip> -p <port> -d <directory>\n");
                exit(EXIT_FAILURE);
        }
    }
    //starting a daemon
    int process_id = fork();

    if (process_id < 0) {
        cout << "Fork failed\n" << endl;
        exit(1);
    }

    //parent process. KILL HIM!!
    if (process_id > 0) {
        cout << "I am parent and I am going to die\n";
        exit(0);
    }

    umask(0);
    int sid = setsid();
    if(sid < 0) {
        // Return failure
        exit(1);
    }

    chdir(dir.c_str());

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    //now lets start the server socket
    int sock_fd, newsock_fd,cli_len;
    char buffer[1024];
    struct sockaddr_in serv_addr, cli_addr;
    int n, pid;

    /* First call to socket() function */
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    serv_addr.sin_port = htons(port);

    /* Now bind the host address using bind() call.*/
    if (bind(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    /* Now start listening for the clients, here
       * process will go in sleep mode and will wait
       * for the incoming connection
    */

    listen(sock_fd,5);
    cli_len = sizeof(cli_addr);

    while (1) {
        newsock_fd = accept(sock_fd, (struct sockaddr *) &cli_addr, (socklen_t*) &cli_len);

        if (newsock_fd < 0) {
            perror("ERROR on accept");
            exit(1);
        }

        /* Create child process */
        pid = fork();

        if (pid < 0) {
            perror("ERROR on fork");
            exit(1);
        }

        if (pid == 0) {
            /* This is the client process */
            close(sock_fd);
            doprocessing(newsock_fd);
            exit(0);
        }
        else {
            close(newsock_fd);
        }

    } /* end of while */




    cout << ip << ' ' << port << ' ' << dir << endl;
        return 0;
}