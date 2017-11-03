This Mongoose-OS project uses the ESP32 by Espressif.

The development board contains an ESP32 system-on-chip.  A DHT22 temperature-humidity sensor chip is wired
to the ESP32 board.

The device is configured as a web server.
Websocket is used to connect the device to a browser.

The device periodically sends a JSON string via the Websocket.
The incoming JSON is parsed by Javascript and is used to update
a time, temperature, and humidity display in the web page.  Temperature is plotted in a line chart.

The system is an HTTP5 web server.  The server provides a web page which then connects to the server via Websocket.
The browser must be Websocket capable.  After a Websocket connection is established, the server begins publishing
JSON data.  The data is received by the web page via embedded Javascript code.  D3.js visualization is used
to plot the incoming temperature data versus time.

The project is intended to be used with a wireless router.
The file

fs/conf9.json

determines the configuration of the device as a station.
The SSID and password should be edited for the user's system.
It is also possible to configure the device as an access
point.

To use browse to the URL:

http://192.168.1.3/websocketweather.html

The above is an example.  You will need to use your router
to determine the assigned IP address.  This can also be
seen using

mos console

at the command line.  The device will connect and this
will appear in the console output.
