#!/usr/bin/env python

from flask import Flask, redirect, url_for, request
import sys
import os
import hashlib
import socket

app = Flask(__name__)
	

MSGLEN = 2048

@app.route('/')
def index():
	return redirect(url_for('static', filename='index.html'))

@app.route('/rd/<int:p>', methods=["GET"])
def rd_getrd(p):
	#1. Figure out the <object-name> from the request
        obj_name = request.form['object'];
        return rd_getrdpeer(p, obj_name)



@app.route('/rd/addfile/<int:p>', methods=["POST"])
def rd_addfile(p):
	#1. Figure out the object-name and the file details/content from the request
        content = request.data
        print content

        filename = request.form['object']
        
	#2. Find the sha256sum of the file content
        filehash = hashlib.sha256(content).hexdigest()
        
	#3. Save the file in the static directory under the sha256sum name and compute the relative path
        relative_path = './static/'+filehash
        f = open(relative_path, 'w')
        f.write(content)
        f.close()
	#4. Connect to the routing daemon on port p
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(("localhost", p))
	#5. Do ADDFILE <object-name> <relative-path> 
        req_msg = ('ADDFILE %d %s %d %s'% \
                           (len(filename), filename,  \
                                    len(relative_path), \
                                    relative_path))
        totalsent = 0
        msg_len = len(req_msg)
        while totalsent < msg_len:
                bytesent = s.send(req_msg[totalsent:])
                if bytesent == 0:
                        raise RuntimeError("socket connection broken")
                totalsent = totalsent + bytesent
                
	#6. Based on the response from the routing daemon display whether the object has been successfully uploaded/added or not 
        index = respd_msg.find(' ')
        if index == -1:
                raise RuntimeError("msg from routing deamon is malformated")
        respd = respd_msg[0:index]        
        respd_msg = respd_msg[index + 1:]
        
        if respd == 'OK':
                print 'content successfully uploaded'
        else:
                print 'upload file failed'

	return "Unimplemented"


@app.route('/rd/<int:p>/<obj>', methods=["GET"])
def rd_getrdpeer(p, obj_name):
	#2. Connect to the routing daemon on port p
        s = socket.socket(
                socket.AF_INET, socket.SOCK_STREAM)
        s.connect(("localhost", p))
	#3. Do GETRD <object-name> 
        req_msg = ('GETRD %d %s'%(len(obj_name), obj_name));        
        totalsent = 0
        msg_len = len(req_msg)
        while totalsent < msg_len:
                bytesent = s.send(req_msg[totalsent:])
                if bytesent == 0:
                        raise RuntimeError("socket connection broken")
                totalsent = totalsent + bytesent
        
        respd_msg = s.recv(MSGLEN)
	#4. Parse the response from the routing daemon
        index = respd_msg.find(' ')
        if index == -1:
                raise RuntimeError("msg from routing deamon is malformated")
        respd = respd_msg[0:index]        
        respd_msg = respd_msg[index + 1:]

        index = respd_msg.find(' ')
        if index == -1:
                raise RuntimeError("msg from routing deamon is malformated")
        len_str = respd_msg[0:index]
        try:
                length = int(len_str)
        except TypeError:
                print 'length from msg is not a integer'
        except:
                print 'unexpected error'
                raise
        respd_msg = respd_msg[index + 1:]
        if len(respd_msg) < length:
                raise RuntimeError("uri is not long enough")
        uri = respd_msg[0:length]
        
	#4 a) If response is OK <URL>, the open the URL
        if respd == 'OK':
                urllib.urlopen(uri)
	#4 b) If response is 404, then show that content is not available
        else:
                print('content is not available')
    #### You may factor out things from here and rd_getrd() function and form a separate sub-routine
	return "Unimplemented"



if __name__ == '__main__':
	if (len(sys.argv) > 1):
		servport = int(sys.argv[1])
		app.run(host='0.0.0.0', port=servport, threaded=True, processes=1)
	else:	
		print "Usage ./webserver <server-port> \n"
