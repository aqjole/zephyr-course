#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <custom_drivers/led_sensor.h>

#define SLEEP_TIME_MS CONFIG_APP_HEARTBEAT_PERIOD_MS
#define TOGGLE_EVERY  5

static const struct device *const led_sensor = DEVICE_DT_GET_ANY(zephyr_led_sensor);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    struct sensor_value val;
    bool enabled = true;
    int cycle = 0;

    if (!device_is_ready(led_sensor)) return 0;

    while (1) {
        if (++cycle % TOGGLE_EVERY == 0) {
            enabled = !enabled;
            if (led_sensor_set_enabled(led_sensor, enabled) < 0) return 0;
            LOG_INF("LED sensor %s", enabled ? "enabled" : "disabled");
        }

        if (sensor_sample_fetch(led_sensor) < 0) return 0;
        k_msleep(SLEEP_TIME_MS);

        if (sensor_channel_get(led_sensor, SENSOR_CHAN_ALL, &val) < 0) return 0;
        LOG_INF("LED was %s", val.val1 ? "ON" : "OFF");
#ifdef CONFIG_APP_LED_DEBUG
        LOG_INF("LED debug: period=%d ms, brightness=%d, fade=%d ms", SLEEP_TIME_MS,
                CONFIG_APP_LED_BRIGHTNESS, CONFIG_APP_LED_FADE_DURATION_MS);
#endif
        k_msleep(SLEEP_TIME_MS);
    }
    return 0;
}
