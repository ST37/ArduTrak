# ArduTrak
Arduino software for remote-controlled tracked vehicles.  

This software has _nothing_ to do with ArduPilot.  

The aim is to find a software solution to use the bus systems of RC receivers to control small tracked vehicles.  
Especially with vehicles like in the FB group [Tiny Trak - Micro FPV Crawler](https://www.facebook.com/groups/181517579149618 "Title"), space is a great asset.  
Most micro receivers only offer bus systems such as DBUS (S-BUS), IBUS, EX-BUS (Jeti) etc.  
The advantage of a software solution is that you can use all 16 channels (such as with the Frsky XM receiver) and with receivers with telemetry you can also send data back to the transmitter.  
In addition, an important part of mixing the wheel control channels can be done in the code.  
An Arduino Pro Micro Clone with a 32U4 processor is currently in use.  
The code _cannot_ be executed with a UNO or the like because a second serial port is missing there.  
At the moment I'm working on integrating the S-BUS protocol to control servos and lights.  
He will have some modes that influence the driving behavior, depending on channel values ​​(switch on the transmitter).  
In addition to measuring the LiPo voltage, gyro data, a compass (based on the GY-9250) and an integration for controlling MinimOSD are also acquired.  
This is followed by other protocols such as IBUS and EX_BUS.  
