import multiprocessing
import subprocess
import time
import os
import fcntl


def client2_process(peer_port, output_queue):
    process = subprocess.Popen(["./p2p", str(peer_port)], stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                               bufsize=1, text=True)

    print(process.stdout.readline())

    message = str(8888) + ' ' + 5000000 * 'A'

    process.stdin.write(message)
    process.stdin.flush()

    output_queue.put("Sent " + message)


def client1_process(peer_port, output_queue):
    process = subprocess.Popen(["./p2p", str(peer_port)], stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                               bufsize=1, text=True)

    print(process.stdout.readline())

    fl = fcntl.fcntl(process.stdout, fcntl.F_GETFL)
    fcntl.fcntl(process.stdout, fcntl.F_SETFL, fl | os.O_NONBLOCK)

    received = False
    line = ""

    while received is False:
        line = process.stdout.readline()
        if len(line.strip()) != 0:
            received = True

    output_queue.put("Received!")


def main():
    client1_output = multiprocessing.Queue()
    client2_output = multiprocessing.Queue()

    client1 = multiprocessing.Process(target=client1_process, args=(8888, client1_output))
    client2 = multiprocessing.Process(target=client2_process, args=(8881, client2_output))

    client1.start()
    client2.start()

    time.sleep(1)  # Wait for processes to initialize (optional)

    t = time.process_time()

    while True:
        if not client2_output.empty():
            out = client2_output.get()
            print(out)
            break

    while True:
        if not client1_output.empty():
            out = client1_output.get()
            print(out)
            break

    elapsed_time = time.process_time() - t

    print("Elapsed time: " + str(elapsed_time))

    client1.join()
    client2.join()


if __name__ == "__main__":
    main()
