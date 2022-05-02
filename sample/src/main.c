/*
 * Copyright (c) 2022 XiNGRZ
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <device.h>
#include <drivers/led.h>
#include <logging/log.h>
#include <zephyr.h>

LOG_MODULE_REGISTER(main);

static const struct device *backlight = DEVICE_DT_GET(DT_ALIAS(lcd_backlight));

void main(void)
{
	int brightness = 0;
	int rc;

	if (device_is_ready(backlight)) {
		LOG_INF("Found LED device %s", backlight->name);
	} else {
		LOG_ERR("LED device %s is not ready", backlight->name);
		return;
	}

	while (1) {
		rc = led_set_brightness(backlight, 0, brightness);
		if (rc) {
			LOG_ERR("Couldn't update brightness: %d", rc);
		}

		brightness++;
		if (brightness >= 16) {
			brightness = 0;
		}

		k_sleep(K_MSEC(1000));
	}
}
