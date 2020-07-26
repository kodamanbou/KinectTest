#ifndef KINECTTEST_UDP_RECEIVER_H
#define KINECTTEST_UDP_RECEIVER_H

#include <iostream>
#include <winsock.h>
#include <k4abt.h>
#include <map>

using namespace std;

class udp_receiver {
    SOCKET sock;
    WSAData wsadata;
    struct sockaddr_in addr;
    char buff[1024];

public:
    void init_sock();

    void close_sock() const;

    void receive_data();
};


#endif //KINECTTEST_UDP_RECEIVER_H
