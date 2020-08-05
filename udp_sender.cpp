#include "udp_sender.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void udp_sender::init_sock() {
    int err = WSAStartup(MAKEWORD(1, 0), &wsadata);
    if (err != 0) {
        switch (err) {
            case WSASYSNOTREADY:
                cout << "WSASYSNOTREADY" << endl;
                break;
            case WSAVERNOTSUPPORTED:
                cout << "WSAVERNOTSUPPORTED" << endl;
                break;
            case WSAEINPROGRESS:
                cout << "WSAEINPROGRESS" << endl;
                break;
            case WSAEPROCLIM:
                cout << "WSAEPROCLIM" << endl;
                break;
            case WSAEFAULT:
                cout << "WSAEFAULT" << endl;
                break;
            default:
                break;
        }
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(49875);
    addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
}

void udp_sender::close_sock() const {
    closesocket(sock);
    WSACleanup();
}

void udp_sender::send_data(map<string, k4abt_joint_t> &data) {
    string buffer;
    json j;
    for (pair<string, k4abt_joint_t> p:data) {
        j[p.first]["orientation"]["w"] = p.second.orientation.wxyz.w;
        j[p.first]["orientation"]["x"] = p.second.orientation.wxyz.x;
        j[p.first]["orientation"]["y"] = p.second.orientation.wxyz.y;
        j[p.first]["orientation"]["z"] = p.second.orientation.wxyz.z;
        j[p.first]["position"]["x"] = p.second.position.xyz.x;
        j[p.first]["position"]["y"] = p.second.position.xyz.y;
        j[p.first]["position"]["z"] = p.second.position.xyz.z;
    }

    buffer = j.dump();
    sendto(sock, buffer.c_str(), buffer.size(), 0, (struct sockaddr *) &addr, sizeof(addr));
    cout << "Sent " << buffer.size() << "bytes" << endl;
}
