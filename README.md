# C-ASM 2024 projekt

## Kompileerimine
Jooksutada `make`. Binaarfail ilmub kohta `build/app`.

## Interpretaator
Käivitada binaarfail, käsureal esimese argumendina Brainfucki programm.

Näide: `./build/app '+[-->-[>>+>-----<<]<--<---]>-.>>>+.>>..+++[.>]<<<<.+++.------.<<-.>>>>+.'`.

## Assemblerisse translaator
Käivitada binaarfail, käsureal esimene argument Brainfucki programm, teine argument `--print` või `-p`.
Assembleri kood väljastatakse standardväljundisse.

## Assembleri koodi kompileerimine
Vaja linkida `bf_mem.c.o` failiga, mis ilmub kompileerides kohta `build/src/bf_memory.c.o`.
