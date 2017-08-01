# IoT_exam_authentication_system
This is an Arduino based device to authenticate and mark attendance of students in exams using finger prints and magnetic ID cards.

# Modules
1. Arduino Mega Board
2. Adafruit optical finger print sensor
3. HCC750M Magnetic swipe card reader
4. NX4024T032_11 Nextion TFT touch display
5. ESP-01 Wifi module

# Device
The code for the Arduino is in the file sys.ino. The connections are described in the sys.ino file itself. The device is programmed to connect a remote server to retrieve the student details to authenticate.

# Server
The server runs on java. A MySQL database contains the details of the students. The details are read and send to the device as requested. 


