<p align="center">
  <a href="" rel="noopener">
 <img width=200px height=200px src="https://i.imgur.com/6wj0hh6.jpg" alt="Project logo"></a>
</p>

<h3 align="center">Project Title</h3>

<div align="center">

[![Status](https://img.shields.io/badge/status-active-success.svg)]()
[![GitHub Issues](https://img.shields.io/github/issues/kylelobo/The-Documentation-Compendium.svg)](https://github.com/kylelobo/The-Documentation-Compendium/issues)
[![GitHub Pull Requests](https://img.shields.io/github/issues-pr/kylelobo/The-Documentation-Compendium.svg)](https://github.com/kylelobo/The-Documentation-Compendium/pulls)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](/LICENSE)

</div>

---

<p align="center"> Few lines describing your project.
    <br> 
</p>

## 📝 Table of Contents

- [About](#about)
- [Getting Started](#getting_started)
- [Deployment](#deployment)
- [Usage](#usage)
- [Built Using](#built_using)
- [TODO](../TODO.md)
- [Contributing](../CONTRIBUTING.md)
- [Authors](#authors)
- [Acknowledgments](#acknowledgement)

## 🧐 About <a name = "about"></a>

This CAPL dll provides interfaces to control an ITECH programmable DC power supply through USB in CAPL.
The only two APIs(dllItechDcPowerWrite & dllItechDcPowerQuery) in this dll are implemented on top of VISA(Virtual instrument software architecture).
The standard SCPI command can be send to an ITECH device through those two APIs by the way controlling this device.
The supported SCPI commands can be found in the device's document normally named "One's programming guide".

## 🏁 Getting Started <a name = "getting_started"></a>

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See [deployment](#deployment) for notes on how to deploy the project on a live system.

### Prerequisites

MinGW and Make.

This CAPL dll is built with a MinGW gcc tools for 32bit Windows target. The 64bit CAPL dll can't be recognized by the CAPL browser any way. The compile process is managed by a Makefile.

```
I use msys64 to install MinGW and Make.
```

## 🎈 Usage <a name="usage"></a>

There is an example code in CAPL. In this test case, a ITECH DC power supply's voltage is set to 5V.After 5 seconds,its output is truned ON. After 5 seconds, its voltage is adjusted to 10V. After 5 seconds, its voltage and current are queried and its voltage is adjusted to 800mV. After 5 seconds, its output is turned off.

```
testcase TurnOnAndOff()
{
  char resultString[100];
  float result;
  dllItechDcPowerWrite("VOLT 5V");
  testWaitForTimeout(5000);
  dllItechDcPowerWrite("OUTP 1");
  testWaitForTimeout(5000);
  dllItechDcPowerWrite("VOLT 10V");
  testWaitForTimeout(5000);
  dllItechDcPowerQuery("MEAS:VOLT?",resultString,result);
  write("Measured voltage: %f",result);
  dllItechDcPowerQuery("MEAS:CURR?",resultString,result);
  write("Measured current: %f",result);
  dllItechDcPowerWrite("VOLT 800mV");
  testWaitForTimeout(5000);
  dllItechDcPowerWrite("OUTP 0");
  
}
```

## 🚀 Deployment <a name = "deployment"></a>

Here are steps to build this CAPL dll and configure the CAPL test module.

## ⛏️ Built Using <a name = "built_using"></a>

After change to this project's root directory, run follow command to build this CAPL dll.

```
make
```

No test code has been added in this project.
