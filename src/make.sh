#!/usr/bin/env bash
g++ -c ArgParser.cpp;
g++ -c main.cpp;
g++ -o main main.o ArgParser.o;