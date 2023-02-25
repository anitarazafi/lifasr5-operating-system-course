#!/usr/bin/env python3

# telnet program example
import socket, select, string, sys

if __name__ == "__main__":

    if(len(sys.argv) < 3):
        print('Usage : telnet.py hostname port')
        sys.exit()
    
    host = sys.argv[1]
    port = int(sys.argv[2])
    
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(2)
    
    # connect to remote host
    try:
        s.connect((host, port))
    except:
        print('Unable to connect')
        sys.exit()
    
    print('Connected to remote host')
    
    while 1:
        socket_list = [sys.stdin, s]
        read_sockets, write_sockets, error_sockets = select.select(socket_list , [], [])
        for sock in read_sockets:
            if sock == s:
                data = sock.recv(4096)
                if not data :
                    print('Connection closed')
                    sock.close()
                    s.close()
                    sys.exit()
                else :
                    sys.stdout.write(data.decode('utf-8'))
            else :
                msg = sys.stdin.readline()
                s.send(msg.encode('utf-8'))

