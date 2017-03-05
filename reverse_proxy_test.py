import subprocess
import os
import sys

# REQUIRES LIBRARY TO RUN
import psutil

ROOT_PATH = ""
ECHO_PATH = "/echo"
STATIC_PATH = "/static/"
STATUS_PATH = "/status"
PROXY_PATH = "/"
CONFIG_FILE_NAME = "config"
CONFIG_FILE_NAME2 = "config2"

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
    PROCNAME = "server"

    for proc in psutil.process_iter():
      if proc.name() == PROCNAME:
        print "Webserver process found, killing process..."
        proc.kill()

def test(name, path, test_string, port):
    print name
    http_req = "curl GET http://localhost:" + port + path
    http_res = get_output_of_shell_command(http_req)

    # Print for clarity
    print "HTTP request: \n"
    print http_req + "\n\n"

    if test_string in http_res:
      print "Success!\n"
    else:
      print "Error, check output: \n" + http_res
      kill_server(1)

def main():

  # Make and get output
  print get_output_of_shell_command("make")

  # Run new webserver for proxy handling
  run_background_process("./server " + CONFIG_FILE_NAME + " &")

  # Basic reverse proxy test
  test("PROXY HANDLER", PROXY_PATH, "UCLA.edu Homepage", "8080");

  kill_server(0)

  # Run new webserver for proxy handling
  run_background_process("./server " + CONFIG_FILE_NAME2 + " &")

  # Reverse proxy redirect test
  test("PROXY HANDLER", PROXY_PATH, "UCLA.edu Homepage", "8080");

  kill_server(0)


if __name__ == "__main__":
  main()