# AXR
Arduino X-Rebirth Cockpit

This is a working "proof of concept" that an Arduino based hardware cockpit can react to what happens in X-Rebirth by, for example, flashing an LED or changing its color.

Initially, I wanted to build just a more cockpit-like keyboard with some buttons, toggle switches, etc.. but I though it would be cool to build something that could react to what happens in the game, say:
   * display shield/hull state
   * warn about incoming missile and maybe even warn about its close proximity
   * display weapon state, overheating, available missiles, etc...

So far, **shield state indication** is working (hull states require copying and modifying shield code on arduino side).

## Here are things needed to make it happen:
1. Launching the game with "-logfile debuglog.txt -debug scripts" parameter to allow logging debug messages for sending the shield/hull state to Arduino
1. Using a Mission Director xml script that checks the shield/hulls state and logs the change in debuglog.txt
1. Python script that checks if debuglog.txt changes, sends messages to Arduino over USB serial port, optionally listens for messages from Arduino about buttons state and translates them to keyboard events
1. Arduino programmed to listen a USB serial port for commands and control LEDs
1. Optional: LEDs, resistors, jumper wires, buttons

## And here are some more details:
1. This should be self explanatory
1. Mission director script is still fairly simple. Cues that check the shield and the hulls state are instantiated and are checked every 1s but as these are simple checks so it may be possible to check them more often. Shield percentage is translated from 0-100 to 0-4 range as follows: (player.ship.shieldpercentage^0.33)i, to limit how often debuglog.txt is modified. Hull state is also monitored and updated by the MD script but not yet interpreted by Arduino. <show_notification ...> segments used for debugging and can be commented out.
The script is aware if player is on or off the ship.
At least few event states could be interesting for future development, like:
   * <event_player_incoming_missile>
   * <event_player_out_of_ammo>
   * <event_player_cycled_weapon>
   * <event_player_launched_missile>
   * there maybe some other player and player.ship properties that can also be used..
1. Python script (written with Python 3, should work with Python 2 after changing print function to a statement or commenting it out as it is just there for debugging) requires a "serial" (pySerial) and "keyboard" libraries to be installed. Other libraries are standard. In order to use the script, variables "ser.port" and "log" need to be changed to an appropriate COM port number (can be checked when uploading the sketch in Arduino IDE) and debuglog.txt location (usually in user "Documents\Egosoft\X Rebirth\" folder) . The script does following things:
   * Checks if debuglog.txt was modified (every 0.1s)
   * If it was, reads only the last line
   * If the line has values from the MD script, commands are sent over USB serial port to Arduino
   * Script also monitors serial port for incoming communication, and if a button state is communicated, a TAB button is pressed for using an in-game boost (just for a test...).
1. Any Arduino capable of serial over USB is fine. I use a cheep Nano. Arduino Pro Micro/Leonardo could be used if someone wants the system to see it as a kayboard or a gamepad instead of sending commands back over serial port to a python script (but it would require modifying the code). The script is rather simple, self explanatory and relatively well documented (I hope).
It assumes that an external LED is on pin D12, button is on pin D2. State of the shield indicating LED is changed by a series of "if"s comparing a string from serial port to a few defined conditions. Pressing a button will send message to python script and it should also turn on a onboard LED (that is controlled by pin D13, just to test that button works). In a minimal configuration (if one does not have any buttons or external LEDs). the script could be modified to use the onboard LED on pin D13 for the shield state indication.
1. Components and connection can be seen in the photo (a 330 Ohm resistor could be used, instead of 1 kOhm that I used, in order to get a nominal LED brightness).

## Things that could be added to this:
* More LEDs for hull, missiles, etc...
* RGB LEDs for gradual color notification instead of blinking (might need more frequent logging)
* Sound notification
* LCD with active weapon, ammo count, credits ... whatever useful can be extracted from the game.
* LCD with a graphics (one would need to design it first)

I'm guessing X4 will have a similar MD scripting capabilities so this concept could be used as well for the next X game. 
