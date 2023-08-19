// Shim for wfdservice

// libaudioclient
#include <android/media/IAudioPolicyService.h>
#include <media/AidlConversion.h>
#include <media/AudioSystem.h>
#include <media/DeviceDescriptorBase.h>
#include <media/IAudioFlinger.h>
#include <media/PolicyAidlConversion.h>
#include <media/TypeConverter.h>

// libaudio_system_headers
#include <system/audio.h>

// liblog
#include <log/log.h>

using android::aidl_utils::statusTFromBinderStatus;
using android::media::IAudioPolicyService;
using namespace android;

extern "C" status_t
_ZN7android11AudioSystem24setDeviceConnectionStateE15audio_devices_t24audio_policy_dev_state_tPKcS4_14audio_format_t(
        audio_devices_t device, audio_policy_dev_state_t state, const char* device_address,
        const char* device_name, audio_format_t encodedFormat) {
    const sp<IAudioPolicyService>& aps = AudioSystem::get_audio_policy_service();
    media::AudioPort aidlPort;
    DeviceDescriptorBase devDescr(device, device_address);
    status_t status;

    if (aps == 0) return PERMISSION_DENIED;

    devDescr.setName(device_name);
    status = devDescr.writeToParcelable(&aidlPort);

    if (status != OK) {
        ALOGE("Failed to convert to AudioPort Parcelable: %s", statusToString(status).c_str());
        return status;
    }
    return statusTFromBinderStatus(aps->setDeviceConnectionState(
            VALUE_OR_RETURN_STATUS(
                    legacy2aidl_audio_policy_dev_state_t_AudioPolicyDeviceState(state)),
            aidlPort.hal,
            VALUE_OR_RETURN_STATUS(
                    legacy2aidl_audio_format_t_AudioFormatDescription(encodedFormat))));
}
