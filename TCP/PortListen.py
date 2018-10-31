# Filename: PortListen.py
# @author: CharlesYoung
# Created on 2018-10-15


"""TCPSever:

TCPServer IP Address       : 101.132.151.237
TCPServer IP Port          : 80 for self-bala car, 81 for remote controllers
Login info for controllers : 'bala_control 2222'
	(The controllers need to send this string to the server in 15 sec after connection)
"""

"""Protocol:

| recv         | send         | meaning                    |
| ------------ | ------------ | -------------------------- |
| ###          | #            | show parameters            |
| #K<n>#<val># | <n><val>\r\n | set parameter <n> to <val> |

Notes: 
1. 'recv' refers to string received from remote controllers (PC, phone, etc)
2. 'send' refers to string sent to self-balance car 
3. Mapping:
	K1 -> Balance_Kp [-100 100]
	K2 -> Balance_Kd [
	K3 -> Velocity_Kp 
	K4 -> Velocity_Ki [-10 10]
	K5 -> Velocity_Kd [-20 20]

eg. If receive #K2#0.2#, then send 20.2 to set parameter 2 (Balance_Kd) to 0.2.

"""
 
import threading
import socket
import argparse
import logging
from MysqlHelper import MysqlHelper
import getpass
from hashlib import sha256
 
encoding = 'utf-8'
BUFSIZE = 1024

password = ''

parser = argparse.ArgumentParser()
parser.add_argument('--port1', type = int, help = "Port 1.")
parser.add_argument('--port2', type = int, help = "Port 2.")
parser.add_argument('--log', default = './logs/communication.log', help = "Logging file.")
args = parser.parse_args()

logging.basicConfig(format='%(asctime)s %(levelname)s:%(message)s', filename = args.log, filemode = 'w', level = logging.INFO)

users_list = []
txBuffer = [dict(), dict()]

lock = threading.Lock()

def checkcmd_Bala(cmd):
	splits = cmd.strip().upper().split()[0].split('#')

	para_list = ['K1', 'K2', 'K3', 'K4', 'K5']

	if len(splits) == 4 and all(i == '' for i in splits):
		return b'#'
	if len(splits) < 1 or len(splits) != 4 or splits[0] != '' or splits[1] not in para_list:
		return b'0'
	else:
		try:
			val = float(splits[2])
			cmd_out = str(para_list.index(splits[1]) + 1) + str(val) + '\n\r'
			return bytes(cmd_out, encoding = 'utf8')
		except:
			pass
	return b'0'


def sockclient_send(port, client, cltadd, ports, username):
	global txBuffer
	while True:
		try:
			lock.acquire()
			if client.fileno() == -1:
				raise socket.timeout
			while txBuffer[ports.index(port)][cltadd]:
				data = txBuffer[ports.index(port)][cltadd][0]
				txBuffer[ports.index(port)][cltadd].pop(0)
				if username == 'Bala':
					send_data = checkcmd_Bala(bytes.decode(data, encoding))
					if send_data != b'0':
						logging.info('Port %d sends message to user %s (%s, %s): %s ', port, username, cltadd[0], cltadd[1], bytes.decode(send_data, encoding))
						client.send(send_data)					
					else:
						logging.info('Prevent invalid command sending to user Bala: %s ', bytes.decode(data, encoding))
				else:
					logging.info('Port %d sends message to user %s client (%s, %s): %s ', port, username, cltadd[0], cltadd[1], bytes.decode(data, encoding))
					client.send(data)
			lock.release()	
		except:
			lock.release()
			logging.error('Port %d\' client (%s, %s) encounters an error while sending! ', port, cltadd[0], cltadd[1])
			break
	logging.info('Port %d\'s client (%s, %s)\' sender closed. ', port, cltadd[0], cltadd[1])
	print('Port {0}\'s client ({1}, {2})\' sender closed. '.format(port, cltadd[0], cltadd[1]))
	client.close()	
	if username in users_list:
		users_list.remove(username)
	txBuffer[ports.index(port)].pop(cltadd, None)


