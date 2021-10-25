# server-client-C-

There are 2 servers:- server.cpp and server2.cpp
           server.cpp---> single threaded server
           server2.cpp---> multithreaded server

There is one code for client ----> client.cpp

A make file is created to compile all the three codes together

A test Input file is created named as Input.txt
abc.txt----> sample output for the given text file


How To run?
---> run the make command
---> run the server.cpp using "./server <ip_address> <port_no.>
---> run the server2.cpp using "./server2 <ip_address> <port_no.>
---> run the client.cpp using "./client <ip_address_of_server> <port_no_of_server> <input.txt> <output.txt>

