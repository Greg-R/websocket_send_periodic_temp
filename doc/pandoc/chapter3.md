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


