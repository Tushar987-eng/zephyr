#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>
#include <our_driver.h>

static int cmd_fetch_handler(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = shell_device_get_binding(argv[1]);
    if(!dev)
    {
        shell_error(sh, "Could not find device %s",argv[1]);
        return -EFAULT;
    }
    int ret = sensor_sample_fetch(dev);
    if(ret!=0)
    {
        shell_error(sh, "Could not fetch channel, got %d",ret);
        return ret;
    }
    else
    {
        shell_info(sh, "Fetch channel successful, (LED ON)");
    }
    return 0;
}

static int cmd_read_handler(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = shell_device_get_binding(argv[1]);
    if(!dev)
    {
        shell_error(sh, "Could not find device %s",argv[1]);
        return -EFAULT;
    }
    struct sensor_value val;
    int ret = sensor_channel_get(dev,SENSOR_CHAN_ALL, &val);
    if(ret!=0)
    {
        shell_error(sh, "Could not get channel, got %d",ret);
        return ret;
    }
    else
    {
        shell_info(sh, "Get channel successful, (LED OFF), was ->%d", val.val1);
    }
    return 0;
}

static int cmd_info_handler(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    const struct device *dev = shell_device_get_binding(argv[1]);
    if(!dev)
    {
        shell_error(sh, "Could not find device %s",argv[1]);
        return -EFAULT;
    }
    shell_print(sh, "name: %s, ready: %s", dev->name,
                device_is_ready(dev) ? "yes" : "no");
    return 0;
}

static int cmd_update_handler(const struct shell *sh, size_t argc, char **argv)
{
    int err=0;
    ARG_UNUSED(argc);
    const struct device *dev = shell_device_get_binding(argv[1]);
    if(!dev)
    {
        shell_error(sh, "Could not find device %s",argv[1]);
        return -EFAULT;
    }
    int value = shell_strtol(argv[2], 10, &err);
    if (err) 
    { 
        shell_error(sh, "'%s' is not a number", argv[2]); 
        return -EINVAL; 
    }
    if(value<0 || value>100)
    {
        shell_error(sh, "Value beyond supported range (0-100) %d",value);
        return -EINVAL;
    }

    int ret = our_driver_extension_api(dev, value);
    if (ret < 0) 
    { 
        shell_error(sh, "set failed: %d", ret); 
        return ret; 
    }
    shell_print(sh, "counter set to %d", ret);
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sensor_subcmd,
    SHELL_CMD_ARG(fetch, NULL, " Fetch command ", cmd_fetch_handler, 2, 0),
    SHELL_CMD_ARG(read, NULL, " Read command ", cmd_read_handler, 2, 0),
    SHELL_CMD_ARG(info, NULL, " Info command ", cmd_info_handler, 2, 0),
    SHELL_CMD_ARG(update, NULL, "Update command ", cmd_update_handler, 3, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensorroot, &sensor_subcmd, "Sensor Shell commands", NULL);