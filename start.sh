#!/bin/bash

docker run --cap-add=SYS_PTRACE --security-opt seccomp=unconfined -it --rm -v $(pwd):/home/nic/life life
