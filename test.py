import subprocess
import os

def get_output_of_shell_command(cmd):
  p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
  out, err = p.communicate()
  if err:
    return err
  return out

def run_background_process(cmd):
  os.system(cmd)


def main():

  print get_output_of_shell_command("make")
  run_background_process("./webserver demo/example.conf &")
  print get_output_of_shell_command("GET http://localhost:3000")

  # TODO:
  # 1. Silence output to console
  # 2. Compare output with desired test outcome
  # 3. Output integration test result

if __name__ == "__main__":
  main()
