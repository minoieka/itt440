import socket
import errno
import sys
import os

#UDP_IP= "170.0.0.1"
#UDP_PORT= 5005
#MESSAGE= "Hello..this_is my first UDP!"
sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)

UDP_IP = ('localhost', 3000)
print>>sys.stderr, 'first %s port_is %s' % UDP_IP
sock.bind(UDP_IP)

while True:
	print>>sys.stderr, '\nWaiting the message'
	data,address = sock.recvfrom(4096)
	print>>sys.stderr, 'received %s bytes from &s '%(len(data),address)
	print>>sys.stderr, data

	if data:
		sent = sock.sendto(data,address)
		print>>sys.stderr, 'sent %s bytes back to %s' %(sent,address)

	get = sock.getsockopt(socket.SOL_SOCKET,socket.SO_SNDBUF)
	print "Get SNDBUF:",get
	#pitfall 1
	if get > 0:
		print " Already set"

	elif get < 0:
		print "Error",errno.EUSERS
		print "DESCREPTION:",os.strerror(errno.EUSERS)
		data.close(sock)
	#pitfall 2
	set = sock.setsockopt(socket.SOL_SOCKET,socket.SO_SNDBUF,1)
	print "SET SNDBUF:",sock.getsockopt(socket.SOL_SOCKET,socket.SO_SNDBUF)
	break
	#pitfall 3
