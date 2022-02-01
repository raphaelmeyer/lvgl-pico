# LVGL/Pico Sandbox

## Getting started

- start vscode
- install recommended plugin
- click on `><` and select `Reopen in Container`
- configure and build

Plug in the RPi Pico (BOOTSEL) and copy the uf2 from the container to the RPi in a shell on the host.
Paths and container name may differ.

    docker cp <container>:/build/helloworld.uf2 /run/media/.../RPI-RP2/
