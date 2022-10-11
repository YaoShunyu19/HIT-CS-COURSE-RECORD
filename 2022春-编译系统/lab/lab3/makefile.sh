#!/bin/bash
bison -d syntax.y
flex lexical.l
gcc syntax.tab.c interim.c semantic.c main.c -lfl -o parser
