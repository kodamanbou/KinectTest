#include "udp_receiver.h"
#include "custom_serializer.h"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/serialization/map.hpp>

BOOST_CLASS_IMPLEMENTATION(k4abt_joint_t , boost::serialization::object_class_info);
BOOST_CLASS_IMPLEMENTATION(k4a_float3_t , boost::serialization::object_class_info);
BOOST_CLASS_IMPLEMENTATION(k4a_quaternion_t , boost::serialization::object_class_info);

void udp_receiver::init_sock() {
    int err = WSAStartup(MAKEWORD(1, 0), &wsadata);
    if (err != 0) {
        switch (err) {
            case WSASYSNOTREADY:
                std::cout << "WSASYSNOTREADY" << std::endl;
                break;
            case WSAVERNOTSUPPORTED:
                std::cout << "WSAVERNOTSUPPORTED" << std::endl;
                break;
            case WSAEINPROGRESS:
                std::cout << "WSAEINPROGRESS" << std::endl;
                break;
            case WSAEPROCLIM:
                std::cout << "WSAEPROCLIM" << std::endl;
                break;
            case WSAEFAULT:
                std::cout << "WSAEFAULT" << std::endl;
                break;
            default:
                break;
        }
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(49875);
    addr.sin_addr.S_un.S_addr = INADDR_ANY;

    int code = bind(sock, (struct sockaddr *) &addr, sizeof(addr));
    if (code == SOCKET_ERROR) {
        throw std::exception(std::to_string(WSAGetLastError()).c_str());
    }
    memset(buff, 0, sizeof(buff));
}

void udp_receiver::close_sock() const {
    closesocket(sock);
    WSACleanup();
}

std::map<std::string, k4abt_joint_t> udp_receiver::receive_data() {
    int code = recv(sock, buff, sizeof(buff), 0);
    if (code == SOCKET_ERROR) {
        throw std::exception(std::to_string(WSAGetLastError()).c_str());
    }

    boost::iostreams::basic_array_source<char> device(buff, code);
    boost::iostreams::stream<boost::iostreams::basic_array_source<char>> istr(device);
    boost::archive::binary_iarchive ia(istr);
    std::map<std::string, k4abt_joint_t> bones;
    ia >> bones;

    return bones;
}
