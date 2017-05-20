/*
 * main.cpp
 *
 *  Created on: 20.05.2017
 *      Author: mik
 */

#include <iostream>
#include <libusb-1.0/libusb.h>

using namespace std;

void printUsbDevice(libusb_device * usbDevice);

int main() {

  enum {
    NO_ERROR = 0,
    INITIALIZATION_ERROR = 1
  };

  libusb_context * sessionContext = NULL;
  libusb_device ** devicesOnBus = NULL;

  int result = libusb_init(&sessionContext);
  if(result < 0) {
    cout << "Initialization error " << result << endl;
    return INITIALIZATION_ERROR;
  }

  const int VERBOSITY_LEVEL = 3;
  libusb_set_debug(sessionContext, VERBOSITY_LEVEL);
  ssize_t deviceCounter = libusb_get_device_list(sessionContext, &devicesOnBus);
  if(deviceCounter < 0) {
    cout << "Get device error" << endl;
  }
  cout << "Found " << deviceCounter << " devices in list." << endl;

  for(ssize_t i = 0; i < deviceCounter; i++) {
    printUsbDevice(devicesOnBus[i]);
  }
  libusb_free_device_list(devicesOnBus, 1);
  libusb_exit(sessionContext);

  return NO_ERROR;
}

void printUsbDevice(libusb_device * usbDevice) {
  libusb_device_descriptor deviceDescriptor;
  int result = libusb_get_device_descriptor(usbDevice, &deviceDescriptor);
  if (result < 0) {
    cout << "Failed to get device descriptor" << endl;
    return;
  }
  cout << "Number of possible configurations: "
      << (int)deviceDescriptor.bNumConfigurations << "\t" ;
  cout << "Device Class: " << (int)deviceDescriptor.bDeviceClass << "\t";
  cout << "VendorID: " << deviceDescriptor.idVendor << "\t";
  cout << "ProductID: " << deviceDescriptor.idProduct << endl;
  libusb_config_descriptor *config;
  libusb_get_config_descriptor(usbDevice, 0, &config);
  cout<<"Interfaces: "<<(int)config->bNumInterfaces<<" ||| ";
  const libusb_interface *inter;
  const libusb_interface_descriptor *interdesc;
  const libusb_endpoint_descriptor *epdesc;
  for(int i=0; i<(int)config->bNumInterfaces; i++) {
    inter = &config->interface[i];
    cout<<"Number of alternate settings: "<<inter->num_altsetting<<" | ";
    for(int j=0; j<inter->num_altsetting; j++) {
      interdesc = &inter->altsetting[j];
      cout<<"Interface Number: "<<(int)interdesc->bInterfaceNumber<<" | ";
      cout<<"Number of endpoints: "<<(int)interdesc->bNumEndpoints<<" | ";
      for(int k=0; k<(int)interdesc->bNumEndpoints; k++) {
        epdesc = &interdesc->endpoint[k];
        cout<<"Descriptor Type: "<<(int)epdesc->bDescriptorType<<" | ";
        cout<<"EP Address: "<<(int)epdesc->bEndpointAddress<<" | ";
      }
    }
  }
  cout << endl << endl << endl;
  libusb_free_config_descriptor(config);
}

