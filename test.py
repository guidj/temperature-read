import subprocess
import datetime
import json
from interval import repeat

tagFile = open("sample.log", 'r')

#last 30 records only
records = tagFile.readlines()
tagFile.close()

n = len(records)
limit = 30

if n < limit:
    startIndex = 0
else:
    startIndex = n - limit + 1

for i in range(startIndex, n):
    print records[i], " ", (i + 1)
    
tagFile = open("sample.log", 'w')

for i in range(startIndex, n):
    tagFile.write(records[i])
    
if n == 0:
    tagFile.write("23 1 2014 16 46 1")

tagFile.close()


print "Number of records: ", n