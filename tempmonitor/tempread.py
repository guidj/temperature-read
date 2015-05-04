#!/usr/bin/python

"""
Project "Thermal-Aware Grid" (TAG)
High Performance Computing Services Center
Universiti Teknologi PETRONAS

File: tempread.py
Purpose:
This script calls a room temperature reading application at regular intervals and saves
the logged temperature, along with the date, time, and availability of the host given
said temperature.

Author: Guilherme Dinis Jr.
Last Revision:
"""

import subprocess
import datetime
import os
import os.path

from interval import repeat
from config import *


def temp_read():

    date = datetime.datetime.now()

    p = subprocess.Popen([TEMP_READ_BINARY, "-u", "C"], stdout=subprocess.PIPE, shell=False)

    output, err = p.communicate()

    log_file = open(LOG_FILE, 'a')
    availability_file = open(AVAILABILITY_FILE, 'a')
    temperature_file = open(TEMPERATURE_FILE, 'a')

    if os.path.exists(TAG_FILE):
        tag_file = open(TAG_FILE, 'r')

        records = tag_file.readlines()
        tag_file.close()

        n = len(records)
    else:

        records = list()
        n = 0

    limit = MAX_RECORDS

    if n < limit:
        start_index = 0
    else:
        start_index = n - limit + 1

    tag_file = open(TAG_FILE, 'w')

    #re-write on maxRecords - 1 records back to file, and make room for next record to reach maxTAGRecords limit
    for i in range(start_index, n):
        tag_file.write(records[i])

    if p.returncode == 1:
        #log error
        log_file.write(
            "[" + str(date.year) + "-" + str(date.month) + "-" + str(date.day) + " " + str(date.hour) + ":" + str(
                date.minute) + "]\t" + output + "\n")

    elif p.returncode == 0:

        #log temperature read
        #format: year month day hour minute second temperature
        temperature = float(output)

        #availability
        availability = 0 if temperature > THRESHOLD_TEMP else 1

        availability_msg = str(date.day) + " " + str(date.month) + " " + str(date.year) + " " + str(date.hour) + " " + \
                           str(date.minute) + " " + str(availability) + "\n"
        temperature_msg = str(date.day) + " " + str(date.month) + " " + str(date.year) + " " + str(date.hour) + " " + \
                          str(date.minute) + " " + str(temperature) + "\n"

        availability_file.write(availability_msg)
        tag_file.write(availability_msg)
        temperature_file.write(temperature_msg)

        print temperature_msg

    log_file.close()
    tag_file.close()
    availability_file.close()
    temperature_file.close()

if __name__ == "__main__":

    #run TempRead every n seconds, forever
    repeat(READ_INTERVAL, temp_read)
