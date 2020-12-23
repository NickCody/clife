#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

docker build -t life -f .devcontainer/Dockerfile .
docker run -it --rm -v $SCRIPT_DIR:/workspaces/clife -w /workspaces/clife life "./run-container.sh"