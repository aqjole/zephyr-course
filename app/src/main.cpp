#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#define SLEEP_TIME_MS CONFIG_APP_HEARTBEAT_PERIOD_MS

static const struct device *const led_sensor = DEVICE_DT_GET_ANY(zephyr_led_sensor);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    struct sensor_value val;

    if (!device_is_ready(led_sensor)) return 0;

    while (1) {
        if (sensor_sample_fetch(led_sensor) < 0) return 0;
        LOG_INF("LED state: ON");
        k_msleep(SLEEP_TIME_MS);

        if (sensor_channel_get(led_sensor, SENSOR_CHAN_ALL, &val) < 0) return 0;
        LOG_INF("LED state: OFF (was %d)", val.val1);
#ifdef CONFIG_APP_LED_DEBUG
        LOG_INF("LED debug: period=%d ms, brightness=%d, fade=%d ms", SLEEP_TIME_MS,
                CONFIG_APP_LED_BRIGHTNESS, CONFIG_APP_LED_FADE_DURATION_MS);
#endif
        k_msleep(SLEEP_TIME_MS);
    }
    return 0;
}
