###############################################################################
#
# Copyright (C) u-blox ag
# u-blox ag, Thalwil, Switzerland
#
# All rights reserved.
#
# This  source  file  is  the  sole  property  of  u-blox  AG. Reproduction or
# utilization of this source in whole or part is forbidden without the written 
# consent of u-blox AG.
#
###############################################################################
#
# Project: PE_AND
#
###############################################################################

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/asn1.mk

#LOCAL_SHARED_LIBRARIES := \
#	liblog \
#	libcutils

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH) 

LOCAL_SRC_FILES := \
	$(ASN_MODULE_SOURCES)

LOCAL_MODULE := libSupl
#LOCAL_MODULE := gps.default

LOCAL_MODULE_TAGS := eng

LOCAL_CFLAGS :=

LOCAL_PRELINK_MODULE := false
#LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/

#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)
