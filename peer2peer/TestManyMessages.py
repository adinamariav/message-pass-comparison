import multiprocessing
import subprocess
import time
import os
import fcntl


def peer2_process(peer_port, messages, input_queue, output_queue):
    process = subprocess.Popen(["./p2p", str(peer_port)], stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                               bufsize=1, text=True)

    print(process.stdout.readline())

    for i in range(messages):
        received = input_queue.get()

        process.stdin.write(received)
        process.stdin.flush()

    output_queue.put("Sent " + str(messages) + " messages")


def peer1_process(peer_port, messages, output_queue):
    process = subprocess.Popen(["./p2p", str(peer_port)], stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                               bufsize=1, text=True)

    print(process.stdout.readline())

    fl = fcntl.fcntl(process.stdout, fcntl.F_GETFL)
    fcntl.fcntl(process.stdout, fcntl.F_SETFL, fl | os.O_NONBLOCK)

    nr_received = 0

    while nr_received < messages:
        line = process.stdout.readline()
        if line != '':
            # print(line)
            nr_received = nr_received + 1

    output_queue.put("Received " + str(nr_received) + " messages")


def main():
    peer1_input = multiprocessing.Queue()
    peer1_output = multiprocessing.Queue()

    peer2_input = multiprocessing.Queue()
    peer2_output = multiprocessing.Queue()

    messages = 50000

    peer1 = multiprocessing.Process(target=peer1_process, args=(8888, messages, peer1_output))
    peer2 = multiprocessing.Process(target=peer2_process, args=(8881, messages, peer2_input, peer2_output))

    peer1.start()

    peer2.start()

    time.sleep(1)  # Wait for processes to initialize (optional)

    t = time.process_time()

    for i in range(messages):
        message = str(8888) + " mesaj\n"
        peer2_input.put(message)

    while True:
        if not peer2_output.empty():
            out = peer2_output.get()
            print(out)
            break

    while True:
        if not peer1_output.empty():
            out = peer1_output.get()
            print(out)
            break

    elapsed_time = time.process_time() - t

    print("Elapsed time: " + str(elapsed_time))

    peer1.join()
    peer2.join()


if __name__ == "__main__":
    main()
