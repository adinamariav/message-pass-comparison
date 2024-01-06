import multiprocessing
import subprocess
import time
import os
import fcntl


def client2_process(client_id, messages, input_queue, output_queue):
    process = subprocess.Popen(["client/client", str(client_id)], stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                               text=True, bufsize=1)

    process.stdout.readline()
    process.stdout.readline()

    for i in range(messages):
        received = input_queue.get()

        process.stdin.write(received)
        process.stdin.flush()

    output_queue.put("Sent " + str(messages) + " messages")


def client1_process(client_id, sender_client_id, messages, input_queue, output_queue):
    process = subprocess.Popen(["client/client", str(client_id)], stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                               text=True)

    process.stdout.readline()
    process.stdout.readline()

    fl = fcntl.fcntl(process.stdout, fcntl.F_GETFL)
    fcntl.fcntl(process.stdout, fcntl.F_SETFL, fl | os.O_NONBLOCK)

    nr_received = 0

    while nr_received < messages:
        line = process.stdout.readline()
        if len(line.strip()) != 0:
            # print(line)
            words = line.strip().split()
            if words[0] == "Received" and words[1] == str(sender_client_id):
                nr_received = nr_received + 1

    output_queue.put("Received " + str(nr_received) + " messages")


def main():
    client1_input = multiprocessing.Queue()
    client1_output = multiprocessing.Queue()

    client2_input = multiprocessing.Queue()
    client2_output = multiprocessing.Queue()

    messages = 900000

    client1 = multiprocessing.Process(target=client1_process, args=(1, 2, messages, client1_input, client1_output))
    client2 = multiprocessing.Process(target=client2_process, args=(2, messages, client2_input, client2_output))

    client1.start()

    client2.start()

    time.sleep(1)  # Wait for processes to initialize (optional)

    t = time.process_time()

    for i in range(messages):
        message = str(1) + " mesaj\n"
        client2_input.put(message)

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
