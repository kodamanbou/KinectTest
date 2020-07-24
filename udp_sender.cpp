#include "udp_sender.h"

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
    addr.sin_port = htons(50000);
    addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
}

void udp_sender::close_sock() const {
    closesocket(sock);
    WSACleanup();
}

void udp_sender::send_data(const map<string, k4abt_joint_t>& data) {
}
