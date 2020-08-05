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
        std::cout << "Head X: " << j["Head"]["position"]["x"] << std::endl;
    }

    receiver.close_sock();

    return 0;
}
