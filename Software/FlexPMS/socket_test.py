import sys
import time
import socket

try:
    port = int(sys.argv[1])
except (IndexError, ValueError):
    port = 5555

print 'Binding on port %d' % port

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("localhost", port))


def send(st):
    s.send('%s\r\n' % st)

send('MWSTATUS')

while 1:
    r = s.recv(1024)
    if r:
        print 'Recieving: %s' % r
    send('')
        