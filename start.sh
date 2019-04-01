#!/bin/bash

docker run --cap-add=SYS_PTRACE --security-opt seccomp=unconfined -it --rm life ./life
