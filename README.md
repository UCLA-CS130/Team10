### Basic Echo Server Usage

```bash
  g++ server.cpp -o server -lboost_system
  ./server <port>
```

then open another terminal
```bash
  telnet localhost <port>
```

and it should echo your responses!
