'''
	udp socket client， , code from https://www.binarytides.com/programming-udp-sockets-in-python
	Silver Moon
'''

import socket	#for sockets
import sys	#for exit

# create dgram udp socket
try:
	s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
except socket.error:
	print('Failed to create socket')
	sys.exit()

host = '192.168.13.66'
port = 50008

while(1) :
	msg = input('Enter message to send : ')
	
	try :
		#Set the whole string
		s.sendto(msg.encode('utf-8'), (host, port))
		
		# receive data from client (data, addr)
		d = s.recvfrom(1024)
		reply = d[0]
		addr = d[1]
		
		print(b'Server reply : ' + reply)
	
	except socket.error as msg:
		print('Error Code : ' + str(msg[0]) + ' Message ' + msg[1])
		sys.exit()
