/*
 * Copyright (c) 2013 Dominic Spill
 *
 * This file is part of Unambiguous Encapsulation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef FREEBSD
#include <libusb.h>
#else
#include <libusb-1.0/libusb.h>
#endif

#include <stdio.h>
#define UE_VID 0x1D50
#define UE_PID 0x605A

libusb_device_handle *open_ue_device()
{
	struct libusb_context *ctx = NULL;
	struct libusb_device **usb_list = NULL;
	struct libusb_device_handle *devh = NULL;
	struct libusb_device_descriptor desc;
	int usb_devs, i, r;

	usb_devs = libusb_get_device_list(ctx, &usb_list);
	for(i = 0 ; i < usb_devs ; ++i) {
		r = libusb_get_device_descriptor(usb_list[i], &desc);
		if(r < 0)
			fprintf(stderr, "couldn't get usb descriptor for dev #%d!\n", i);
		if (desc.idVendor == UE_VID && desc.idProduct == UE_PID)
		{
			r = libusb_open(usb_list[i], &devh);
			if (r)
				libusb_error_name(r);
			else
				return devh;
		}
	}
    return NULL;
}

void vend_req(libusb_device_handle *devh, int request, int value)
{
	int ret;
	ret = libusb_control_transfer(devh,
		LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE,
		request, value, 0, NULL, 0, 2000);
	if(ret < 0)
		printf("* Error sending vendor request\n");
	
}

int main(int argc, char **argv)
{
	libusb_device_handle *devh;
	unsigned char readback;
	int ret, transferred;

	ret = libusb_init(NULL);
	if (ret < 0) {
		fprintf(stderr, "libusb_init failed (got 1.0?)\n");
		return 1;
	}
	libusb_set_debug(NULL, 3);

	devh = open_ue_device();
//	vend_req(devh, 1, 0xf0f0f0f0);
	vend_req(devh, 1, 0x5555);

	ret = libusb_bulk_transfer(devh, 0x81, &readback, 1, &transferred, 0);
	if (ret < 0){
		printf("* Couldn't read: %s\n", libusb_error_name(ret));
	/* Ignore case where we have an incomplete state */
	} else if (transferred >= 1) {
		printf("%d ", readback);
		printf("\n");
	}
	fflush(NULL);
}
