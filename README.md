
# ORCA-SOFTWARE-TOOLS

ORCA-SOFTWARE-TOOLS is a set of software artifacts that serves as basis for the development of applications for the ORCA MPSOC. This repository includes a copy of the HellfireOS, examples applications, and other integration tools. 

- HellfireOS. This repository has a copy of the HellfireOS, a fully-preemptive real-time operating system.
- Applications. Several applications are provided in the software folder. 
- Extensions. Some extensions to permit using hardware monitors and counters, the publish-subscribe communication model, and static task mapping.
- Tools. Provide integration tools for off-chip communication and integration with the ROS system.

Related repositories are enlisted below.

- ORCA-MPSOC: https://github.com/andersondomingues/orca-mpsoc
- ORCA-SIM: https://github.com/andersondomingues/orca-sim

## Quick Start

1) Clone this repository
2) Go to the root folder and type ``$make`` in the terminal (Make and RiscV toolchain required!)
3) The generated ``image.bin`` will be available in the ``bin`` folder.

## Project organization

- ``/bin`` : compiled binaries and compilation artifacts
- ``/docs`` : tutorials and guidance resources
- ``/logs`` : output logs for each individual processor core
- ``/models`` : hardware models (processor core, memory cores, etc.)
- ``/platforms`` : top-level platform models (for both many-core and single-core platforms)
- ``/simulator`` : URSA's core
- ``Makefile`` : Make's script. Do not modifiy it unless you know make language well.
- ``Configuration.mk`` : Change this script to reflect the intended project configuration (comments on the parameters are inside the file)


## Project Roadmap

Things that we are likely to work on in the next months:

- Provide an API to use for energy evaluation of applications
- Unit tests on software (mainly for HellfireOS, see (https://github.com/andersondomingues/hellfireos)
- Support for C++ apps 
- Rework task mapping and wiring (code can be cumbersome when dealing with a large number of signals)
- Provide some IDE-specific projects (hopefully they won't inject any dirt in the project)
- Stub code generator 
- Update HellfireOS to match a more stable and recent branch (see (https://github.com/sjohann81/hellfireos)
- DOOM (see https://github.com/lcq2/risc-666)
- Decent tutorials 

## Licensing

This is free software! See ``LICENSE.MD`` for details. 

## Contact

Feel free to contact me ([andersondomingues](https://github.com/andersondomingues)), the maintainer of this project: mailto:ti.andersondomingues@gmail.com.

## Third-Party Work and Acknowledgement

- HellfireOS. The HellfireOS kernel is maintained by Sergio Johann (sjohann81). More information on his work can be found at [his repository](https://github.com/sjohann81).

- I would to thank Mr. Alexandre Amory (https://github.com/amamory) for the fancy compilation scripts, they rock!
