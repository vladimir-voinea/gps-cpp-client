#!/bin/bash

docker run --rm -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY -e WAYLAND_DISPLAY -e XDG_RUNTIME_DIR -e PULSE_SERVER -v /mnt/wslg:/mnt/wslg -it gstreamer_app
