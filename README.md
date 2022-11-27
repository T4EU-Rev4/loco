# loco
Modules located at the loco to communicate with the showcase controller an the wagons.
See Logo_01 for the actual implementation of the ESP inside the loco.


## Test_MQTT_1
This is just the demo given by EMQX to see, if everything is working propery.
https://www.emqx.com/en/blog/esp32-connects-to-the-free-public-mqtt-broker 

Use the tool MQTT client tool MQTT X mentioned in the section "Required IoT components"


## Test_MQTT_2
We register some more topics and show, how they can be seperated in the callback routine callback
The topics are organized in a struct, where a bus adress is assigned to every topic.
These structs are organized in an array of struct, so in case of receiving a topic is is enough
to compare the topics and read the assigned adress of the device (wagon).
getIntFromString() read the first, second, third,... in value from the received payload.
c=3;v=7 is fine  but not necessary.  3;7 will also work.
see: https://forum.arduino.cc/t/int-aus-string-extrahieren/143685   thanks to jurs and serenifly

Next step: Clean up code, move to VSCode, implement I2C for wagon control


## ESP32_Test_02
Just a Blink version with LED on Pin GPIO23 to see, is the toolchain in VSCode is propertly installed.

## ESP32_Test_03
Test a single PCF8575 device with ESP32

## ESP32_Test_04
Introduce a new small library I2CDevice. It can replace the library for the PCF8575 on the ESP for the use
in the T4EU project. Only output mode is supported. In can word on a per bit base (as the old library can)
as well as on a per byte mode as required for more sophisticated animations based on microcontrollers in a wagon.

## Loco_01
This is a first version of a firmware on the ESP32, to fullfill the follwoing requirement:

- accept some commands from a MQTT-Server
- start and stop animations on different wagons of the train by MQTT-commands

The devices PCF8575 are just port expanders with no processing capabilities. So any animations must run on the ESP controlling these devices.
To keep the code readable, we build a kind of a frame work, where the different animations can be put in own classes derived from the basic animation class TAnimationBasic. Due to the PCF8575, there are two bytes available named _portA and _portB. These two bytes are transfered to the desired I2C device, so set the bits in this two byte accordingly.

There a three virtual methods to implement: \\ 
**beforeStart()** \\
This method is called once, befor an animation starts. Do all the prerequisits and initalizations here. \\ \\ 

**afterStop()** \\
This is the place to clean up the animation, eg. stop motors, switch off any Led and so an. It ist called once when animation stops.

**animate()** \\
This method will do the whole job of animation. It is called every 100ms. 









