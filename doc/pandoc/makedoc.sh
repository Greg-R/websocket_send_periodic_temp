#!/bin/bash
pandoc latex-config.yml chapter1.md chapter2.md chapter3.md -f markdown -t latex -s -o esp32websocket.tex

