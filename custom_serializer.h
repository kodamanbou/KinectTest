#ifndef KINECTTEST_CUSTOM_SERIALIZER_H
#define KINECTTEST_CUSTOM_SERIALIZER_H

#include <k4abt.h>
#include <boost/serialization/array.hpp>

namespace boost {
    namespace serialization {
        template<class Archive>
        void serialize(Archive &ar, k4abt_joint_t &joint, const unsigned int version) {
            ar & joint.position & joint.orientation & joint.confidence_level;
        }

        template<class Archive>
        void serialize(Archive &ar, k4a_float3_t &floats, const unsigned int version) {
            ar & floats.xyz;
            ar & make_array(floats.v, sizeof(floats.v));
        }

        template<class Archive>
        void serialize(Archive &ar, k4a_float3_t::_xyz &xyz, const unsigned int version) {
            ar & xyz.x & xyz.y & xyz.z;
        }

        template<class Archive>
        void serialize(Archive &ar, k4a_quaternion_t &quaternion, const unsigned int version) {
            ar & quaternion.wxyz;
            ar & make_array(quaternion.v, sizeof(quaternion.v));
        }

        template<class Archive>
        void serialize(Archive &ar, k4a_quaternion_t::_wxyz &wxyz, const unsigned int version) {
            ar & wxyz.w & wxyz.x & wxyz.y & wxyz.z;
        }
    }
}

#endif //KINECTTEST_CUSTOM_SERIALIZER_H
