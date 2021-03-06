#include "pch.h"
#include <stdio.h>
#include "libusb.h"

typedef int libusb_hotplug_callback_handle;

static void print_devs(libusb_device **devs)
{
	libusb_device *dev;
	libusb_device *dev_parent;
	libusb_device_handle **handle;
	int i = 0, j = 0, result, *cfg;
	uint8_t path[8];

	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		struct libusb_interface_descriptor interface_desc;

		int r = libusb_get_device_descriptor(dev, &desc);
		
		if (r < 0) {
			printf(stderr, "failed to get device descriptor");
			return;
		}

		printf("%04x:%04x (bus %d, device addr %d) USB Type: %04x Device Speed: %i ",
			desc.idVendor, desc.idProduct,
			libusb_get_bus_number(dev), libusb_get_device_address(dev), desc.bcdUSB),
			libusb_get_device_speed(dev);

		r = libusb_get_port_numbers(dev, path, sizeof(path));
		if (r > 0) {
			printf(" path: %d", path[0]);
			for (j = 1; j < r; j++)
				printf(".%d", path[j]);
		}
		printf("\n");

		//dev_parent = libusb_get_parent(dev);
		

		result = libusb_open(dev, &handle);
		if (result == 0) {
			printf("handle: %i \n", result);
				result = libusb_get_configuration(handle, &cfg);
				if (result == 0) {
					printf("config %i \n", cfg);
				}
		}
	
	}

	
}

int main(void)
{
	libusb_device **devs;
	int r;
	ssize_t cnt;

	r = libusb_init(NULL);
	if (r < 0)
		return r;

	cnt = libusb_get_device_list(NULL, &devs);
	printf("Number of devices found = %d \n", cnt);
	if (cnt < 0)
		return (int)cnt;
	
	print_devs(devs);
	libusb_free_device_list(devs, 1);


	libusb_exit(NULL);
	return 0;
}
