# REV Blinkin LED Driver

## Getting Started

### Getting the board ready

insert diagram

- Follow the diagram to plug the Blinkin board into an Arduino UNO
- Plug a 12V battery into the Blinkin with an XT30 cable
- Plug an LED stip into the Blinkin with the Blinkin LED cable adapter
- Plug the Arduino UNO into your computer with a USB-A to USB-B cable

### Setting up the Arduino IDE

- Download the [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- Install and open the IDE
- In the **Tools** tab, select "Arduino/Genuino Uno" under **Board**

<img src="https://github.com/willtoth/BlinkinFirmware/blob/master/images/Tools-Board%20Screenshot.png" width="720" height="405" />

- Again in the **Tools** tab, select the port the Arduino is located at

<img src="https://github.com/willtoth/BlinkinFirmware/blob/master/images/Tools-Port%20Screenshot.png" width="720" height="405" />

### Uploading the Blinkin firmware to the board

- Download **BlinkinFirmware-master.zip** from this GitHub repository
- Extract the contents into a folder named **BlinkinFirmware**
- Open **BlinkinFirmware.ino** in the Arduino IDE
- Download [FastLED-master.zip](https://github.com/FastLED/FastLED) and [CircularBuffer-master.zip](https://github.com/rlogiacco/CircularBuffer)
- Under "Include Library menu," located in the **Sketch** tab, click "Add .ZIP Library..."

<img src="https://github.com/willtoth/BlinkinFirmware/blob/master/images/Sketch-Include%20Library%20Screenshot.png" width="720" height="405" />

- Locate **FastLED-master.zip** and **CircularBuffer-master.zip** and add both of them
- Press the **Upload** button at the top of the IDE to load the firmware onto the board

<img src="https://github.com/willtoth/BlinkinFirmware/blob/master/images/Upload%20Screenshot.png" width="720" height="405" />

### Using the preset patterns

- Hold the **Mode** button on the Blinkin board to enable setup mode
- Use the **Mode** and **Strip Select** buttons to cycle through the preset patterns
- Turn the left two potentiometers on the Blinkin to adjust the preset team colors (Used in patterns 49 through 79)
- Turn the rightmost potentiometer to adjust the length of the LED strip
- A list of the preset patterns can either be found in **BlinkinFirmware.ino** or [here](http://www.revrobotics.com/content/docs/REV-11-1105-UM.pdf)

## Editing the Firmware

### Using the Serial Monitor

- In **BlinkinFirmware.ino**, add `Serial.begin(115200);` in setup()

<img src="https://github.com/willtoth/BlinkinFirmware/blob/master/images/Serial-begin%20Screenshot.png" width="720" height="405" />

- Use `Serial.print();` or `Serial.println();` anywhere in the code to print to the Serial Monitor
- Press the **Serial Monitor** button at the top right corner of the IDE to open the Serial Monitor

<img src="https://github.com/willtoth/BlinkinFirmware/blob/master/images/Serial%20Monitor%20Screenshot.png" width="720" height="405" />

- At the bottom right corner of the Serial Monitor, set the baud rate to 115200

<img src="https://github.com/willtoth/BlinkinFirmware/blob/master/images/Serial%20Monitor-baud%20Screenshot.png" width="720" height="405" />

#### Seeing what pattern you cycle to

- Open **UserIO.ino**
- Add `Serial.println(noSignalPatternDisplay);` at the locations in the screenshots below

<img src="https://github.com/willtoth/BlinkinFirmware/blob/master/images/Print%20Cycle%20Screenshot_1.png" width="720" height="405" />
<img src="https://github.com/willtoth/BlinkinFirmware/blob/master/images/Print%20Cycle%20Screenshot_2.png" width="720" height="405" />

### Creating a custom function

- In **BlinkinFirmware.ino**, there is an array of functions named `gPatterns`
- Change the name of an existing function in this list of functions

<img src="https://github.com/willtoth/BlinkinFirmware/blob/master/images/gPatterns%20Change%20Function%20Screenshot.png" width="720" height="405" />

- Then, to modify what it does, open the **.ino** file the function is located in (For this example, open **PWM_1_Standard.ino**)
- Now find the function that was renamed, and change its name to the new one here
- Inside the brackets next to the function is what runs when the function is selected
- Delete the contents in the brackets and add a new method

<img src="https://github.com/willtoth/BlinkinFirmware/blob/master/images/PWM_1_Standard%20Change%20Function%20Screenshot.png" width="720" height="405" />

- Below, create the new method with the desired parameters and create what the function does inside the brackets
- In this example, the method makes the LED strip blink rapidly between two colors, similar to the strobe function

<img src="https://github.com/willtoth/BlinkinFirmware/blob/master/images/Add%20Method%20Screenshot.png" width="720" height="405" />
