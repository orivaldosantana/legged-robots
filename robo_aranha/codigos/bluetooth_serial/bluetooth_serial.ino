/* FILE:    ARD_BLUETOOTH_SERIAL_MODULE_HCARDU0004_Example.pde
   DATE:    17/07/12
   VERSION: 0.2

  REVISIONS:
  
  27/08/12> V0.2 updated software serial functions as these are now included 
            with V1.0 of the Arduino development environment.

This is a simple example of how to use the HobbyComponents Bluetooth serial 
module (HCARDU0004). This module allows communication from an Arduino dev 
board to a Bluetooth enabled device. The module communicates with an Arduino 
device via a simple two wire serial protocol. If you choose to use the modules 
default settings then the module requires no setting up to communicate with it,
just connect to the Arduino. The device defaults to 9600 baud, 8 data bits, 
and 1 stop bit. The baud rate, amongst other settings can be changed using 
standard AT commands via its serial interface.

This sketch demonstrates an example of how to communicate with the 
device using a software serial interface using just two DIO lines to interface 
with the module. The program will pass-through for any data sent to and from 
the device to the Arduino's hardware serial interface. This will allow you to 
communicate with any device paired with the module via Arduino IDE's built in 
serial port monitor.


SENSOR PINOUT:

PIN 1: KEY
PIN 2: VCC
PIN 3: GND
PIN 4: TXD
PIN 5: RXD


SETUP INSTRUCTIONS:

Connect the Bluetooth module as follows:

Arduino       Bluetooth module
5V            PIN 2 (VCC),
GND           PIN 3 (GND)
DIO 10        PIN 4 (TXD)
DIO 11        PIN 5 (RXD)

Compile the sketch and upload to your Arduino. 

Pair your Bluetooth enabled device with the Bluetooth module using PASSCODE: 1234
(you do not need to do anything on the module side to do this).

Open up the serial port monitor in the Arduino IDE (CTRL+SHIF+M).

You will now be able to communicate with the paired Bluetooth device.


You may copy, alter and reuse this code in any way you like but please leave 
reference to HobbyComponents.com in your comments if you redistribute this code. 
THIS CODE MAY NOT BE USED IN ANY FORM BY OTHER EBAY SELLERS.*/



/* Include the software serial port library */
#include <SoftwareSerial.h>

/* DIO used to communicate with the Bluetooth module's TXD pin */
#define BT_SERIAL_TX_DIO 13
/* DIO used to communicate with the Bluetooth module's RXD pin */
#define BT_SERIAL_RX_DIO 14

/* Initialise the software serial port */
//SoftwareSerial BluetoothSerial(BT_SERIAL_TX_DIO, BT_SERIAL_RX_DIO);

void setup()  
{
  /* Set the baud rate for the hardware serial port */
  Serial2.begin(9600);
  /* Set the baud rate for the software serial port */
  //BluetoothSerial.begin(9600);

}

/* Main loop that will pass any data to and from the Bluetooth mode to the
   host PC */
void loop()
{
 
  /* If data is available from the Bluetooth module then pass it on to the 
     hardware serial port. 
  if (BluetoothSerial.available()) {
    char ch = BluetoothSerial.read(); 
    Serial.print("bbbb: ");
    Serial.println(ch);
  }
  */
 
   /* If data is available from the hardware serial port then pass it on 
      to the Bluetooth module. */
  if (Serial2.available()) {
    //BluetoothSerial.write(Serial.read());
    char ch = Serial2.read(); 
    Serial2.print("1");
    Serial2.println(ch);
  }
    
  
}
