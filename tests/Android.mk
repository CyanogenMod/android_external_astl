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

# define the ASTL_TESTS environment variable to build the test programs
ifdef ASTL_TESTS

LOCAL_PATH := $(call my-dir)

# used to define a simple test program and build it as a standalone
# device executable.
#
# you can use EXTRA_CFLAGS to indicate additional CFLAGS to use
# in the build. the variable will be cleaned on exit
#
define device-test
  $(foreach file,$(1), \
    $(eval include $(CLEAR_VARS)) \
    $(eval LOCAL_SRC_FILES := $(file)) \
    $(eval LOCAL_MODULE := $(notdir $(file:%.cpp=%))) \
    $(eval $(info LOCAL_MODULE=$(LOCAL_MODULE))) \
    $(eval LOCAL_CFLAGS += $(EXTRA_CFLAGS)) \
    $(eval LOCAL_MODULE_TAGS := tests) \
    $(eval LOCAL_STATIC_LIBRARIES := libastl) \
    $(eval include $(BUILD_EXECUTABLE)) \
  ) \
  $(eval EXTRA_CFLAGS :=)
endef

# same as 'device-test' but builds a host executable instead
# you can use EXTRA_LDLIBS to indicate additional linker flags
#
define host-test
  $(foreach file,$(1), \
    $(eval include $(CLEAR_VARS)) \
    $(eval LOCAL_SRC_FILES := $(file)) \
    $(eval LOCAL_MODULE := $(notdir $(file:%.cpp=%))) \
    $(eval $(info LOCAL_MODULE=$(LOCAL_MODULE) file=$(file))) \
    $(eval LOCAL_CFLAGS += $(EXTRA_CFLAGS))  \
    $(eval LOCAL_LDLIBS += $(EXTRA_LDLIBS)) \
    $(eval LOCAL_MODULE_TAGS := eng tests) \
    $(eval LOCAL_STATIC_LIBRARIES := libastl) \
    $(eval include $(BUILD_HOST_EXECUTABLE)) \
  ) \
  $(eval EXTRA_CFLAGS :=) \
  $(eval EXTRA_LDLIBS :=)
endef

sources := \
    test_algorithm.cpp \
    test_string.cpp \
    test_type_traits.cpp \
	test_uninitialized.cpp \
    test_vector.cpp

# Disable all optimization for the host target to help test tools (valgrind...)
EXTRA_CFLAGS := -I bionic/libstdc++/include -I external/astl/include -g -O0
$(call host-test, $(sources))

EXTRA_CFLAGS := -I bionic/libstdc++/include -I external/astl/include
$(call device-test, $(sources))

endif  #ASTL_TESTS
