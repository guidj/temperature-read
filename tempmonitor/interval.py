#!/usr/bin/python

import threading


def repeat(interval, worker_function, iterations=0):
    if iterations != 1:
        threading.Timer(interval, repeat, [interval, worker_function, 0 if iterations == 0 else iterations - 1]).start()

    worker_function()
