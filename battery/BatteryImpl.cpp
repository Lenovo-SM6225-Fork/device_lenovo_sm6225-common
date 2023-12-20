#define LOG_TAG "BatteryHAL"

#include <log/log.h>

#include <battery.h>

#include <fstream>
#include <string>

static const char kChargeCtlStartSysfs[] =
    "/sys/class/power_supply/battery/StartCharging_Test";
static const char kChargeCtlStopSysfs[] =
    "/sys/class/power_supply/battery/StopCharging_Test";

extern "C" {

void setChargable(const bool enable) {
  std::ifstream ifs(enable ? kChargeCtlStartSysfs : kChargeCtlStopSysfs);
  std::string dummy;
  if (ifs)
    std::getline(ifs, dummy);
  else
    ALOGW("Failed to open sysfs: Enabled %d", enable);
}


}
