#include <errno.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>
#include <zephyr/shell/shell_string_conv.h>

#include <custom_drivers/led_sensor.h>

static const struct device *const led_sensor = DEVICE_DT_GET_ANY(zephyr_led_sensor);

static int check_ready(const struct shell *sh)
{
	if (!device_is_ready(led_sensor)) {
		shell_error(sh, "%s is not ready", led_sensor->name);
		return -ENODEV;
	}

	return 0;
}

static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv)
{
	int ret;

	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	ret = check_ready(sh);
	if (ret < 0) {
		return ret;
	}

	ret = sensor_sample_fetch(led_sensor);
	if (ret < 0) {
		shell_error(sh, "sensor_sample_fetch failed: %d", ret);
		return ret;
	}

	shell_print(sh, "Sample fetched");
	return 0;
}

static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv)
{
	struct sensor_value val;
	int ret;

	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	ret = check_ready(sh);
	if (ret < 0) {
		return ret;
	}

	ret = sensor_channel_get(led_sensor, SENSOR_CHAN_ALL, &val);
	if (ret < 0) {
		shell_error(sh, "sensor_channel_get failed: %d", ret);
		return ret;
	}

	shell_print(sh, "Value: %d (LED off)", val.val1);
	return 0;
}

static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	shell_print(sh, "Device: %s", led_sensor->name);
	shell_print(sh, "Ready:  %s", device_is_ready(led_sensor) ? "yes" : "no");
	return 0;
}

static int cmd_sensor_set(const struct shell *sh, size_t argc, char **argv)
{
	long value;
	int err = 0;
	int ret;

	if (argc != 2) {
		shell_error(sh, "Missing value, usage: sensor set <0|1>");
		return -EINVAL;
	}

	value = shell_strtol(argv[1], 10, &err);
	if (err != 0) {
		shell_error(sh, "Invalid value '%s', expected 0 or 1", argv[1]);
		return -EINVAL;
	}

	if (value < 0 || value > 1) {
		shell_error(sh, "Value %ld out of range, expected 0 or 1", value);
		return -ERANGE;
	}

	ret = check_ready(sh);
	if (ret < 0) {
		return ret;
	}

	ret = led_sensor_set_enabled(led_sensor, value == 1);
	if (ret < 0) {
		shell_error(sh, "led_sensor_set_enabled failed: %d", ret);
		return ret;
	}

	shell_print(sh, "LED sensor %s", value ? "enabled" : "disabled");
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
	SHELL_CMD(fetch, NULL, "Call sensor_sample_fetch() (LED on).", cmd_sensor_fetch),
	SHELL_CMD(read, NULL, "Call sensor_channel_get() and print the value (LED off).",
		  cmd_sensor_read),
	SHELL_CMD(info, NULL, "Print device name and ready state.", cmd_sensor_info),
	SHELL_CMD_ARG(set, NULL, "Enable (1) or disable (0) the LED: set <0|1>",
		      cmd_sensor_set, 2, 0),
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sub_sensor, "LED sensor commands", NULL);
