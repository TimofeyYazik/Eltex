import subprocess
import threading
import sys

def run_client():
    subprocess.run(["./client"])  # Предполагается, что клиентский исполняемый файл называется 'client'
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

