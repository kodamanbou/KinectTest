#include "udp_receiver.h"

void udp_receiver::init_sock() {
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
    addr.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(sock, (struct sockaddr *)&addr, sizeof(addr));
}

void udp_receiver::close_sock() const {
    closesocket(sock);
    WSACleanup();
}

void udp_receiver::receive_data() {
}
