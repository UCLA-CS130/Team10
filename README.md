
## Usage

```bash
./webserver <config file>
```

## Example

```bash
$ make
$ ./webserver ./demo/example.conf
$ GET http://localhost:8081/demo/index.html > demo/index.html.res  
```

Check the resulting output in demo/index.html.res and make sure
```bash
$ diff demo/index.html demo/index.html.res
```
returns nothing, which means the two files are exactly the same.

## TODO
1. Change parse function in main.cpp to recursive function

### Code skeleton
http://www.boost.org/doc/libs/1_57_0/doc/html/boost_asio/examples/cpp03_examples.html

### Makefile
http://stackoverflow.com/questions/33282703/running-http-server-example-from-boost-asio
