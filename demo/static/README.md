# Our design documentation was written in Markdown :)



## Usage

`$ make`

`$ ./webserver ./demo/config.conf`



#### List of Handlers:

1. EchoHandler - /echo
2. StaticHandler - /static/
3. StatusHandler - /status
4. NotFoundHandler - any uri not included above
5. ReverseProxyHandler - /proxy

#### List of Features added

1. Markdown rendering
  * Used third-party library to implement reading .md files and rendering them to HTML.
  * Demo:

  `$ make`

  `$ ./webserver ./demo/config.conf`

  `$ GET http://localhost:3000/static/README.md`

  * This exact file should be spat back out in HTML format.


2. Gzip compression

3. AWS hosting
  * Root URL: http://ec2-35-163-65-169.us-west-2.compute.amazonaws.com

## Testing

#### Unit testing

`$ make debug`

#### Integration Test


`$ sudo apt-get install python-pip`

`$ sudo pip install psutil`

`$ make int`


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
