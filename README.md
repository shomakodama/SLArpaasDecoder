# SLArpaas Decoder

## Overview

This code is written and maintained by **Shoma Kodama** (The University of Tokyo; `shoma@hep.phys.s.u-tokyo.ac.jp`).

This is for data taken with SLArpaas_test16_v1.NIU or SLArpaas_test16_v2.NIU.

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

</details>





