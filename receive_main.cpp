#include "udp_receiver.h"

int main() {
    json j;
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
            j = receiver.receive_data();
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
            receiver.close_sock();
            return 2;
        }
        std::cout << "Lower W: " << j["LeftLowerArm"]["orientation"]["w"] << " ";
        std::cout << "Lower X: " << j["LeftLowerArm"]["orientation"]["x"] << " ";
        std::cout << "Lower Y: " << j["LeftLowerArm"]["orientation"]["y"] << " ";
        std::cout << "Lower Z: " << j["LeftLowerArm"]["orientation"]["z"] << std::endl;
    }

    receiver.close_sock();

    return 0;
}
