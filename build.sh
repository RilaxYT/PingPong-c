#!/bin/bash

# Tworzy katalog build jeśli nie istnieje
mkdir -p build

# Kompiluj termbox.c do obiektu
cc -c libs/termbox/termbox.c -o build/termbox.o

# Kompiluj termbox.c do obiektu
cc -c libs/termbox/utf8.c -o build/utf8.o

# Kompiluj main.c do obiektu
cc -c src/main.c -o build/main.o

# Linkuj obiekty w finalny program
cc build/main.o build/termbox.o build/utf8.o -o build/main
