# blinkin

Hardware Change Request
1. Add +5V to headers
2. Add RST to headers
  a. would be nice to have by the TX and RX pins on J4
3. Connect Signal (D6) to screw terminal
4. Move PWM input signal to D2 (Interrupt 0)
  a. Does this port need to be isolated?
  b. Does it create a ground loop as is?
5. Reverse input protection?
6. AVR Bypass caps need to be before the device on VCC
7. The Atmega328p would make the toolchain much easier to use for teams than the 328pb
8. A 16MHz crystal would also make the toolchain easier
  a. As it is now, a new board defination would have to be added by the teams wanting to program the device
