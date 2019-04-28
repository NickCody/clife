# Conway's Game of Life in C

On a Linux system with gcc, just:

    cd src
    make
    ./life

Or, run via docker:

    docker build -t life . && docker run -it --rm life ./life

## CLI

Specify rows/cols:

    ./life -c 40 -r 40

Load in file as starter board:

    ./life -c 10 -r 10 cruiser-10x10.txt
