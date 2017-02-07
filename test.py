import subprocess
import os
import sys

# REQUIRES LIBRARY TO RUN
import psutil



def get_output_of_shell_command(cmd):
  p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
  out, err = p.communicate()
  if err:
    return err
  return out

def run_background_process(cmd):
  os.system(cmd)

def main():

  # Make and get output
  print get_output_of_shell_command("make")

  # Run the webserver in the background
  run_background_process("./webserver demo/nginx.conf &")

  # GET the HTTP response for echo
  print "TEST ECHO SERVER\n"
  http_req = "GET http://localhost:3000/echo"
  http_res = get_output_of_shell_command(http_req)

  # Print for clarity
  print "HTTP request: \n"
  print http_req + "\n\n"
  print "HTTP response: \n"
  print http_res

  # Check for correct host and port
  if "localhost:3000" in http_res:
    print "Success! 1 out of 2 tests passed!"
  else:
    print "Error, check output: \n" + http_res
    sys.exit(1)

  # GET the HTTP response for index.html
  print "TEST FILE SERVER\n"
  http_req = "GET http://localhost:3000/"
  http_res = get_output_of_shell_command(http_req)

  # Print for clarity
  print "HTTP request: \n"
  print http_req + "\n\n"
  print "HTTP response: \n"
  print http_res

  # Check for correct host and port
  if "CS130 is the best class ever" in http_res:
    print "Success! 2 out of 2 tests passed!\n"
  else:
    print "Error, check output: \n" + http_res
    sys.exit(1)


  # Kill webserver
  PROCNAME = "webserver"

  for proc in psutil.process_iter():
    if proc.name() == PROCNAME:
      print "Webserver process found, killing process..."
      proc.kill()
      print "Process killed. Exiting..."
      sys.exit()


  print "Could not find webserver process...\nexiting..."
  sys.exit(1)


if __name__ == "__main__":
  main()
