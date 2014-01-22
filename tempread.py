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
import json
from interval import repeat

global thresholdTemp
global logFileName
global availabilityFileName
global temperatureFileName

def loadConfiguration():
	configFile = "config.json"
	f = open(configFile, 'r')

	configString = f.read()
	config = json.loads(configString)
	return config


def TempRead():

	cmd = "./tempread -u C"
	date = datetime.datetime.now()

	p = subprocess.Popen(["./tempread", "-u", "C"], stdout=subprocess.PIPE, shell=False)

	output, err = p.communicate()

	#print "error : ", err
	#print "output: ", output
	#print "return: ", p.returncode

	logFile = open(logFileName,'a')
	availabilityFile = open(availabilityFileName, 'a')
	temperatureFile = open(temperatureFileName, 'a')

	if (p.returncode == 1):
		#log error
		logFile.write("[" + str(date.year) + "-" + str(date.month) + "-" + str(date.day) + " " + str(date.hour) + ":" + str(date.minute) + "]\t" + output + "\n")
		#print ("[" + str(date.year) + "-" + str(date.month) + "-" + str(date.day) + " " + str(date.hour) + ":" + str(date.minute) + "]"), output
	elif (p.returncode == 0):
		#log temperature read
		#format: year month day hour minute second temperature
		temperature = float(output)

		#availability
		availability = 0 if (temperature > thresholdTemp ) else 1
		availabilityFile.write(str(date.day) + " " + str(date.month) + " " + str(date.year) + " " +  str(date.hour) + " " + str(date.minute) + " " + str(availability) + "\n")
		temperatureFile.write(str(date.day) + " " + str(date.month) + " " + str(date.year) + " " +  str(date.hour) + " " + str(date.minute) + " " + str(temperature) + "\n")
		#print (str(date.day) + "-" + str(date.month) + "-" + str(date.year) + "\t" +  str(date.hour) + " " + str(date.minute) + "\t" + str(availability))


	logFile.close()
	availabilityFile.close()
	temperatureFile.close()

#load configuration
config = loadConfiguration()

readInterval = float(config["readInterval"])
thresholdTemp = float(config["thresholdTemp"])
logFileName = config["logFile"]
availabilityFileName = config["availabilityFile"]
temperatureFileName = config["temperatureFile"]


#run TempRead everty 2 seconds, forever
repeat(readInterval, TempRead)
#TempRead()
