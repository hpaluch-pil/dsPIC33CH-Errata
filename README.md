# dsPIC33CH ISR/REPEAT errata

Attempt to reproduce dsPIC33CH Slave core Errata, where
it may corrupt stack (by repeating entry instructions like - PUSH) or 
abort early REPEAT instruction leading to unexpected results.

Errata is available in this document:
- https://ww1.microchip.com/downloads/en/DeviceDoc/dsPIC33CH512MP508-Family-Silicon-Errata-and-Data-Sheet-Clarification-DS80000805K.pdf

Status:
- it successfully triggers AddressError TRAP in few seconds.

> WARNING!
>
> I think that this AddressError trap is because of Hardware error described in errata.
> However that it is still chance that it is not the case and that it is caused by some bug in my program(!).
>
> Please consider all content here WITHOUT ANY WARRANTY!

# Required Hardware

- [dsPIC33CH Curiosity Development Board DM330028-2](https://www.microchip.com/en-us/development-tool/DM330028-2)
- two Micro-B USB cables
  - one for programming debugging
  - 2nd for UART connection where are printed debugging instructions

# Required Software

- MPLAB X IDE v6.15
- XC16 v2.10
- DFP 1.14.357

# How to reproduce TRAP

Current code will lead to AddressError trap in few seconds.

What it does:
- Master Core
  - 8 Mhz external Clock, 180 MHz (90 MIPS) Core clock
  - toggles `RE0_LED1` every 50ms using Delay
- Slave Core
  - 8 Mhz external Clock, 200 MHz (100 MIPS) Core clock
  - debug messages on UART (`RC10_RXB` PIC Input, `RC11_TXB` PIC Output)
    - please use these `COMx` port settings on your PC: Speed 115200 Baud, 8-bit data, 1 stop bit,
      no parity, no flow control
  - toggles LED2 around every 1s using main loop Delay
  - rotating RGB LED around every second
  - toggle `RC7_ANA` on every CCP1 interrupt
  - toggle `RD4_RSTA` on every CCP2 interrupt

Both cores:
- permanently lit LED1 (for Master) or LED2 (for Slave) in case of TRAP.

When:
- CCP1 interrupt frequency is around 10 kHz
- CCP2 interrupt frequency is around 100 kHz

> NOTE:
>
> Please look also at
> branch [br-faster-trap](https://github.com/hpaluch-pil/dsPIC33CH-Errata/tree/br-faster-trap)
> Where the code trigger trap even faster and where is also Logic Analyzer output.

I get trap
```
TRAPS_ADDRESS_ERR = 2, /** Address error Trap vector */
```
In 3 seconds - example of UART output:
```
SLAVE main:173 starting v1.00
SPLIM=20464 (0x4ff0)
MC=1 INT1=8191 INT2=16300
 INT1SP=0x1224 INT2SP=0x1206 MAXSP=0x1246 SPLIM=0x4ff0
MC=2 INT1=16383 INT2=32610
 INT1SP=0x1226 INT2SP=0x1206 MAXSP=0x124e SPLIM=0x4ff0
MC=3 INT1=24575 INT2=48921
 INT1SP=0x1222 INT2SP=0x122a MAXSP=0x124e SPLIM=0x4ff0
!2
```

The `!2` is trap message from customized `ErrataSlave.X/mcc_generated_files/traps.c`,
where `!` means Trap occurred and `2` is Error code - in this case `AddressError`.
Also red LED2 should stop blinking and RGB LED should stop rotating (currently accidentally on Green, but
it is just coincidence).

# Resources

* Official Curiosity Board page:
  - https://www.microchip.com/en-us/development-tool/DM330028-2
* Errata for dsPIC33CH512 used on Curiosity board
  - https://ww1.microchip.com/downloads/en/DeviceDoc/dsPIC33CH512MP508-Family-Silicon-Errata-and-Data-Sheet-Clarification-DS80000805K.pdf


