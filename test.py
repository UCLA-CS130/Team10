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
  run_background_process("./webserver demo/example.conf &")

  # GET the HTTP response
  http_req = "GET http://localhost:3000"
  http_res = get_output_of_shell_command(http_req)

  # Print for clarity
  print "HTTP request: \n"
  print http_req + "\n\n"
  print "HTTP response: \n"
  print http_res

  # Check for correct host and port
  if "localhost:3000" in http_res:
    print "Success! 1 out of 1 tests passed"
  else:
    print "Error, check output: \n" + http_res 
    sys.exit(1)


  # Kill webserver
  PROCNAME = "webserver"

  for proc in psutil.process_iter():
    if proc.name() == PROCNAME:
      print "Webserver process found, killing process..."
      proc.kill()
      sys.exit()


  print "Could not find webserver process...\nexiting..."
  sys.exit(1)


if __name__ == "__main__":
  main()
