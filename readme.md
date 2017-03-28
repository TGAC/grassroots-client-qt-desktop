# Desktop client

This builds a  graphical user interface desktop client to access a Grassroots server. It uses a [Qt](https://www.qt.io)-based GUI 

## Installation

To build this handler, you need the [grassroots core](https://github.com/TGAC/grassroots-core), [grassroots build config](https://github.com/TGAC/grassroots-build-config) installed and configured. You will also need the [Qt development](https://www.qt.io/download/) files installed

The files to build the Dropbox handler are in the ```build/<platform>``` directory. 

### Linux

If you enter this directory 

```cd build/linux```

you can then create the makefile by typing

```qmake -makefile grassroots-QT.pro```

and then 

``` make ```

to install the client into the Grassroots system.

## Usage

 * **h**: The uri of the Grassroots server to connect to.
 * **a**: The command to send to the Grasroots server. This can be one of the following:
   * 0: *get_all_services*: Get all of the services available from the Grassroots server.
   * 1: *get_schema_version*: Get the version of the schema that the Grassroots server is using.
   * 2: *get_interested_services*: Get all of the services that are capable of running for a given resource from the Grassroots server.
   * 3: *run_keyword_services*: Run all keyword-aware services for a given keyword.
 * **u**: The username to use.
 * **r**: The Grassroots Server is not web-based so use a raw socket connection instead.
 * **s**: If using a raw socket connection, connect to this port.
 * **k**: The keyword to use when running a keyword search. 								
