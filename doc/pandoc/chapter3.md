# The Web Browser Code: HTML, CSS, and Javascript

The single-page app displays the incoming temperature data from the ESP32 server.
The data is plotted in a single line chart versus time.

The current hard-coded default time period displayed in the chart is 12 hours.

D3.js is used to build the line chart.  The web browser must have Websocket
and ES6 version Javascript.

The Websocket and message parsing Javascript is easy to follow.  However, the D3.js
code used to display the data in the line chart is a little more complicated.
There are numerous resources available for learning D3.js.  Here are the top resources:

[https://d3js.org/](https://d3js.org/)

and

[https://bl.ocks.org/](https://bl.ocks.org/)

This project used this as a starting point:

[https://bl.ocks.org/curran/ba21316eafc6b84b22d1a5d49ad2a798](https://bl.ocks.org/curran/ba21316eafc6b84b22d1a5d49ad2a798)

The author of the above line chart has a nice youtube tutorial on D3.js:

[https://www.youtube.com/watch?v=8jvoTV54nXw](https://www.youtube.com/watch?v=8jvoTV54nXw)

##  HTML5, CSS, and Javascript Code in websocketweather.html

The ESP32 connects via websocket to the web page websocketweather.html.
The file is located in the fs directory in the Mongoose-OS project.

The event handler code is bound to the route "/", so the URL will be:

~~~
http://192.168.1.4/websocketweather.html
~~~

where the example IP address is 192.168.1.4 is shown.

If the ESP32 is configured for station mode, the IP address
can be determined using the router web interface.  Another method
is to monitor the ESP32 as it boots.  This is done using this command:

~~~
mos console
~~~

at the command line, and assuming the ESP32 is connected via USB/UART.
The IP address will be shown as the ESP32 successfully connects to an access point.

Onto the code...

For this "single page app" the CSS and Javascript are embedded in the web page.
In the "head" block, there is a script tag which loads the D3.js code for building
the line chart.  This is followed by a block of conventional CSS styling which
is applied to various pieces of the line chart.

The structure of the page is simple, there being only a few lines of HTML5 text and then the line chart.
Most of the code is a script block with the Javascript necessary to build the line chart.  There is a
message handler which parses incoming JSON into time and temperature data which is used to update the chart.

The x axis is fixed to a 12 hour time span.  Temperature data is plotted at 5 minute intervals.  This seems
sufficient to produce a smoothly changing line.

The y axis is fixed to a range of 50 to 100 degrees Fahrenheit.  This can be changed to any desired range
easily enough, and the Celsius to Fahrenheit conversion function can be removed to display the native Celsius
output of the DHT22 sensor.

##  Websocket Connecting

This bit of code determines the correct IP address, and then negotiates a Websocket connection
to the ESP32.  The trick is to use a Regular Expression to extract the IP address.
Then it is simple to use the Websocket API to open the connection.

~~~
        const serverUrlRegex = /\d+\.\d+\.\d+\.\d+/; //  Matches 192.168.1.8 etc.
        const currentUrl = window.location.origin; //  Get the URL from the browser.
        console.log(`The currentURL is ${currentUrl}.`);
        const serverUrl = currentUrl.match(serverUrlRegex); //  Extract what is needed to create WebSocket.
        console.log(`The server URL is ${serverUrl[0]}`); //  The match is in the 0th element of the array.
        let ws = new WebSocket(`ws://${serverUrl}`);
~~~

Again using the Websocket API, the status of the Websocket is indicated on the web page.
This little textual indication of Websocket status in the web page proved very important.

~~~
        ws.onopen = () => {
            console.log('Web browser opened a WebSocket.');
            //  Update the connection status in the browser.
            connectstatus.textContent = "WebSocket Connected";
        }
        ws.onclose = () => {
            console.log('Web browser WebSocket just closed.');
            //  Update the connection status in the browser.
            connectstatus.textContent = "WebSocket is disconnected";
        }
~~~

##  D3.js Line Chart

This D3.js code is perhaps a little unusual in that the chart is not
constructed until the first time data is received from the ESP32.
The code gets the first time stamp, and then uses this to construct
the x axis.  So the first incoming time is the left end of the time axis,
and the end is set to 12 hours later.

This is done by putting some of the line chart building code in a function,
and then only running that function after the first time data is received.
This is done only once, thus the variable

~~~
let chartBuilt = false;
~~~

is initialized to false, and is changed to true at the conclusion
of the processing of the first incoming time data:

~~~
if (chartBuilt === false) buildChart();
~~~

and then chartBuilt = true at the conclusion of the buildChart() function.

##  Websocket Message Handling

The Websocket API is once again called upon, this time to respond to
events caused by incoming messages from the ESP32.

~~~
        ws.onmessage = (message) => {
            console.log(message.data);
            //  Parse the incoming JSON data from the ESP32.
            let statusMessage = JSON.parse(message.data);
            //  Check the message type and handle as required.
            if (statusMessage.messageType === "temperature") {
                console.log(`wxData message received`)
                let newTempData = {
                    'time': currentTime,
                    'temp': +statusMessage.currentTemp
                };
                //  Add a new data object to the end of the array.
                tempData.push(newTempData);
                //  Update chart.
                update(tempData);
                //  Update textual display.
                temperature.textContent = statusMessage.currentTemp;
            } else if (statusMessage.messageType === "humidity") {
                humidity.textContent = statusMessage.currentHumidity;
            } else if (statusMessage.messageType === "serverTime") {
                //  Current Server Time Updater.  Updated every 1 second by server.
                currentTime = new Date(statusMessage.serverTime);
                console.log(`The current time from the server is ${currentTime}`);
                let hourMinuteFormat = d3.timeFormat("%H:%M");
                let hourMinute = hourMinuteFormat(currentTime);
                console.log(`hour and minute ${hourMinute}`);
                clock.textContent = currentTime;
                //  The chart is built only after receiving the first time data.
                if (chartBuilt === false) buildChart();
            }}
~~~

The code is almost self-explanatory.  Incoming Websocket messages are parsed into
proper Javascript objects, and an if-else-if block is used to handle the messages.
Each message has a "messageType" field, and this is used to process the message
accordingly.

Note that in the case of incoming temperature data, the array tempData is updated,
and then the line chart is updated via the call to the update() function.

The humidity data is current only used in the textual display.  A second line
chart for humidity could be added easily enough.

##  Volatility of the Chart Data

This is a simple demonstration, and the data is not stored in non-volatile memory.
If the browser is refreshed, the data will be lost.  Also, the data does not wrap
when it reaches the end of the 12 hour time period.

The next project will use Google IOT or AWS to allow non-volatile data storage.
