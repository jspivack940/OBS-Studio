// SPDX-FileCopyrightText: 2022 tytan652 <tytan652@tytanium.xyz>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <util/base.h>

#include <va/va.h>

VADisplay vaapi_open_device(int *fd, const char *device_path,
			    const char *func_name);
void vaapi_close_device(int *fd, VADisplay dpy);

bool vaapi_device_rc_supported(VAProfile profile, VADisplay dpy, uint32_t rc,
			       const char *device_path);

bool vaapi_quality_preset_supported(VAProfile profile, VADisplay dpy);

bool vaapi_display_h264_supported(VADisplay dpy, const char *device_path);

bool vaapi_device_h264_supported(const char *device_path);

const char *vaapi_get_h264_default_device(void);
