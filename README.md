
## Usage

```bash
./server <host> <port> <root-directory>
```

## Example

```bash
$ make
$ ./server 0.0.0.0 8081 . &
$ GET http://localhost:8081/demo/index.html > demo/index.html.res  
```

Check the resulting output in demo/index.html.res and make sure
```bash
$ diff demo/index.html demo/index.html.res
```
returns nothing, which means the two files are exactly the same.

## TODO
1. Use config file and config parser to handle <host> <port> and <root-directory> arguments
2. Create valid HTTP response headers
2a. \r\n bullshit

### Code skeleton
http://www.boost.org/doc/libs/1_57_0/doc/html/boost_asio/examples/cpp03_examples.html

### Makefile
http://stackoverflow.com/questions/33282703/running-http-server-example-from-boost-asio
