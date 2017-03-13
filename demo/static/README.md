[![Build Status](https://travis-ci.org/UCLA-CS130/Team10.svg?branch=master)](https://travis-ci.org/UCLA-CS130/Team10)


## Usage

```bash
$ make
$ ./webserver ./demo/config.conf
```

List of Handlers:

1. EchoHandler - /echo
2. StaticHandler - /static/
3. StatusHandler - /status
4. NotFoundHandler - any uri not included above


## Testing

#### Integration Test

```bash
$ sudo apt-get install python-pip
$ sudo pip install psutil
$ python test.py
```

## Quick Guide to adding new Handlers

1. Create your handler hpp and cpp files
2. Add your Handler configuration in the config.conf file
3. You can add any special behavior with other Handlers in connection.cpp, where
the handle_read method will control which Handler to use.

## Source Code Structure

Our main.cpp instantiates a Server object, which takes in a ServerConfig object,
as well as the host address. The Server then creates a map of pointers to Handlers
as per the ServerConfig instructions, which are based off of demo.config.conf.

A connection manager is used to manage multiple connections to server. Each
connection is responsible for determining the Handler type it needs to invoke
in order to display the information requested. The request and response classes
are designed solely to parse and provide information for the connection class to manage.




### Code skeleton
http://www.boost.org/doc/libs/1_57_0/doc/html/boost_asio/examples/cpp03_examples.html

### Makefile
http://stackoverflow.com/questions/33282703/running-http-server-example-from-boost-asio
