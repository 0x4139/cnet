cnet
=======

A simple synchronous TCP library for Node.js. This is implemented as a [native addon]
(http://nodejs.org/api/addons.html).

**Note before proceeding** that Node is designed to be an asynchronous environment,
and if you want to use this module you should have a really good reason to do so.


Usage
--------------
```
    var cnet = require('cnet');
    var socket = cnet.connect('host',port);
    cnet.write(socket,'0x4139@Gmail.com');
    var data = cnet.read(socket);
    console.log(data);
    cnet.close(socket);
```

Current Status
--------------

Currently only the client part is implemented, with the ability to open a connection
to the server, read/write to the connection, and close the connection. The server part
isn't implemented yet, as I can't think of a useful use case for it.

License
-------

This piece of code is licensed under the MIT license @ Vali Malinoiu.
