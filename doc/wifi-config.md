#  WIFI Configuration for STA and AP

This chapter is a tutorial on how to configure the device
for either WIFI station or access point modes.

There may be other methods to accomplish this.  I found
the following method to work successfully and to have some
flexibility.  Keep in mind that Mongoose-OS configuration
facilities are designed for commercial products and end-user
configuration.  This will deviate from that goal a bit,
being that this project is targeted at the hobbyist and
experimenter.

## Mos tool for Configuring as Station

It is easy to use the mos tool at the command line to configure WIFI.

For example, let us say you have a home router, and you would like
your device to connect to it, preferably with DHCP active in the router.

The default configuration of the device will be as an "Access Point".
The following series of commands will switch your device to a station:

~~~
mos wifi WIFI_NETWORK_NAME WIFI_PASSWORD
~~~

The device should connect to the router quickly.  If it does not,
make sure any sort of router access control settings are temporarily disabled.

The settings are volatile with regards to flashing the device.  The mos wifi
command will be have to run again each time the device is flashed.

However, this can easily be made permanent.  Use this command:

~~~
mos ls
~~~

You should see a listing of files something like this:

~~~
ca.pem
conf0.json
conf9.json
index.html
sys_config_schema.json
sys_ro_vars_schema.json
~~~

The key here is the file conf9.json which is the "user configuration file".
To make this permanent, a copy needs to be made and moved to the fs directory
in the project directory:

~~~
mos get conf9.json > fs/conf9.json
~~~

The above command pipes the file from the device into the fs directory with the name conf9.json.
Now you need to build and flash the project again:

~~~
mos --clean build
mos flash
~~~

Now run mos ls again, and you should see that the file con9.json is in the device's local file system.

##  Back to an Access Point

Simply delete the file conf9.json from the fs directory and flash the device.
The default IP address seen is 192.168.4.1 and the default password is Mongoose.

A good way to monitor the boot-up process is to use the console:

~~~
mos console
~~~

Pressing the reset button on the development board should start the boot process, and numerous
messages will be written to the terminal.  This will include IP addresses in the case of a station
connecting to a router with DHCP and also connection status.  Very useful!






