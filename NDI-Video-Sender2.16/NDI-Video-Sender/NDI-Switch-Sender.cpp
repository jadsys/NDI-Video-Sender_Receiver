#include "VideoSourceManager.h"
#include "NDISender.h"
#include <chrono>
#define CAMERA_NUM 6
#define SENDER_NUM 3
int main(void) {
    VideoSourceManager* vsm = new VideoSourceManager();
    NDISender* sender[SENDER_NUM];
    for (int i = 1; i <= CAMERA_NUM; i++) {
        vsm->addVideoSource(i);
    }
    for (int i = 1; i <= SENDER_NUM; i++) {
        cout << i << endl;
        vsm->registSender(i);
        cout << "------------" << endl;
        sender[i] = new NDISender(vsm, i);
    }

    while (true) {
        chrono::seconds dura(10);
        this_thread::sleep_for(dura);
    }

    for (int i = 0; i < SENDER_NUM; i++) {
        delete sender[i];
    }
    delete vsm;
    return 0;
}
