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
#define LOG_TAG "RecursiveChown"
#include <log/log.h>

int main(int argc, const char** argv) {
  struct dirent *dp;
  DIR *dfd;
  const char *dir;
  char buf[PATH_MAX] = {0};

  if (argc != 2)
    return 1;

  dir = *++argv;
  ALOGI("Started! Directory: %s", dir);
  if ((dfd = opendir(dir)) == NULL) {
    ALOGE("Can't open %s: %s", dir, strerror(errno));
    return 1;
  }

  while ((dp = readdir(dfd)) != NULL) {
    struct dirent *dp_inner;
    DIR *dfd_inner;
    char buf_inner[PATH_MAX] = {0};

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
        if (errno == EACCES) {
          // Would mean no permission to setattr the whole folder
          char resolved_path[PATH_MAX];
          if (realpath(buf_inner, resolved_path) != NULL)
            ALOGI("Realpath of the file: %s", resolved_path);
          break;
        }
      }
    }
    closedir(dfd_inner);
  }
  closedir(dfd);
  ALOGI("Exiting!");
}
