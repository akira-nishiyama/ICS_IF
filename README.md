# ICS_IF Overview
Serial servomotor control IP.  Provides Interactive Communication System(ICS) Interface to your FPGA.
ICS spec is [here](https://kondo-robot.com/faq/ics3-5_3-6softwaremanual_2)
For now this repository showing an example of [vivado_cmake_helper](https://github.com/akira-nishiyama/vivado_cmake_helper) usage.
The functions are under construction.

# Dependency
Vivado 2019.2
Vivado_HLS 2019.2
CMake 3.1 or later
Ninja(Make also works)
vivado_cmake_helper

# Setup
- Install Vivado.

- Install CMake and NInja
```bash
apt install cmake ninja-build
```

- Clone vivado_cmake_helper. The path can be anywhere.
```bash
git clone https://github.com/akira-nishiyama/vivado_cmake_helper ~
```

- get source
```bash
git clone https://github.com/akira-nishiyama/ICS_IF
```

# Installation
To use this ip in your FPGA project,
You have to import all ip created by this repository into user_ip_repo.
(need: ics_if_tx, ics_if_rx, ics_if_main, ics_if)
Below command automatically install all need artifacts into your user_ip_repo.

```bash
source <vivado_installation_path>/setup.sh
source ~/vivado_cmake_helper/setup.sh
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=~/user_ip_repo -GNinja
cmake --build .
ninja install
```

# Project Structure
This project consists of three HLS submodules and one blockdesign with rtl module and xilinx ip.
The blockdesign is recreated by tcl script exported by vivado 2019.2.
Once build this project, You can get each project in your build folder.(and use them for development.)

# License
This software is released under the MIT License, see LICENSE.

# Register Map
Under construction.

