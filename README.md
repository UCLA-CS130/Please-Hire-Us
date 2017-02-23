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

##### Contributors
 * Michael Germano
 * Austin Nasso
 * Gil Bar-Or

 
