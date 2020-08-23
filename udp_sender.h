#ifndef KINECTTEST_UDP_SENDER_H
#define KINECTTEST_UDP_SENDER_H

#include <iostream>
#include <winsock.h>
#include <k4abt.h>
#include <map>

using namespace std;

class udp_sender {
    SOCKET sock;
    WSAData wsadata;
    struct sockaddr_in addr;

public:
    void init_sock();

    void close_sock() const;

    void send_data(map<int, k4abt_joint_t> &data);
};

#endif //KINECTTEST_UDP_SENDER_H
