# mDNS Resolver [![Build Status](https://travis-ci.org/madpilot/mDNSResolver.svg?branch=master)](https://travis-ci.org/madpilot/mDNSResolver)

An Arduino library that resolves mDNS (bonjour) names. It doesn't do any discovery, it just resolves names.

## Compatibility

* Hardware: ESP8266. I don't believe the stock Arduino UDP library supports multicast ([Please correct me if I'm wrong](https://github.com/madpilot/mDNSResolver/issues)), so only the ESP8266 is supported at the moment
* Software: Tested against avahi and avahi-alias which adds CNAMEs. Post a bug report if it doesn't work with a server that you are using.

## Why?

My home automation projects don't need discovery - I just wanted to be able to enter the name of my MQTT server when configuring my devices. There are other libraries out there that do discovery of services, but they didn't do name resolution easily - And they introduced more code than I needed.

# Installation

Clone this code in to your [Arduino directory](https://www.arduino.cc/en/Guide/Libraries#toc5).

```bash
cd ~/Arduino
git clone git@github.com:madpilot/mDNSResolver.git
```

Restart your IDE. You can now include

```cpp
#include <mDNSResolver.h>
```

in your sketch.

# Usage

```cpp
IPAddress ip = resolver.search("test.local");
```

Returns INADDR_NONE if the name can't be found.

See the [examples folder](https://github.com/madpilot/mDNSResolver/tree/master/examples) for the full setup details

# Advanced Customisation

You can change a few settings by defining some constants before including the mDNSResolver header file, but only if you really know what you are doing

```cpp
#define MDNS_RETRY    1000      # Number of milliseconds between rebroadcasting a name query
#define MDNS_ATTEMPTS 5         # Number of times to try a resolving before giving up
#define UDP_TIMEOUT   255       # Amount of time (in milliseconds) before giving up on the UDP packet
#define MDNS_TLD      ".local"  # Top-level domain to search for. This can technically be anything, but pretty much everyone says leave it as .local

#include <mDNSResolver.h>
```

# Testing

The library has a suite of specs, which you can run on any machine that supports g++ (requires c++11 support).

The specs are written using [Catch](https://github.com/philsquared/Catch), written in BDD style.

```bash
make test
```

# Contributing

Issues and bugs can be raised on the [Issue tracker on GitHub](https://github.com/madpilot/mDNSResolver/issues)

For code and documentation fixes, clone the code, make the fix, write and run the tests, and submit a pull request.

Feature branches with lots of small commits (especially titled "oops", "fix typo", "forgot to add file", etc.) should be squashed before opening a pull request. At the same time, please refrain from putting multiple unrelated changes into a single pull request.

# License

See [LICENSE.txt](https://github.com/madpilot/mDNSResolver/tree/master/LICENSE.txt)

# Credits

Much inspiration, and some parts of the code have been taken (or modified from) https://github.com/mrdunk/esp8266_mdns

