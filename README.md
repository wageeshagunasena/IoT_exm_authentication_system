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

The interface file for the TFT display is Interface.HMI.

Used libraries:

1. Adafruit_Fingerprint_Sensor_Library
2. ITEADLIB_Arduino_Nextion-0.7.0
3. magstripelib-master

# Server
The server runs on java. A MySQL database contains the details of the students. The details are read and send to the device as requested. When the authentication for a specific exam is finished a report containing the details of attendance is send to a defined e-mail address.

The SQL file containing the database is in the file DB.sql. The Netbeans project for the server is in the folder Server.

Used libraries:

1. mysql-connector-java-5.1.39
2. javax.mail

# Installation
1. Download and install Nextion editor from the link - http://nextion.itead.cc/download.html

2. Run Nextion Editor and open the Interface.HMI project file.

3. Select the correct device and compile. Open the build folder (File->Open Build Folder) and copy the generated .tft file to a emptied sd card.

4. Insert the sd card to the slot in the display and set the power on. The display start to download the interface file and show when it is finished. The set the power off and remove the sd card. Now the interfaces are installed in the display.

5. Connect the modules and the Arduino Mega board as defined in the sys.ino file

6. Install the device libraries mentioned in the Device section to your library folder in the Arduino working directory.

7. Through the Arduino IDE complie and upload the device code to the board.

8. 
