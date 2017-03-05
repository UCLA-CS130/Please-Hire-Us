# Simple Web Server
UCLA CS130: Winter 2017  
Created using Boost C++ Library

## Usage

### Setup

```
$ git clone --recursive https://github.com/UCLA-CS130/Please-Hire-Us.git
```

### Dependencies

#### Boost Library
```
$ sudo apt-get install libboost-all-dev
```

### Build and Run

To build and run the web server with a config file named 'config':

```
$ make
$ ./server config
```

To clean up (necessary to do before building and running again):

```
$ make clean
```

To run all unit tests:

```
$ make test
```

To run all unit tests and check coverage:

```
$ make coverage
```

To run reverse proxy integration tests:

```
$ pip install psutil
$ make reverse_proxy_int_test
```

## Source Code Information

### Code Layout

* `server.cpp`: Defines the server class, which initiates and runs the server process
* `request.cpp`: Defines the request class, representing an HTTP request
* `response.cpp`: Defines the response class, representing an HTTP response
* `request_handler.hpp`: The abstract base class for request handling
* `request_handler_echo.cpp`: Defines the EchoHandler class, which initiates and handles echo requests
* `request_handler_static.cpp`: Defines the StaticHandler class, which initiates and handles static requests
* `request_handler_status.cpp`: Defines the StatusHandler class, which initiates and handles status requests
* `request_handler_notFound.cpp`: Defines the NotFoundHandler class, which initiates and handles 404 not found requests

Each of the above .cpp files has an accompanying `_test.cc` file containing unit tests for the respective class



##### Contributors
 * Michael Germano
 * Austin Nasso
 * Gil Bar-Or

 
