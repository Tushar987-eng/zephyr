#ifndef EXTENSION_HEADER_H_
#define EXTENSION_HEADER_H_
#include <zephyr/device.h>
#ifdef __cplusplus
extern "C" {
#endif

int our_driver_extension_api(const struct device *dev, int val);

#ifdef __cplusplus
}
#endif
#endif