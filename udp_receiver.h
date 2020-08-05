#ifndef KINECTTEST_UDP_RECEIVER_H
#define KINECTTEST_UDP_RECEIVER_H

#include <iostream>
#include <winsock.h>
#include <k4abt.h>
#include <map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class udp_receiver {
    SOCKET sock;
    WSAData wsadata;
    struct sockaddr_in addr;
    char buff[7168];

public:
    void init_sock();

    void close_sock() const;

    json receive_data();
};

#endif //KINECTTEST_UDP_RECEIVER_H
