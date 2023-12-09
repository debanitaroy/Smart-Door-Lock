# Smart-Door-Lock
A smart door locking system, using arduino, servo motors and RFID Module and relay module.
I have connected the LCD Screen. The 4x4 membrane keypad and the one-channel relay is also connected to the Arduino which has been properly programmed. I have completed the circuit, which is displayed in the above picture. I have connected the RFID module and servo motor which is connected to the solenoid lock and ten to the 12V adapter.
When the wrong password is entered via the keypad the lcd screen will display “ACCESS DENIED” and the servo motor will not open and the door remains locked. The same happens when the RFID scanner scans a wrong card which does not have the correct hex code as the key. 
“ACCESS GRANTED” message is only displayed when the correct set password is entered or the card having the right hex code is scanned.
