#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

static const struct device *const led_sensor = DEVICE_DT_GET_ANY(zephyr_led_sensor);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    if (!device_is_ready(led_sensor)) {
        LOG_ERR("%s is not ready", led_sensor->name);
        return 0;
    }

    LOG_INF("%s ready, use the 'sensor' shell command", led_sensor->name);
    return 0;
}
