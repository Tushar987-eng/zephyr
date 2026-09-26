#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include <our_driver.h>

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

struct our_driver_config {
    struct gpio_dt_spec led;
};

struct our_driver_data {
    int cnt;
    bool on;
};

int our_driver_extension_api(const struct device *dev,int value)
{
    if(dev == NULL)
        return -EINVAL;
    struct our_driver_data *data = dev->data;

    data->cnt = value;

    return data->cnt;
}

static int our_sensor_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
    LOG_INF("Hello! We are trying to do sensor_sample_fetch with channel : %d, this will move the LED to ON",chan);
    const struct our_driver_config *cfg = dev->config;
    struct our_driver_data *data = dev->data;
    int ret = gpio_pin_set_dt(&cfg->led, 1);
    if(ret == 0)
        data->on = true;
    return ret;
}

static int our_sensor_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val)
{
    if (val == NULL) {
        return -EINVAL;
    }
    LOG_INF("Hello! We are trying to do sensor_channel_get with channel : %d, this will move the LED to OFF",chan);
    const struct our_driver_config *cfg = dev->config;
    struct our_driver_data *data = dev->data;
    val->val1 = data->on;
    val->val2 = 0;
    int ret = gpio_pin_set_dt(&cfg->led, 0);
    if(ret == 0)
        data->on = false;
    return ret;
}

static DEVICE_API(sensor, custom_sensor_api) = {
    .sample_fetch = our_sensor_sample_fetch,
	.channel_get = our_sensor_channel_get,
};

static int init(const struct device* dev)
{
    const struct our_driver_config *cfg = dev->config;
    struct our_driver_data *data = dev->data;

    data->cnt = 0;

    if (!gpio_is_ready_dt(&cfg->led)) 
        return -ENODEV;

    if (gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE) < 0) 
        return -1;

    LOG_INF("Driver is Initialized!!!!!\n");
    return 0;
}

#define OUR_DRIVER_DEFINE(inst)                                 \
    static struct our_driver_data data_##inst;                  \
    static const struct our_driver_config cfg_##inst = {        \
        .led = GPIO_DT_SPEC_INST_GET(inst , gpios),             \
    };                                                          \
    SENSOR_DEVICE_DT_INST_DEFINE(inst, init, NULL,              \
        &data_##inst, &cfg_##inst, POST_KERNEL,                 \
        CONFIG_SENSOR_INIT_PRIORITY, &custom_sensor_api);       \

DT_INST_FOREACH_STATUS_OKAY(OUR_DRIVER_DEFINE)
