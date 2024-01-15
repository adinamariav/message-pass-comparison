# message-pass-comparison

This project aims to explore the client-server and peer-to-peer models by implementing a message passing application adhering to them and comparing the results.

The applications are implemented in C++, and the subsequent tests are implemented in Python using the 'multiprocess' module.

# Test 1

The first test meant to explore the time to send a large message over the two applications.
The time recorded was similar in both the peer-to-peer and client-server applications.

# Test 2

The second test measured the time it takes to process a large volume of messages in a short period of time.
The peer-to-peer application performed very badly compared to the client-server application.
