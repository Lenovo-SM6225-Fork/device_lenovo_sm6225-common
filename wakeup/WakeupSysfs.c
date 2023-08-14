#include <dirent.h>
#include <errno.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Android
#if __has_include(<log/log.h>)
#define LOG_TAG "WakeupTool"
#include <log/log.h>
#else
#define ALOGI(fmt, ...) printf(fmt "\n", ##__VA_ARGS__);
#define ALOGD ALOGI
#define ALOGE ALOGI
#endif

// TODO: Make it take parameters and remove hardcode
int main(void) {
  struct dirent *dp;
  DIR *dfd;
  const char *dir = "/sys/class/wakeup";
  char buf[PATH_MAX];

  ALOGI("Started!");
  if ((dfd = opendir(dir)) == NULL) {
    ALOGE("Can't open %s: %s", dir, strerror(errno));
    return 1;
  }

  while ((dp = readdir(dfd)) != NULL) {
    struct dirent *dp_inner;
    DIR *dfd_inner;
    char buf_inner[PATH_MAX];

    snprintf(buf, sizeof(buf), "%s/%s", dir, dp->d_name);
    // We are on e.g. /sys/class/wakeup13
    // Iterate its subdirs
    if ((dfd_inner = opendir(buf)) == NULL) {
      ALOGE("Can't open %s: %s", buf, strerror(errno));
      // Not the main sysfs node anyway
      continue;
    }
    while ((dp_inner = readdir(dfd_inner)) != NULL) {
      if (dp_inner->d_type != DT_REG)
        continue;
      if (!strcmp(dp_inner->d_name, "uevent"))
        continue;
      snprintf(buf_inner, sizeof(buf_inner), "%s/%s", buf, dp_inner->d_name);
      // We are on e.g. /sys/class/wakeup13/name
      if (chown(buf_inner, 1000, 1000) == -1) {
        ALOGE("chown() failed of %s: %s", buf_inner, strerror(errno));
        if (errno == EPERM) {
          char resolved_path[PATH_MAX];
          if (realpath(buf_inner, resolved_path) == 0)
            ALOGI("Realpath of the file: %s", resolved_path);
        }
      }
    }
  }
  ALOGI("Exiting!");
}
