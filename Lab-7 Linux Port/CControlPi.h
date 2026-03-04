#pragma once

#include <string>
#include <chrono>

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
