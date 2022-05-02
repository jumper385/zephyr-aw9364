/*
 * Copyright (c) 2022 XiNGRZ
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT awinic_aw9364

#include <drivers/gpio.h>
#include <drivers/led.h>
#include <logging/log.h>
#include <stdbool.h>
#include <zephyr.h>

LOG_MODULE_REGISTER(aw9364);

#define LEVEL_OFF 0

struct aw9364_data {
	uint8_t level;
};

struct aw9364_config {
	struct gpio_dt_spec output_gpio;
	uint8_t steps;
};

static int aw9364_set_brightness(const struct device *dev, uint32_t led, uint8_t value)
{
	const struct aw9364_config *config = dev->config;
	struct aw9364_data *data = dev->data;
	int ret;

	if (value > config->steps) {
		return -EINVAL;
	}

	if (value == LEVEL_OFF) {
		ret = gpio_pin_set_dt(&config->output_gpio, 0);
		k_msleep(3);
		return ret;
	} else {
		if (data->level == LEVEL_OFF) {
			if ((ret = gpio_pin_set_dt(&config->output_gpio, 1)) != 0) {
				return ret;
			}
			data->level = config->steps;
			k_usleep(30);
		}
		int pul_from = config->steps - data->level;
		int pul_to = config->steps - value;
		int pul_num = (config->steps + pul_to - pul_from) % config->steps;
		for (int i = 0; i < pul_num; i++) {
			if ((ret = gpio_pin_set_dt(&config->output_gpio, 0)) != 0) {
				return ret;
			}
			if ((ret = gpio_pin_set_dt(&config->output_gpio, 1)) != 0) {
				return ret;
			}
		}
	}
	data->level = value;

	return 0;
}

static int aw9364_on(const struct device *dev, uint32_t led)
{
	const struct aw9364_config *config = dev->config;
	struct aw9364_data *data = dev->data;
	if (data->level == LEVEL_OFF) {
		return aw9364_set_brightness(dev, led, config->steps);
	} else {
		return 0;
	}
}

static int aw9364_off(const struct device *dev, uint32_t led)
{
	struct aw9364_data *data = dev->data;
	if (data->level != LEVEL_OFF) {
		return aw9364_set_brightness(dev, led, LEVEL_OFF);
	} else {
		return 0;
	}
}

static int aw9364_init(const struct device *dev)
{
	const struct aw9364_config *config = dev->config;
	struct aw9364_data *data = dev->data;
	int ret;

	if (!device_is_ready(config->output_gpio.port)) {
		LOG_ERR("Output GPIO not ready");
		return -ENODEV;
	}

	ret = gpio_pin_configure_dt(&config->output_gpio, GPIO_OUTPUT);
	if (ret < 0) {
		LOG_ERR("Could not configure output GPIO (%d)", ret);
		return ret;
	}

	data->level = LEVEL_OFF;

	return 0;
}

static struct aw9364_data aw9364_led_data;

static const struct led_driver_api aw9364_led_api = {
	.on = aw9364_on,
	.off = aw9364_off,
	.set_brightness = aw9364_set_brightness,
};

#define AW9364_DEVICE(i)                                                                           \
	static struct aw9364_data aw9364_data_##i;                                                 \
                                                                                                   \
	static const struct aw9364_config aw9364_config_##i = {                                    \
		.output_gpio = GPIO_DT_SPEC_INST_GET(i, output_gpios),                             \
		.steps = DT_INST_PROP_OR(i, steps, 16),                                            \
	};                                                                                         \
                                                                                                   \
	DEVICE_DT_INST_DEFINE(i, &aw9364_init, NULL, &aw9364_data_##i, &aw9364_config_##i,         \
			      POST_KERNEL, CONFIG_LED_INIT_PRIORITY, &aw9364_led_api);

DT_INST_FOREACH_STATUS_OKAY(AW9364_DEVICE)
