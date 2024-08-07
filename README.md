# Eldentree
## A message queue managment app

<img src="https://drive.usercontent.google.com/uc?id=18lF9L0VFy5E2a1ieIR50TEXOnWiUdOCT&export=download">

## Table of contents
- [Introduction](#introduction)
- [Capabilities](#capabilities)
- [Building](#building)
- [Usage](#usage)
- [Notes](#notes)

## Introduction
Eldentree is message queuing app inspired by the "Erd tree" from the video game "Elden ring". This app which is built on top of **libqeh**, a custom library for handling queued events, allows producers to effectively pass their events onto the workers.

## Capabilities
- No limit on the number of queues.

- No limit on the number of pending events.

- No limit on queue name length.

- No limit on event message length.

- Fair scehduling prevents from some queues to starve. Everyone will have a fair chance of having their voice heard. 

- No limit on the number of workers.

- Worker count can be customized depending on
 your need. 
- Proper error handling. 

- Capability to peek into the event queue at any time.

## Building
This app and its underlying library both come with build systems. To build the project follow the steps below:

1- Clone this repo or download its zip file.

2- Change directory to the root folder of the project.

3- Install the latest stable version of cmake for your system.

4- Run the following command from the project's root folder:
```sh
cmake -S . -B ./build/ && cmake --build ./build/
```
5- If no errors surfaced during the building process, you can access the binary at "build/Eldentree".

6- This app is standalone, no particular privilege required, no config file required, and no obscure library required. 

7- If you desire, you can move the binary to wherever you like.


## Usage
This app requires a list of const char pointers in order to access the inputs. By default, it uses a hardcoded array of c-style strings defined in the main.cpp file. You're free to add whatever code you need in order to parse your network data or file data into the valid structure accepted. In short, you'll need to provide an array of c-style strings. Each element of the array should have only and only 2 null terminators i.e '\0'.

The first null terminator comes immediately after the queue name. The second one should come immediately after the event message. For instance:
```C
const char* input = "this_is_queue_name\0this_is_event_message\0";
```
The snippet above is an example of a **Single** input element. You'll need one of these for each and every event-queue-pair. This process can easily be automated.

## Notes
- This product is provided under a liability-free license.

- This product is not intended to be used as a finished product.

- As always, it's on you to know what you're doing. As such, it is stronly advised for you to follow the security best practices for your platofrm. 

- You are responsible for what you do with this product. I neither encourage nor condone any malicious activity that might be caused by this product.




Last update on: Aug 2024


