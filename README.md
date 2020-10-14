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
Python 3.6.9(Used with document generation)
wavedrompy 2.0.3(Used with document generation)
plantuml.jar(Used with document generation)
pandoc 1.19.6.2.4(Used with document generation)
Inkscape (Used with document generation)
Google Test v1.8.0

# Setup
- Install Vivado.

- Install CMake and NInja
```{class="prettyprint lang-sh"}
apt install cmake ninja-build
```

- Clone vivado_cmake_helper. The path can be anywhere.
```{class="prettyprint lang-sh"}
git clone https://github.com/akira-nishiyama/vivado_cmake_helper ~
```

- Install Google Test
```{class="prettyprint lang-sh"}
git clone https://github.com/google/googletest.git <path-to-gtest-repo>
source <vivado-installation-path>/settings64.sh
vivado_hls -i
cd <path-to-gtest-repo>
git checkout release-v1.8.0
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=<path-to-gtest-install-dir>
make
make install
```

- Install document generation related programs
```{class="prettyprint lang-sh"}
apt install python3 pandoc inkscape
pip3 install wavedrompy
```

- Download plantuml.jar from https://plantuml.com/download.
  Save it in a directory that is in your path or in a repository top.

- get source
```{class="prettyprint lang-sh"}
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

# Test
from repository top, just do following.
```{class="prettyprint lang-sh"}
mkdir build
cd build
cmake .. -GNinja -DGTEST_ROOT=<path-to-gest-install-dir>
ninja elaborate_all
ctest
```

# Document
from repository top, just do following and the document is generated in build/docs
```{class="prettyprint lang-sh"}
mkdir build
cd build
cmake .. -GNinja -DGTEST_ROOT=<path-to-gest-install-dir>
ninja docs
```

# Project Structure
This project consists of three HLS submodules and one blockdesign with rtl module and xilinx ip.
The blockdesign is recreated by tcl script exported by vivado 2019.2.
Once build this project, You can get each project in your build folder.(and use them for development.)

# License
This software is released under the MIT License, see LICENSE.

# Register Map
Under construction.
