import sys
import time
import socket

try:
    port = int(sys.argv[1])
except (IndexError, ValueError):
    port = 5555

try:
    kar_id = int(sys.argv[2])
except (IndexError, ValueError):
    kar_id = 1

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
    print '6. OE Sensor Read'
    print '7. KAR sensor Read'
    print '8. RDY Read'
    print '9. OE list Read'
    print '10. Sensor type Read'
    var = raw_input('choice: ')
    
    if var == '1':
        send('MWSTART %d' % kar_id)
    elif var == '2':
        send('IVALVEOPEN %d' % kar_id)
    elif var == '3':
        send('IVALVECLOSE %d' % kar_id)
    elif var == '4':
        send('OVALVEOPEN %d' % kar_id)
    elif var == '5':
        send('OVALVECLOSE %d' % kar_id)
    elif var == '6':
        send('OEREAD %d' % kar_id)
    elif var == '7':
        send('KARREAD %d' % kar_id)
    elif var == '8':
        send('RDYREAD %d' % kar_id)
    elif var == '9':
        send('OELIST %d' % kar_id)
    elif var == '10':
        send('SENSORTYPE %d' % kar_id)
