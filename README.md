#Temperature Reader

This is a tool to read log temperature reads, and measured availability based on a configured threshold temperature.

It has an accompanying tool to read temperatures from [temperature@lert's USB](http://www.temperaturealert.com/Wireless-Temperature-Store/Temperature-Alert-USB-Sensor.aspx) monitor.

##Usage

1.  Compile executables:
```sh
cd tempread
make
```
The binary files will be created on a `bin/` folder.
2.  Configure parameters, by editing the [config.py](tempmonitor/config.py) file.  See the [configuration](#configuration) section.
3.  Run Reader:
```sh
python tempmonitor/tempread.py
```

##Configuration


**READ_INTERVAL**: In seconds
**LOG_FILE**: App error logs
**AVAILABILITY_FILE**: Availability log file, based on threshold temperature
**TEMPERATURE_FILE**: Temperature log output
**TAG_FILE**: Keeps the latest MAX_RECORDS temperature logs 
**MAX_RECORDS**: Max number of temperature logs to keep in TAG_FILE
**THRESHOLD_TEMP**: A read below this value indicates availability
**TEMP_READ_BINARY**: Location of binary/executable that outputs temperature reads
