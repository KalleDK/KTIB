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


while 1:
    print 'Choose which command to send'
    print '1. Manual watering start/stop'
    print '2. Open I valve'
    print '3. Close I valve'
    print '4. Open O valve'
    print '5. Close O valve'
    var = raw_input('choice: ')
    if var == '1':
	send('MWSTART 1\r\n')
    elif var == '2':
	send('IVALVEOPEN 1\r\n')
    elif var == '3':
	send('IVALVECLOSE 1\r\n')
    elif var == '4':
	send('OVALVEOPEN 1\r\n')
    elif var == '5':
	send('OVALVECLOSE 1\r\n')
	"""
    send(raw_input('Tekst at sende: '))
    r = s.recv(1024)
    if r:
        print 'Recieving: %s' % r
	"""