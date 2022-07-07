#include "hdevice.h"

#ifdef _WIN32
#include <windows.h>
#include <dshow.h>



static std::vector<HDevice> getDevicesList(REFGUID category)
{
    std::vector<HDevice> ret;

    HRESULT hr = S_OK;

    if (SUCCEEDED(hr))
    {
        ICreateDevEnum *pDevEnum = NULL;
        HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
            CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum));
        if (SUCCEEDED(hr) && pDevEnum)
        {
            IEnumMoniker *pEnum = NULL;
            hr = pDevEnum->CreateClassEnumerator(category, &pEnum, 0);
            if (hr == S_FALSE || pEnum == NULL)
            {
                pDevEnum->Release();
                return ret;
            }
            IMoniker *pMoniker = NULL;
            while (pEnum->Next(1, &pMoniker, NULL) == S_OK && pMoniker)
            {
                IPropertyBag *pPropBag = NULL;
                HRESULT hr = pMoniker->BindToStorage(0, 0, IID_PPV_ARGS(&pPropBag));
                if (FAILED(hr) || pPropBag == NULL)
                {
                    pMoniker->Release();
                    continue;
                }
                VARIANT var;
                VariantInit(&var);
                hr = pPropBag->Read(L"Description", &var, 0);
                if (FAILED(hr))
                {
                    hr = pPropBag->Read(L"FriendlyName", &var, 0);
                }
                if (SUCCEEDED(hr)) {
                    HDevice dev;
                    sprintf(dev.name, "%S", var.bstrVal);
                    ret.push_back(dev);
                    VariantClear(&var);
                }
                pPropBag->Release();
                pMoniker->Release();
            }
            pEnum->Release();
            pDevEnum->Release();
        }
    }
    return ret;
}


std::vector<HDevice> getVideoDevices() {
    return getDevicesList(CLSID_VideoInputDeviceCategory);
}

std::vector<HDevice> getAudioDevices() {
    return getDevicesList(CLSID_AudioInputDeviceCategory);
}
#else
std::vector<HDevice> getVideoDevices() {
    return std::vector<HDevice>();
}

std::vector<HDevice> getAudioDevices() {
    return std::vector<HDevice>();
}
#endif



