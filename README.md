# SpikenzieLabs Solder : Time 2

...repurposed as a Christmas ornament.

Ref: http://spikenzielabs.com/SpikenzieLabs/SolderTime_2.html

License from ST_Two_Release.ino
```
// LEGAL:
// This code is provided as is. No guaranties or warranties are given in
// any form. It is your responsibility to determine this codes suitability
// for your application.
```

Changes:
* Moved "Scrolling Message" mode to first mode so it displays the scrolling message (instead of the time display) when someone presses the "Mode" button.
* Increased the message size to 63 characters from 24 characters.
* Smooth scroll the text onto the display as well as off the display, stopping when the display is blank (originally stopped with the first four characters on the display).
* Wake up on every five minutes from 8:00AM to 5:00PM, scroll the message.

Not used:
* Play a Christmas carol
  * Carol of the Bells looks like a good choice
  * The quality of the music produced by the piezo speaker is very poor, to the point of being unrecognizable.
