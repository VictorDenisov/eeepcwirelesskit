#include <errno.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <libusb.h>

#define INTERFACE 1
#define CTRL_IN			(LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_ENDPOINT_IN | INTERFACE)
#define USB_RQ			0x01

static struct libusb_device_handle *devh = NULL;

static void print_devs(libusb_device **devs)
{
	libusb_device *dev;
	int i = 0;

	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
			fprintf(stderr, "failed to get device descriptor");
			return;
		}

		printf("%04x:%04x (bus %d, device %d)\n",
			desc.idVendor, desc.idProduct,
			libusb_get_bus_number(dev), libusb_get_device_address(dev));
        if (desc.idVendor == 0x04f2 && desc.idProduct == 0x0860) {
            printf("My device\n");
            r = libusb_open(dev, &devh);
            if (r == 0) {
                printf("Success on device open\n");
            } else {
                printf("Unsuccessful device open\n");
                if (r == LIBUSB_ERROR_NO_MEM) {
                    printf("LIBUSB_ERROR_NO_MEM");
                }
                if (r == LIBUSB_ERROR_ACCESS) {
                    printf("LIBUSB_ERROR_ACCESS");
                }
                if (r == LIBUSB_ERROR_NO_DEVICE) {
                    printf("LIBUSB_ERROR_NO_DEVICE");
                }
            }
        }
	}
}

static void find_device(void)
{
	libusb_device **devs;
    int cnt = libusb_get_device_list(NULL, &devs);
    if (cnt == 0) {
		fprintf(stderr, "no devices\n");
		exit(1);
    }
	print_devs(devs);
}

int main(int argc, char* argv[]) {
    int r = 1;
    r = libusb_init(NULL);
    if (r < 0) {
		fprintf(stderr, "failed to initialise libusb\n");
		exit(1);
    }

    find_device();
    //return 0;
	/*devh = libusb_open_device_with_vid_pid(NULL, 0x04f2, 0x0860);
	if (devh == NULL) {
		fprintf(stderr, "Could not find/open device\n");
        r = -1;
		goto out;
	}*/

	r = libusb_claim_interface(devh, 1);
	if (r < 0) {
		fprintf(stderr, "usb_claim_interface error %d\n", r);
            if (r == LIBUSB_ERROR_NOT_FOUND) {
                printf("LIBUSB_ERROR_NOT_FOUND\n");
            }
            if (r == LIBUSB_ERROR_BUSY) {
                printf("LIBUSB_ERROR_BUSY\n");
            }
            if (r == LIBUSB_ERROR_NO_DEVICE) {
                printf("LIBUSB_ERROR_NO_DEVICE\n");
            }
		goto out;
	}
	printf("claimed interface\n");

    unsigned char data[0x10];
	r = libusb_control_transfer(devh, CTRL_IN, USB_RQ, 0x0305, 0x0001, data, sizeof(data), 0);

    int i;
	printf("data size = %d: ", r);
	for (i = 0; i < r; i++) {
		printf("%02x ", data[i]);
    }
	printf("\n");

	r = libusb_control_transfer(devh, CTRL_IN, USB_RQ, 0x0306, 0x0001, data, sizeof(data), 0);

    unsigned int v = r;
	printf("data size = %d: ", v);
	for (i = 0; i < r; i++) {
		printf("%02x ", data[i]);
    }
	printf("\n");

	libusb_release_interface(devh, 0);
out:
	libusb_close(devh);
	libusb_exit(NULL);
	return r >= 0 ? r : -r;
}
