#!/bin/bash
bash ./overlay.sh
sleep 15 

echo "Message : Application Start"
cd ..
python3.7 -u ./main.py