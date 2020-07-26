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

    void send_data(map<string, k4abt_joint_t> &data);
};

namespace boost {
    namespace serialization {
        template<class Archive>
        void serialize(Archive &ar, k4abt_joint_t& joint, const unsigned int version) {
            ar & joint.position & joint.orientation & joint.confidence_level;
        }

        template<class Archive>
        void serialize(Archive &ar, k4a_float3_t& floats, const unsigned int version) {
            ar & floats.xyz.x & floats.xyz.y & floats.xyz.z & floats.v;
        }

        template<class Archive>
        void serialize(Archive &ar, k4a_quaternion_t& quaternion, const unsigned int version) {
            ar & quaternion.wxyz.w & quaternion.wxyz.x & quaternion.wxyz.y & quaternion.wxyz.z & quaternion.v;
        }
    }
}

#endif //KINECTTEST_UDP_SENDER_H
