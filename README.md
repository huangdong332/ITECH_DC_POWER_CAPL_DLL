<p align="center">
  <a href="" rel="noopener">
 <img width=200px height=200px src="https://i.imgur.com/6wj0hh6.jpg" alt="Project logo"></a>
</p>

<h3 align="center">ITECH DC Power Control CAPL DLL</h3>

<div align="center">

[![Status](https://img.shields.io/badge/status-active-success.svg)]()
[![GitHub Issues](https://img.shields.io/github/issues/huangdong332/ITECH_DC_POWER_CAPL_DLL)](https://github.com/huangdong332/ITECH_DC_POWER_CAPL_DLL/issues)
[![GitHub Pull Requests](https://img.shields.io/github/issues-pr/huangdong332/ITECH_DC_POWER_CAPL_DLL)](https://github.com/huangdong332/ITECH_DC_POWER_CAPL_DLL/pulls)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](/LICENSE)

</div>

---

<p align="center"> A CAPL DLL to control an ITECH DC power.
    <br> 
</p>

## 📝 Table of Contents

- [About](#about)
- [Getting Started](#getting_started)
- [Usage](#usage)
- [Built Using](#built_using)

## 🧐 About <a name = "about"></a>

This CAPL dll provides interfaces to control an ITECH programmable DC power supply through USB in CAPL.
The only two APIs(dllItechDcPowerWrite & dllItechDcPowerQuery) in this dll are implemented on top of VISA(Virtual instrument software architecture).
The standard SCPI command can be send to an ITECH device through those two APIs by the way controlling this device.
The supported SCPI commands can be found in the device's document normally named "One's programming guide".

### Communication Interface

USB and RS232 are both supported now.

## 🏁 Getting Started <a name = "getting_started"></a>

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. 

### Prerequisites

MinGW and Make.

This CAPL dll is built with a MinGW gcc tools for 32bit Windows target. The 64bit CAPL dll can't be recognized by the CAPL browser any way. The compile process is managed by a Makefile.

```
I use msys64 to install MinGW and Make.
```

Instrument Driver

[NIVISA](https://www.ni.com/en-us/support/downloads/drivers/download.ni-visa.html) and [NIIPC](https://www.ni.com/en-us/support/downloads/drivers/download.ivi-compliance-package.html) driver package should be installed on your machine.

## 🎈 Usage <a name="usage"></a>

There is an example code in CAPL. In this test case, a ITECH DC power supply's voltage is set to 5V.After 5 seconds,its output is truned ON. After 5 seconds, its voltage is adjusted to 10V. After 5 seconds, its voltage and current are queried and its voltage is adjusted to 800mV. After 5 seconds, its output is turned off.

### USB

```
testcase TurnOnAndOff()
{
  char resultString[100];
  float result;
  //Set output voltage to 5V.
  dllItechDcPowerWrite("VOLT 5V");
  testWaitForTimeout(5000);
  //Set output state to  ON.
  dllItechDcPowerWrite("OUTP 1");
  testWaitForTimeout(5000);
  dllItechDcPowerWrite("VOLT 10V");
  testWaitForTimeout(5000);
  //Measure output voltage, numberic result saved in "result".
  dllItechDcPowerQuery("MEAS:VOLT?",resultString,result);
  write("Measured voltage: %f",result);
  //Measure output current, numberic result saved in "result".
  dllItechDcPowerQuery("MEAS:CURR?",resultString,result);
  write("Measured current: %f",result);
  dllItechDcPowerWrite("VOLT 800mV");
  testWaitForTimeout(5000);
  //Set output state to  OFF.
  dllItechDcPowerWrite("OUTP 0");
  
}
```

### RS232

```
testcase test()
 {
  char resultString[100];
  float result;
  //!!!IMPORTANT!!!
  //Must be the first step in case of RS232.
  //This SCPI command enables reomte control.
  dllItechDcPowerWriteSerial("SYST:REM");
  dllItechDcPowerWriteSerial("VOLT 5V");
  testWaitForTimeout(5000);
  dllItechDcPowerWriteSerial("OUTP 1");
  testWaitForTimeout(5000);
  dllItechDcPowerWriteSerial("VOLT 10V");
  testWaitForTimeout(5000);
  dllItechDcPowerQuerySerial("MEAS:VOLT?",resultString,result);
  write("Measured voltage: %f",result);
  dllItechDcPowerQuerySerial("MEAS:CURR?",resultString,result);
  write("Measured current: %f",result);
  dllItechDcPowerWriteSerial("VOLT 800mV");
  testWaitForTimeout(5000);
  dllItechDcPowerWriteSerial("OUTP 0");
 }
```

## ⛏️ Built Using <a name = "built_using"></a>

After change to this project's root directory, run follow command to build this CAPL dll.

```
make
```

No test code has been added in this project.
