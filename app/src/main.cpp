#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include <our_driver.h>

#define SLEEP_TIME_MS CONFIG_APP_HEARTBEAT_PERIOD_MS

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(app_led)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

namespace {
    void test() {
        const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
        struct sensor_value val;
        auto ret = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
        LOG_INF("Channle ret : %d\n",ret); 
    }
}

int main(void)
{
    const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
    struct sensor_value val;
    if(!device_is_ready(driver))
    {
        LOG_INF("Driver is not ready to function");
        return 0;
    }
    // while(1) {
    //     auto ret = sensor_sample_fetch(driver);
    //     LOG_INF("fetch -> %d (LED ON)", ret);
    //     k_msleep(SLEEP_TIME_MS);
    //     ret = sensor_channel_get(driver, SENSOR_CHAN_ALL, &val);
    //     LOG_INF("get -> %d, was_on=%d (LED OFF)", ret, val.val1);
    //     k_msleep(SLEEP_TIME_MS);
    //     LOG_INF("You have exercised the extension API, now counter is : %d",our_driver_extension_api(driver));
    // }

    // test();
    // bool led_state = true;

    // if (!gpio_is_ready_dt(&led)) return 0;

    // if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    // while (1) {
    //     if (gpio_pin_toggle_dt(&led) < 0) return 0;

    //     led_state = !led_state;
    //     LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
    //     k_msleep(SLEEP_TIME_MS);
    // }
    // return 0;
}
