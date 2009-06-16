# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Build control file for Bionic's test programs
# define the BIONIC_TESTS environment variable to build the test programs
#

LOCAL_PATH := $(call my-dir)

astl_common_src_files := \
    string.cpp

# Build the target lib
include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(astl_common_src_files)

LOCAL_C_INCLUDES := external/astl/include

LOCAL_CFLAGS += -I bionic/libstdc++/include -I external/astl/include

LOCAL_SYSTEM_SHARED_LIBRARIES := libc libstdc++ libutils

LOCAL_MODULE:= libastl

include $(BUILD_STATIC_LIBRARY)

# Define the ASTL_TESTS environment variable to build the host lib
# needed for testing under valgrind.
# This is done automatically if you use: runtest astl

ifdef ASTL_TESTS

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(astl_common_src_files)

LOCAL_C_INCLUDES := external/astl/include

LOCAL_CFLAGS += -I bionic/libstdc++/include -I external/astl/include

LOCAL_SYSTEM_SHARED_LIBRARIES := libc libstdc++ libutils

LOCAL_MODULE:= libastl

include $(BUILD_HOST_STATIC_LIBRARY)

endif  #ASTL_TESTS
