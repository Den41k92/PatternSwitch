# Pattern Switch
Simple sequence detection library for Arduino<br/>

## Description
This library allows you to create and detect a sequence (pattern) of event series following by different intervals.
For example, this could be used for a door lock which can be opended by knocking it with a certain sequence. 
In this case, each 'knock' called 'trigger'. <br/>

The speed of the sequence is not significant, only order does. This means the intervals between triggers can be as short as 10 msec (default interval resoltion) and as long until timeout (2 seconds by default).
<details>
<summary>Example</summary>
You create a following pattern with error tolerance of 10 % (in milliseconds): <br/>
<code>[ <b>500</b> <span style="color:grey">&plusmn;50</span>, <b>250</b> <span style="color:grey">&plusmn;25</span>, <b>100</b> <span style="color:grey">&plusmn;10</span>, <b>250</b> <span style="color:grey">&plusmn;25</span>) ] <br /></code>
and this will also match the following one:  <br/>
<code>[ <b>250</b> <span style="color:grey">&plusmn;25</span>, <b>125</b> <span style="color:grey">&plusmn;12</span>, <b>50</b> <span style="color:grey">&plusmn;5</span>, <b>125</b> <span style="color:grey">&plusmn;12</span> ] </code> (for 2x speed), as well as: <br/>
<code>[ <b>1000</b> <span style="color:grey">&plusmn;10</span>, <b>500</b> <span style="color:grey">&plusmn;50</span>, <b>200</b> <span style="color:grey">&plusmn;20</span>, <b>500</b> <span style="color:grey">&plusmn;50</span> ] </code>(for 0.5x speed) <br/>
</details>

## Features
- A callback function can be assigned for each event (match, error, end of recording etc.)
- Detection based on relative difference between intervals, not absolute value
- Configurable timeout and % of allowed timing error (tolerance)
- Pattern can be recorded, stored and loaded from EEPROM

By default, the maximum pattern size is 32, but it can be changed inside PatternSwitch.h
