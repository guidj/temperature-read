__author__ = 'guilherme'

import os
import os.path

PROJECT_BASE = ''.join([os.path.dirname(os.path.abspath(__file__)), "/../"])

READ_INTERVAL = 2
LOG_FILE = "tempread.log"
AVAILABILITY_FILE = "availability.log"
TEMPERATURE_FILE = "temperature.log"
TAG_FILE = "tag.log"
MAX_RECORDS = 30
THRESHOLD_TEMP = 28
TEMP_READ_BINARY = os.path.join(PROJECT_BASE, os.path.join("bin", "fakereader"))