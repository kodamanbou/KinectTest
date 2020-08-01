#include "udp_receiver.h"

int main() {
    std::map<std::string, k4abt_joint_t> bones;
    udp_receiver receiver{};

    try {
        receiver.init_sock();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        receiver.close_sock();
        return 1;
    }

    for (int i = 0; i < 100; ++i) {
        try {
            bones = receiver.receive_data();
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
            receiver.close_sock();
            return 2;
        }
        std::cout << "Head X: " << bones["Head"].position.xyz.x << std::endl;
    }

    receiver.close_sock();

    return 0;
}
