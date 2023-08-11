/*
 * Copyright 2021 Soo Hwan Na "Royna"
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <thread>
#include <android-base/properties.h>
#include <fstream>
#include <iostream>
#include <string>

static void copy_kmsg(const std::string *logpath) {
  std::ifstream readfile("/proc/kmsg");
  std::ofstream writefile(*logpath + "/kmsg.txt");
  while (1) { writefile << readfile.rdbuf(); }
}
static void copy_logcat(const std::string *logpath) {
  std::string cmd("/system/bin/logcat -b all");
  cmd += " -f " + *logpath + "/logcat.txt";
  system(cmd.c_str());
}

using android::base::GetProperty;

int main(void) {
  std::string kLogDir = GetProperty("vendor.logger.log_storage", "/data/debug");
  std::thread kmsg(copy_kmsg, &kLogDir);
  std::thread logcat(copy_logcat, &kLogDir);
  kmsg.join(); // Block from exiting. since cp blocks too. 
	       // It is stopped when killed by init
  logcat.join();
  return 0;
}
