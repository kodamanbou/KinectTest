#include "udp_sender.h"
#include <boost/archive/binary_oarchive.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/serialization/map.hpp>

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

void udp_sender::send_data(map<string, k4abt_joint_t>& data) {
    string buffer;
    boost::iostreams::back_insert_device<string> inserter(buffer);
    boost::iostreams::stream<boost::iostreams::back_insert_device<string>> ostr(inserter);
    boost::archive::binary_oarchive oa(ostr);
    oa << data;
    ostr.flush();

    sendto(sock, buffer.c_str(), sizeof(buffer.c_str()), 0, (struct sockaddr *) &addr, sizeof(addr));
    cout << "Sent " << sizeof(buffer.c_str()) << "bytes" << endl;
}
