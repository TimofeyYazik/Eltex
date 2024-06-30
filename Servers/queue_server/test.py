import subprocess
import threading
import sys
import time

def run_client():
    process = subprocess.Popen(["./client"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
    
    commands = ["time", "time", "exit"]
    for cmd in commands:
        process.stdin.write(cmd + "\n")
        process.stdin.flush()
        time.sleep(1)  
    
    process.stdin.close()
    
    stdout, stderr = process.communicate()

def create_client_threads(num_clients):
    threads = []
    for _ in range(num_clients):
        thread = threading.Thread(target=run_client)
        threads.append(thread)
        thread.start()

    for thread in threads:
        thread.join()

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python load_test.py <num_clients>")
        sys.exit(1)

    num_clients = int(sys.argv[1])
    create_client_threads(num_clients)
