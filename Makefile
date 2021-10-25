simple:
	g++ server.cpp -o server
	g++ server2.cpp -o server2 -lpthread
	g++ client.cpp -o client

server_single:
	g++ server.cpp -o server

threaded_server:
	g++ server2.cpp -o server2 -lpthread

client:
	g++ client.cpp -o client