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
interval|clock interval (should be 8 ns)
timing_offset|timing is split into 2 lines (32-bits*2, should be 0x100000000)
fname|input file name
ofilename|output file name
plotevent|saving waveform or not (1: save)

</details>

One python script `Decoder1ch.py` is just reading all lines and showing them in the terminal.



