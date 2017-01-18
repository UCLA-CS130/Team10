## Basic Echo ServerUsage

`
g++ server.cpp -o server -lboost_system
./server <port>
`

then open another terminal
`
telnet localhost <port>
`

and it should echo your responses!
