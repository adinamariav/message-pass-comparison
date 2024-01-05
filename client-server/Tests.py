import multiprocessing
import subprocess
import time


def client2_process(client_id, input_queue, output_queue):
    process = subprocess.Popen(["client/client"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True, bufsize=1)
    print(process.stdout.readline())

    id_line = process.stdout.readline()
    id = id_line.strip().split(" ")[-1]

    for i in range(10000):
        process.stdin.write("1 salut\n")
        process.stdin.flush()

    # while True:
    #     received = input_queue.get()
    #     print(f"Client {client_id} received: {received}")
    #
    #     # Start an external executable
    #
    #     process.stdin.flush()
    #     process.stdin.write("2 salut\n")
    #
    #     # Send the output back through the output_pipe


def client1_process(client_id, input_queue, output_queue):
    process = subprocess.Popen(["client/client"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)

    while True:
        print(process.stdout.readline())


def main():
    client1_input = multiprocessing.Queue()
    client1_output = multiprocessing.Queue()

    client2_input = multiprocessing.Queue()
    client2_output = multiprocessing.Queue()

    client1 = multiprocessing.Process(target=client1_process, args=(1, client1_input, client1_output))
    client2 = multiprocessing.Process(target=client2_process, args=(2, client2_input, client2_output))

    client1.start()

    client2.start()

    time.sleep(1)  # Wait for processes to initialize (optional)

    # Write to client1
    client1_input.put("2 salut")

    # Write to client2
    # client2_input.send("3 hello")

    # Wait for processes to finish
    client1.join()
    client2.join()


if __name__ == "__main__":
    main()

# problema client server - se unesc mesajele intr-o implementare simpla