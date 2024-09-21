# microbit_v1_ble_periperal_example

## Overview
- BLE Peripheral example for micro:bit v1 with nRF Connect SDK


## Environment
- [micro:bit V1.5](https://tech.microbit.org/hardware/1-5-revision/)
  - [Nordic nRF51822-QFAA-R rev 3](https://www.nordicsemi.com/Products/nRF51822)
    | item | detail |
    | ---- | ------ |
    | CORE | [Arm Cortex-M0](https://developer.arm.com/Processors/Cortex-M0) |
    |Flash ROM| 256KB |
    | RAM   | 16KB |
    | Speed | 16MHz |


- [nRF Connect for Desktop](https://www.nordicsemi.com/Products/Development-tools/nrf-connect-for-desktop/download)
- [nRF Connect SDK](https://www.nordicsemi.com/Products/Development-software/nRF-Connect-SDK) v2.4.3

- VSCode extensions
  - [nRF Connect for VS Code Extension Pack](https://marketplace.visualstudio.com/items?itemName=nordic-semiconductor.nrf-connect-extension-pack)
  - [Serial Monitor](https://marketplace.visualstudio.com/items?itemName=ms-vscode.vscode-serial-monitor)
  - [cortex-debug](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug)

- [nRF Connect for Mobile](https://www.nordicsemi.com/Products/Development-tools/nrf-connect-for-mobile)
  - Use for testing READ/WRITE BLE Characteristics

## Build

- On VSCode, Ctrl+Alt+n (nRF Connect) => Applications => Add build configuration
  - Add Build Configuration
    - Board Target :
      - Check `All`, then
        - [ ] Nordic SoC
        - [x] All 
      - Type `bbc_microbit` 
    - Press \[Build Configuration\]

OR

- nRF Connect for Desktop => Toolchain Manager => nRF Connect SDK v2.4.3 => ▽ => Open bash/Open command 
    ```sh
    west build --build-dir build . --pristine --board bbc_microbit -- -DNCS_TOOLCHAIN_VERSION=NONE
    ```
