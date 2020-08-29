#!/usr/bin/python3

import socket
import sys

HOST='127.0.0.1'    # localhost
PORT=1234        # Port


with socket.socket(family=socket.AF_INET, type=socket.SOCK_STREAM) as s:
    s.connect((HOST,PORT))
    s.sendall(b'Hello World')
    data=s.recv(1024)

print('Recieved:',repr(data))
