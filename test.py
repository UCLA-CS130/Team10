import subprocess
import os
import sys

# REQUIRES LIBRARY TO RUN
import psutil

ROOT_PATH = "demo/"
ECHO_PATH = "/echo"
STATIC_PATH = "/static/"
STATUS_PATH = "/status"
CONFIG_FILE_NAME = "config.conf"

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
        print "Process killed. Exiting..."
        sys.exit(exit_code)

    print "Could not find webserver process...\nexiting..."
    sys.exit(1)

def test(name, path, test_string):
    print name
    http_req = "GET http://localhost:3000" + path
    http_res = get_output_of_shell_command(http_req)

    # Print for clarity
    print "HTTP request: \n"
    print http_req + "\n\n"
    print "HTTP response: \n"
    print http_res


    if test_string in http_res:
      print "Success!\n"
    else:
      print "Error, check output: \n" + http_res
      kill_server(1)

def main():

  # Make and get output
  print get_output_of_shell_command("make")

  # Run the webserver in the background
  run_background_process("./webserver " + ROOT_PATH + CONFIG_FILE_NAME + " &")

  # GET the HTTP response for echo
  test("ECHO HANDLER", ECHO_PATH, "localhost:3000")

  # GET the HTTP response for index.html
  test("STATIC HANDLER", STATIC_PATH, "CS130 is the best class ever")

  # GET the HTTP response for status
  test("STATUS HANDLER", STATUS_PATH, "Number of connections: 2\n/echo: 200\n/static/: 200")

  # GET the HTTP 404 response
  test("404 HANDLER", STATIC_PATH + "/dummy.html", "404 Not Found")

  kill_server(0)


if __name__ == "__main__":
  main()
