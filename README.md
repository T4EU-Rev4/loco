# loco
Modules located at the loco to communicate with the showcase controller an the wagons


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

