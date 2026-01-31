#pragma once

#include "stdafx.h"
#include <string>
#include <chrono>
#include "Serial.h"

enum type {Digital = 0, Analog, Servo}; ///< Enum holds commands

/**
* @class CControl
* @brief C++ object used for communication with the microcontroller
*
* This class has all the variables to interact with
*  the Tiva-C tm4c123 (80MHz) microcontroller.
*
* @author Jass Saini
*
*/
class CControl {
private:
    
    Serial _com; ///< Used for PC to microcontroller communication.
    bool _connected; ///< Indicates if serial connection is active.
    std::chrono::steady_clock::time_point _lastDebounceTime; ///< Check last valid button 

public:
    /**
    *
    * @brief Constructor for CControl class.
    *
    * Initializes internal variables.
    *
    */
    CControl(); 

    /**
    *
    * @brief Destructor for CControl class.
    *
    * Closes serial connection if open.
    *
    */
    ~CControl(); ///< Destructor CControl

    /**
    *
    * @brief Initializes the serial port to the com port.
    *
    * @param comport The comport being targeted
    * @return Nothing to return
    */
    void init_com(int comport); 
    
    /**
    *
    * @brief Sends GET command to microcontroller and receives data.
    *
    * @param type Command type (Digital, Analog, Servo)
    * @param channel Channel number
    * @param result Returned value
    * @return True if successful, false otherwise
    *
    */
    bool get_data(int type, int channel, int& result); 

    /**
    *
    * @brief Sends SET command to microcontroller.
    *
    * @param type Command type (Digital, Analog, Servo)
    * @param channel Channel number
    * @param value Value to send
    * @return True if successful, false otherwise
    *
    */
    bool set_data(int type, int channel, int value); 

    /**
    *
    * @brief Reads analog channel and converts to percentage.
    *
    * @param channel Analog channel number
    * @param percentFullScale Returned percentage (0-100%)
    * @return True if successful, false otherwise
    *
    */
    bool get_analog(int channel, float& percentFullScale);

    /**
    *
    * @brief Runs analog joystick test.
    *
    * Continuously reads joystick X and Y channels
    * and displays raw values and percentages.
    *
    * @return True when test is exited
    *
    */
    bool analog_test();

    /**
    *
    * @brief Reads digital button with debounce.
    *
    * @param channel Digital channel number
    * @param pressed True when button press detected
    * @return True if successful, false otherwise
    *
    */
    bool get_button(int channel, bool& pressed);
    
    /**
    *
    * @brief Runs digital input and LED output test.
    *
    * Reads a digital input pin and mirrors its state
    * to a digital output LED.
    *
    * @param buttonPin Digital input pin number
    * @param ledPin Digital output LED pin number
    * @return True when test is exited
    *
    */
    bool digital_test(int buttonPin, int ledPin);

    /**
    *
    * @brief Runs button debounce test.
    *
    * Counts number of valid button presses.
    *
    * @param buttonPin Digital input pin number
    * @return True when test is exited
    *
    */
    bool button_test(int buttonPin);

    /**
    *
    * @brief Runs servo sweep test.
    *
    * Sweeps servo back and forth between limits.
    *
    * @param servoChannel Servo channel number
    * @return True when test is exited
    *
    */
    bool servo_test(int servoChannel);

    /**
    *
    * @brief Automatically scans COM ports and connects to the microcontroller.
    *
    * @return True if connected, false otherwise
    *
    */
    bool auto_connect();

    /**
    *
    * @brief Checks connection and attempts reconnection if disconnected.
    *
    * @return True if connected, false otherwise
    *
    */
    bool ensure_connection();
};
