Lab 3 — Page Replacement Simulator
----------------------------------

Author: Eduardo Galvez
Language: C (C17)
Environment: Linux (tested on Ubuntu via WSL and macOS)

Build Instructions:
    make         # compiles memsim
    make clean   # removes objects and binary

Full Run Examples:
    ./memsim 128  pg-reference.txt  out_128.txt
    ./memsim 256  pg-reference.txt  out_256.txt
    ./memsim 512  pg-reference.txt  out_512.txt
    ./memsim 1024 pg-reference.txt  out_1024.txt

Files:
    main.c / pager.c,h        — simulator control and frame-table logic
    parser.c,h                — input parser for 10,000 page references
    print.c,h                 — formatted table generation
    fifo.c,h                  — FIFO page replacement algorithm
    lru.c,h                   — LRU page replacement algorithm
    opt.c,h                   — OPT (Optimal) page replacement algorithm
    Makefile                  — build rules
    pg-reference.txt          — reference string used for testing

Sample outputs:
    out_128.txt, out_256.txt, out_512.txt, out_1024.txt
