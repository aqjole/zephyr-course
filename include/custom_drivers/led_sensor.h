#ifndef CUSTOM_DRIVERS_LED_SENSOR_H_
#define CUSTOM_DRIVERS_LED_SENSOR_H_

#include <stdbool.h>
#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

int led_sensor_set_enabled(const struct device *dev, bool enabled);

#ifdef __cplusplus
}
#endif

#endif
