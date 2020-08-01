#include "udp_sender.h"
#include "custom_serializer.h"
#include <boost/archive/binary_oarchive.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/serialization/map.hpp>

BOOST_CLASS_IMPLEMENTATION(k4abt_joint_t , boost::serialization::object_class_info);
BOOST_CLASS_IMPLEMENTATION(k4a_float3_t , boost::serialization::object_serializable);
BOOST_CLASS_IMPLEMENTATION(k4a_quaternion_t , boost::serialization::object_serializable);

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

void udp_sender::send_data(map<string, k4abt_joint_t>& data) {
    string buffer;
    boost::iostreams::back_insert_device<string> inserter(buffer);
    boost::iostreams::stream<boost::iostreams::back_insert_device<string>> ostr(inserter);
    boost::archive::binary_oarchive oa(ostr);
    oa << data;
    ostr.flush();

    sendto(sock, buffer.c_str(), buffer.size(), 0, (struct sockaddr *) &addr, sizeof(addr));
    cout << "Sent " << buffer.size() << "bytes" << endl;
}
