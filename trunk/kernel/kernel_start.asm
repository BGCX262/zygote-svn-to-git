[BITS 32]
[global start]
[extern _k_main] ; this is in the c file

start:
  call _k_main

