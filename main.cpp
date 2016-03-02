#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <ctype.h>
#include <cstring>
#include <string>

using namespace std;

int main(int argc, char** argv) {
    string ip;
    int port = -1;
    string dir;
    int opt;
    while ((opt = getopt(argc, argv, "h:p:d:")) != -1) {
        switch (opt) {
            case 'h':
                ip = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'd':
                dir = optarg;
                break;
            default: /* '?' */
                fprintf(stderr, "Usage: -h <ip> -p <port> -d <directory>\n");
                exit(EXIT_FAILURE);
        }
    }




    cout << ip << ' ' << port << ' ' << dir << endl;
    return 0;
}