def sockclient_recv(port, client, cltadd, ports, username):
	global users_list, txBuffer
	while True:
		try:
			data = client.recv(BUFSIZE)
			if data:
				string = bytes.decode(data, encoding)
				if string.upper() == 'EXIT':
					break
				logging.info('Port %d receives from client (%s, %s) with usename %s: %s ', port, cltadd[0], cltadd[1], username, string)
				print(string)
				lock.acquire()
				for add in txBuffer[1 - ports.index(port)]:
					txBuffer[1 - ports.index(port)][add].append(data)
				lock.release()
			else:
				break
		except:
			logging.error('Port %d\' client (%s, %s) encounters an error while receiving! ', port, cltadd[0], cltadd[1])
			break
	logging.info('Port %d\'s client (%s, %s)\' receiver closed. ', port, cltadd[0], cltadd[1])
	print('Port {0}\'s client ({1}, {2})\' receiver closed. '.format(port, cltadd[0], cltadd[1]))
	client.close()
	if username in users_list:
		users_list.remove(username)
	txBuffer[ports.index(port)].pop(cltadd, None)


def sockthread(port, sock, ports):
	global users_list, txBuffer
	print("Port {}\'s listener started.".format(port)) 
	while True:
		client, cltadd = sock.accept()
		logging.info('Port %d accepts a connect: (%s, %s). ', port, cltadd[0], cltadd[1])
		print('Port {0} accepts a connect: ({1}, {2}). '.format(port, cltadd[0], cltadd[1]))
		username = ''
		client.settimeout(15)
		try:
			userinfo = client.recv(BUFSIZE).decode(encoding).strip().split()
			if len(userinfo) == 2:
				if port == 80 and userinfo[0] != 'Bala': 
					raise socket.timeout
				params = [userinfo[0]]
				encrypt = sha256()
				encrypt.update(userinfo[1].encode('utf8'))
				upwd_encrypt = encrypt.hexdigest()
				result = MysqlHelper(password).fetchone('SELECT password FROM verifiedID WHERE username = %s ', params)
				if result == None or result[0] != upwd_encrypt:
					raise socket.timeout
				else:
					client.settimeout(None)
					username = userinfo[0]
					logging.info('Port %d\'s client (%s, %s) login by username: \'%s\'. ', port, cltadd[0], cltadd[1], username)
					print('Port {0}\'s client ({1}, {2}) login by username: \'{3}\'. '.format(port, cltadd[0], cltadd[1], username))
					lock.acquire()
					users_list.append(username)
					txBuffer[ports.index(port)][cltadd] = []
					lock.release()
			else:
				raise socket.timeout
		except:
			logging.info('Port %d\'s new client (%s, %s) failed to login！ ', port, cltadd[0], cltadd[1])
			print('Port {0}\'s client ({1}, {2}) failed to login! '.format(port, cltadd[0], cltadd[1]))
			client.close()
			continue

		threading.Thread(target = sockclient_recv, args = (port, client, cltadd, ports, username)).start() 
		threading.Thread(target = sockclient_send, args = (port, client, cltadd, ports, username)).start() 


if __name__ == '__main__':

	password = getpass.getpass(prompt = 'Please input your password to login in MySQL server: ')
	while True:
		try:
			helper = MysqlHelper(password)
			break
		except:
			print('Password error!')
			password = getpass.getpass(prompt = 'Please input again: ')
	
	port1 = args.port1
	sock1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock1.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	sock1.bind(("0.0.0.0", port1))
	sock1.listen(2)

	port2 = args.port2
	sock2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock2.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	sock2.bind(("0.0.0.0", port2))
	sock2.listen(4)

	ports = [port1, port2]

	threading.Thread(target = sockthread, args = (port1, sock1, ports)).start()
	threading.Thread(target = sockthread, args = (port2, sock2, ports)).start()
