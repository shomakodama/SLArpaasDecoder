# SLArpaas Decoder

## Overview

This repository stores decoders for data taken with SLArpaas_test16_v1.NIU or SLArpaas_test16_v2.NIU.

The DAQ code is stored in the different repository.
Please refer to [SLArpaasDAQ](https://github.com/shomakodama/SLArpaasDAQ).





## Basic Usage

The code is written in **C++** used with **ROOT**.

`root -q "name of the decoder"`

Some parameters are defined in the decoder.
Click below to show parameters.

<details>
<summary>details</summary>

name|description
---|---
length|sample length
interval|clock interval (should be 8 ns for DT5560SE)
timing_offset|offset for a timing since the timing is split into 2 lines (32-bits*2, should be 0x100000000)
fname|input file name
ofilename|output file name
plotevent|saving waveform or not (1: save)

</details>

One python script `Decoder1ch.py` is just reading every line and showing it in the terminal.





## Output

The outout root file has one TTree.
The TTree has "5+number of channels" branches.
Click below to show these branches.

<details>
<summary>details</summary>

name|description
---|---
length|sample length (int)
timing|event timing (uint64_t)
event|event number counting by the counter module (uint32_t)
internalevent|event number counting by the digitizer module (uint32_t)
ADCX|data points (int)
eventflag|event flag (int)

</details>


