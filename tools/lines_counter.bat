
@echo off
cls
echo ***** ***** ***** ***** *****
echo -----------------------------------Calculate src dir-------------------------
python3 files_line_counter.py ../src

echo -----------------------------------Calculate include dir-------------------------
python3 files_line_counter.py ../include