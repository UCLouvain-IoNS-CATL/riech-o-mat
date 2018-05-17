#!/usr/bin/env bash

# For me, to have clang completion in Vim:
#~/.vim/bin/cc_args.py gcc -W -Wall -o riech-o-mat-backend-serial -g $(pkg-config --cflags --libs glib-2.0) riech-o-mat-backend-serial.c

gcc -W -Wall -o riech-o-mat-backend-serial -g $(pkg-config --cflags --libs glib-2.0) riech-o-mat-backend-serial.c
ln -s riech-o-mat-backend-serial riech-o-mat-backend
