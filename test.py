import subprocess
import os
import sys
from threading import Thread
import socket
import time


# REQUIRES LIBRARY TO RUN
import psutil

ROOT_PATH = "demo/"
ECHO_PATH = "/echo"
STATIC_PATH = "/static/"
STATUS_PATH = "/status"
PROXY_PATH = "/proxy/static/"
CONFIG_FILE_NAME = "config.conf"
CONFIG_FILE_NAME2 = "config2.conf"

def get_output_of_shell_command(cmd):
  p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
  out, err = p.communicate()
  if err:
    return err
  return out

def run_background_process(cmd):
  os.system(cmd)

def kill_server(exit_code):
    # Kill webserver
    PROCNAME = "webserver"

    for proc in psutil.process_iter():
      if proc.name() == PROCNAME:
        print "Webserver process found, killing process..."
        proc.kill()

    print "Exiting..."
    sys.exit(exit_code)

def test(name, path, test_string, port):
    print name
    http_req = "GET http://localhost:" + port + path
    http_res = get_output_of_shell_command(http_req)

    # Print for clarity
    print "HTTP request: \n"
    print http_req + "\n\n"
    print "HTTP response: \n"
    print http_res


    if test_string in http_res:
      print "Single thread test: Success!\n"
    else:
      print "Error, check output: \n" + http_res
      kill_server(1)


def setup_socket():
  s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
  return s

def connect(error, isPaused, threadID):
  TCP_IP = '0.0.0.0'
  TCP_PORT = 3000
  BUFFER_SIZE = 1024
  if isPaused:
    MESSAGE = "GET /echo HTTP/1.0"
  else:
    MESSAGE = "GET /echo HTTP/1.0\r\n\r\n"

  s = setup_socket()
  s.connect(('localhost', TCP_PORT))
  print("Thread {} is connecting to the server...\n".format(threadID))
  print("Thread {} sends a request.\n".format(threadID))
  s.send(MESSAGE)
  if isPaused:
    time.sleep(5)
    print("Thread {} completes the request.\n".format(threadID))
    s.send("\r\n\r\n")
  data = s.recv(BUFFER_SIZE)
  s.close()
  print("Thread {} finished connection...\n".format(threadID))

  if (data != "HTTP/1.0 200 OK\r\nContent-Length: 22\r\nContent-Type: text/plain\r\n\r\nGET /echo HTTP/1.0\r\n\r\n"):
    print "Failure response: ", repr(data)
    error_code = 1
    return

  error_code = 0

  return

def main():

  # Make and get output
  print get_output_of_shell_command("make")

  # Run the webserver in the background
  run_background_process("./webserver " + ROOT_PATH + CONFIG_FILE_NAME + " &")

  # GET the HTTP response for echo
  test("ECHO HANDLER", ECHO_PATH, "localhost:3000", "3000")

  # GET the HTTP response for index.html
  test("STATIC HANDLER", STATIC_PATH, "CS130 is the best class ever", "3000")

  # GET the HTTP response for status
  test("STATUS HANDLER", STATUS_PATH, "Number of connections: 2\n/echo: 200\n/static/: 200", "3000")

  # GET the HTTP 404 response
  test("404 HANDLER", STATIC_PATH + "/dummy.html", "404 Not Found", "3000")

  # Run new webserver for proxy handling
  run_background_process("./webserver " + ROOT_PATH + CONFIG_FILE_NAME2 + " &")

  # GET the response for index.html
  test("PROXY HANDLER", PROXY_PATH, "CS130 is the best class ever", "3001");

  # test multithread
  # Thread0 should connects to the server first and sends a partial request.
  # Thread0 then hangs there for 5 seconds and waits for comming bytes to
  # complete the request. Then Thread1 makes a connection, send a request
  # and get responded and return. Then Thread0 receives a complete request.
  # Then it gets response and return.
  print "\nMultithread test begins\n"
  error_code_0 = 0
  error_code_1 = 0
  pause0 = 1
  pause1 = 0
  threadID0 = 0
  threadID1 = 1
  
  paused_connection = Thread(target = connect, args=(error_code_0, pause0, 0))
  connection = Thread(target = connect, args=(error_code_1, pause1, threadID1))


  paused_connection.start()
  connection.start()

  connection.join()
  paused_connection.join()

  if(error_code_0 == 0 and error_code_1 == 0):
    print "\n\nMultithread TEST: SUCCESS\n"
  else:
    print "\n\nMultithread TEST: FAIL\n"

  kill_server(0)


if __name__ == "__main__":
  main()