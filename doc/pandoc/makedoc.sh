#!/bin/bash
pandoc --data-dir=. --template template.latex latex-config.yml chapter1.md chapter2.md chapter3.md wifi-config.md resources.md -f markdown -t latex -s -o esp32websocket.tex

