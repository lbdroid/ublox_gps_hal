###############################################################################
#
# Copyright (C) u-blox AG 
# u-blox AG, Thalwil, Switzerland
# 
# All rights reserved.
# 
# Permission to use, copy, modify, and distribute this software for any
# purpose without fee is hereby granted, provided that this entire notice
# is included in all copies of any software which is or includes a copy
# or modification of this software and in all copies of the supporting
# documentation for such software.
# 
# THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
# WARRANTY. IN PARTICULAR, NEITHER THE AUTHOR NOR U-BLOX MAKES ANY
# REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
# OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.
#
###############################################################################
#
# Project: PE_AND
#
###############################################################################

# version 2.2 does not yet support SUPL/AGPS/RIL/NI interfaces
ifneq ($(PLATFORM_SDK_VERSION),8)
SUPL_ENABLED := 1
endif

LOCAL_PATH := $(call my-dir)
TEMP_PATH:=$(LOCAL_PATH)
include $(LOCAL_PATH)/supl/asn1c_header/Android.mk
LOCAL_PATH:=$(TEMP_PATH)

include $(CLEAR_VARS)

LOCAL_SHARED_LIBRARIES := \
	liblog \
	libcutils

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/parser

PARSER_SRC_FILES := \
	parser/database.cpp \
	parser/parserbuffer.cpp \
	parser/protocolnmea.cpp \
	parser/protocolubx.cpp \
	parser/protocolunknown.cpp \
	parser/stdafx.cpp

LOCAL_SRC_FILES := \
	$(PARSER_SRC_FILES) \
	ubx_moduleIf.cpp \
	ubx_serial.cpp \
	ubx_udpServer.cpp \
	ubx_localDb.cpp \
	ubx_timer.cpp \
	ubx_xtraIf.cpp \
	ubx_cfg.cpp \
	ubx_log.cpp \
	ubxgpsstate.cpp \
	gps_thread.cpp

LOCAL_CFLAGS := \
	-DPLATFORM_SDK_VERSION=$(PLATFORM_SDK_VERSION) \
	-DUNIX_API \
	-DANDROID_BUILD \
	-DUDP_SERVER_PORT=46434

# Additions for SUPL
ifeq ($(SUPL_ENABLED),1)
LOCAL_C_INCLUDES += external/openssl/include/
LOCAL_C_INCLUDES += external/openssl/
LOCAL_C_INCLUDES += external/

LOCAL_SHARED_LIBRARIES += libcrypto
LOCAL_SHARED_LIBRARIES += libssl
LOCAL_STATIC_LIBRARIES += libSupl

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/supl \
	$(LOCAL_PATH)/supl/asn1c_header

SUPL_SOURCE_FILES := \
	supl/rrlpmanager.cpp \
	supl/suplSMmanager.cpp \
	supl/upldecod.cpp \
	supl/uplsend.cpp \
	supl/rrlpdecod.cpp \
	ubx_rilIf.cpp \
	ubx_niIf.cpp \
	ubx_agpsIf.cpp

LOCAL_SRC_FILES += $(SUPL_SOURCE_FILES)
LOCAL_CFLAGS += -DSUPL_ENABLED
# LOCAL_CFLAGS += -UNDEBUG

# Uncomment the line below for test with SUPL Test suite
#LOCAL_CFLAGS += -DSUPL_FQDN_SLP='"slp.rs.de"'

endif

LOCAL_MODULE := gps.default

LOCAL_MODULE_TAGS := optional

LOCAL_PRELINK_MODULE := false
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := gps.conf
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := $(LOCAL_MODULE)
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := u-blox.conf
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES := $(LOCAL_MODULE)
include $(BUILD_PREBUILT)

# STFU
# FIXME: do it properly.. how?!
ifeq ($(SUPL_ENABLED),1)
NOTICE-TARGET-STATIC_LIBRARIES-libSupl:
	@echo shut up
endif
