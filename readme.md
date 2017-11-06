# Desktop client


This builds a  graphical user interface desktop client to access a Grassroots server allowing you to query and run services, view results, *etc.* It uses a [Qt](https://www.qt.io)-based GUI. Its imagery uses the icons available from the [Amiga Image Storage System](http://www.masonicons.info/6.html) (AISS) by Martin Merz, Copyright (C) 2000-2016.

![Screenshot of the Grassroots Qt desktop client](grassroots_qt_client.png "Grassroots Qt Desktop Client")

## Installation

To build this client, you need the [grassroots core](https://github.com/TGAC/grassroots-core) and [grassroots build config](https://github.com/TGAC/grassroots-build-config) installed and configured. You will also need the [Qt development](https://www.qt.io/download/) version 5.x or later files installed.

The files to build the desktop client are in the ```build/<platform>``` directory. 

### Linux

If you enter this directory 

```
cd build/linux
```

you can then create the makefile by typing

```
qmake -makefile grassroots-QT.pro
```

and then 

```
make 
```

to install the client into the Grassroots system.

## Usage

To run the client, the command is ```./grassroots-qt-client <options>```

The available options 

 * ```-h <server_url>```: The web address of the Grassroots server to connect to.
 * ```--list-all```: Get all available services from the Grassroots server.
 * ```--keyword-search <keyword>```: Perform a keyword search for a given value against all keyword-aware services.
 * ```--list-interested <resource>```: Get all services that are able to run against a given resource. The resource is in the form \<protocol\>://\<name\> *e.g.* file:///home/test.fa, https://my.data/object, irods://data.fa, *etc.*


The client has a list of available services in the left-hand pane of its window and clicking on any of these will open the editor for that Service in the right-hand pane of the window. 

Each widget in the editor has a label and if you hover the cursor over the the label or widget then a a help bubble will appear with further information. 
The widgets can be grouped together as in the above example where there are three groups: *Query Sequence Parameters*, *Available Databases provided by EI test Grassroots server 0* and *General Algorithm Parameters*. 
These groups can have a checkbox next to their titles and toggling this allows the group to be hidden or visible to allow you to ignore some options once they have set them to their desired values should they wish.
The values for a Service can be restored to their default values by clicking on the *Restore Defaults* in the bottom right-hand corner of the editor window should you wish.

### Running one or more Services

You can choose to run a Service by ticking either the checkbox next to the Service name in the left-hand pane or the "Run .." checkbox below the widgets in the right-hand pane.
Once you have chosen the Services that you want to run and set their values accordingly, you can run all of them by clicking on the *run* icon, ![Run icon](run.png "Run icon") as shown above.

		
