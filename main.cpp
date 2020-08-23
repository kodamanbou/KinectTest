#pragma comment(lib, "k4a.lib")

#include <iostream>
#include <map>
#include <k4a/k4a.h>
#include <k4abt.h>
#include "udp_sender.h"

using namespace std;

int main() {
    // UDP socket initialize.
    udp_sender sender{};
    sender.init_sock();

    k4a_device_t device = nullptr;
    if (K4A_FAILED(k4a_device_open(K4A_DEVICE_DEFAULT, &device))) {
        cout << "Failed to open k4a device!" << endl;
        return 1;
    }

    size_t serial_size = 0;
    k4a_device_get_serialnum(device, nullptr, &serial_size);
    char *serial = (char *) (malloc(serial_size));
    k4a_device_get_serialnum(device, serial, &serial_size);
    cout << "Opened device: " << serial << endl;
    free(serial);

    k4a_device_configuration_t config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
    config.camera_fps = K4A_FRAMES_PER_SECOND_30;
    config.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
    config.color_resolution = K4A_COLOR_RESOLUTION_OFF;

    // Start the camera with the given configuration
    if (K4A_FAILED(k4a_device_start_cameras(device, &config))) {
        cout << "Failed to start cameras!" << endl;
        k4a_device_close(device);
        return 1;
    }

    // Start body tracking !!!!!
    k4a_calibration_t calibration;
    k4a_device_get_calibration(device, config.depth_mode, config.color_resolution, &calibration);
    k4abt_tracker_t tracker = nullptr;
    k4abt_tracker_configuration_t tracker_config = K4ABT_TRACKER_CONFIG_DEFAULT;
    k4abt_tracker_create(&calibration, tracker_config, &tracker);

    for (int i = 0; i < 20000; i++) {
        k4a_capture_t capture;
        k4a_wait_result_t result = k4a_device_get_capture(device, &capture, K4A_WAIT_INFINITE);
        if (result == K4A_WAIT_RESULT_SUCCEEDED) {
            k4a_wait_result_t queue_result = k4abt_tracker_enqueue_capture(tracker, capture, 0);
            k4a_capture_release(capture);
            if (queue_result == K4A_WAIT_RESULT_TIMEOUT) {
                cout << "Error! Add capture to tracker process queue timeout!" << endl;
                break;
            } else if (queue_result == K4A_WAIT_RESULT_FAILED) {
                cout << "Error! Add capture to tracker process queue failed!" << endl;
                break;
            }

            k4abt_frame_t frame = nullptr;
            k4a_wait_result_t pop_result = k4abt_tracker_pop_result(tracker, &frame, 50);
            if (pop_result == K4A_WAIT_RESULT_SUCCEEDED) {
                size_t num_bodies = k4abt_frame_get_num_bodies(frame);
                cout << num_bodies << "bodies are detected !!" << endl;

                if (num_bodies > 0) {
                    k4abt_skeleton_t skeleton;
                    k4abt_frame_get_body_skeleton(frame, 0, &skeleton);
                    k4abt_frame_release(frame);

                    map<int, k4abt_joint_t> bones;
                    for (int id = 0; id < (int) K4ABT_JOINT_COUNT; ++id) {
                        k4abt_joint_t joint = skeleton.joints[(k4abt_joint_id_t) id];
                        bones[id] = joint;
                    }

                    // Send UDP data.
                    sender.send_data(bones);
                }
            } else if (pop_result == K4A_WAIT_RESULT_TIMEOUT) {
                cout << "Error! Pop body frame result timeout!" << endl;
                break;
            } else {
                cout << "Pop body frame result failed!" << endl;
                break;
            }
        } else if (result == K4A_WAIT_RESULT_TIMEOUT) {
            cout << "Error! Get depth frame time out!" << endl;
            break;
        } else {
            cout << "Get depth capture returned error: " << result << endl;
            break;
        }
    }

    // Shut down the camera when finished with application logic
    k4abt_tracker_shutdown(tracker);
    k4abt_tracker_destroy(tracker);
    k4a_device_stop_cameras(device);
    k4a_device_close(device);

    // close socket.
    sender.close_sock();

    return 0;
}
