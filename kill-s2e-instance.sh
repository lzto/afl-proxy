#!/bin/bash
kill -9 `fuser serial.txt | cut -d: -f2-`
