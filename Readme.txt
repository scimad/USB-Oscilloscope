This is the Readme file for the USB-Oscilloscope project. In this example, we show
how stream of data can be sent to PC with the device using only functionality
compliant to the HID class. Since class drivers for HID are included with
Windows, you don't need to install drivers on Windows.


WHAT IS DONE?
=============
This projects shows how the HID class can be "misused" to transfer fixed
size blocks of data (up to the driver's transfer size limit) over HID feature
reports. This is "control data transfer" mode . This technique is of great 
value on Windows because no driver DLLs are needed (the hid-custom-rq example 
still requires the libusb-win32 DLL,although it may be in the program's 
directory). The host side application requires no installation, it can even
 be started directly from a CD. The reference to this project is taken from
 V-USB library and it's example projects.

PREREQUISITES
=============
Hardware:   AVR based circuit with D+ , D- connected to designated pins,
            a pullup resistor 1.5k is connected to D- (for a low speed 
            device). The diodes in the communication side circuit are used
            for constant voltage drop (0.7+0.07) so that AVR works on 3.6V.

AVR development environment:    gcc tool chain for the AVR,

Host development environment:   A C compiler and libusb on Unix. 
                                On Windows, you need the Driver Development
                                Kit (DDK) Instead of libusb. MinGW ships with
                                a free version of the DDK.

THE FIRMWARE
============
Makefile can be modified according to architecture (CPU clock, target device,
 fuse values) and ISP programmer. Then usbconfig.h can be setup according to
  pin assignments for D+ and D-.

Type "make hex" to build main.hex, then "make flash" to upload the firmware
to the device. Don't forget to run "make fuse" once to program the fuses. If
you use a prototyping board with boot loader, follow the instructions of the
boot loader instead.

Please note that the first "make hex" copies the driver from the top level
into the firmware directory. If you use a different build system than our
Makefile, you must copy the driver by hand.

The device implements a USB-Oscilloscope sending n-blocks of data per USB Report.
Since the conversion time for n conversions is too fast compared to the transfer
time, or say transfer is too slow, it creates a huge problem. The data can't be
sampled when the data are being transferred. So, this implementation of firmware
transfers only one byte of data at once so that the sampled data are in uniform
time gap.


BUILDING THE HOST SOFTWARE
==========================
Make sure that you have libusb (on Unix) or the DDK (on Windows) installed.
MinGW on Windows is recommended since it includes a free version of the DDK.
Then change to directory "host-side" and run "make" on Unix or
"make -f Makefile.windows" on Windows.


THE HOST-SIDE TOOL
==================
The data sent from the device is read and plot in the host-side tool.
To receive the data, use

    hidtool read


----------------------------------------------------------------------------
Minor Project by,
Kaveer Panthi (071-BEX-318)
Madhav Humagain (071-BEX-321)
Sandhya Guragain (071-BEX-334)