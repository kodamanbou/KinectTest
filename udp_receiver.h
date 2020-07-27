#ifndef KINECTTEST_UDP_RECEIVER_H
#define KINECTTEST_UDP_RECEIVER_H

#include <iostream>
#include <winsock.h>
#include <k4abt.h>
#include <map>

class udp_receiver {
    SOCKET sock;
    WSAData wsadata;
    struct sockaddr_in addr;
    char buff[1024];

public:
    void init_sock();

    void close_sock() const;

    std::map<std::string, k4abt_joint_t> receive_data();
};


#endif //KINECTTEST_UDP_RECEIVER_H
