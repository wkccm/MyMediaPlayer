#ifndef HDEVICE_H
#define HDEVICE_H

#undef _COMBASEAPI_H_

#include <vector>

struct HDevice
{
    char name[256];
};

std::vector<HDevice> getVideoDevices();
std::vector<HDevice> getAudioDevices();

#endif // HDEVICE_H
