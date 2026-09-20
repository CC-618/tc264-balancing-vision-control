# TC264 Balancing and Vision Control

Infineon AURIX TC264D firmware for a two-wheel balancing robot. The project combines dual-core control tasks with inertial sensing, PID loops, motor feedback, camera line tracking, and wireless/TCP telemetry.

## Features

- Two-wheel balance and motion control
- IMU sampling and attitude estimation
- Cascaded PID control and encoder feedback
- Camera-based line detection and tracking
- Motor driver and peripheral control through the Infineon iLLD
- Wi-Fi/TCP communication for telemetry or remote control

## Project layout

- `4.12/Seekfree_TC264_Opensource_Library/code/` - Application code for balance, motion, camera, filters, and initialization
- `4.12/Seekfree_TC264_Opensource_Library/libraries/` - Infineon iLLD and SeekFree support libraries
- `4.12/Seekfree_TC264_Opensource_Library/Debug/` - Debug/build project sources
- `.project` and `.cproject` - Eclipse/AURIX Development Studio project metadata

## Build

Open `4.12/Seekfree_TC264_Opensource_Library/` as an AURIX Development Studio project, select the appropriate TC264D target, then build and download it with a compatible debugger.

## Configuration notes

The source contains board-specific pin mappings, control gains, and network settings. Review the Wi-Fi SSID, password, destination IP, and port before using the communication features on a different network.

The bundled Infineon and SeekFree libraries retain their respective license and attribution terms.
