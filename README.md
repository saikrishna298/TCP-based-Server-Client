                                       # TCP-based-WebServer-Client


 1)	Untar using tar –xPvf http_server_client.tar and go to make directory.

 2)	Use commands: make clean;make all;

 3)	Binaries will be formed in /bin directory 

 4)	To run on same system , requested file is to be kept in the bin directory.

 5)	To run server on a different system, requested file should be in the same directory as the server.

 6)	Server binds on IP address: 129.79.247.5 and port 4121. This can be changed in /include/header.h. Ex ./http_server

 7)	Client binary run ex ./http_client 129.79.247.5 4121 <p,np> <filename> 

 8)	For persistent connections (indicated by p and field in header) , connection times out after 10 seconds.

 9)	For persistent connection the functionality to open different files from a given filename was not provided to allocate for the fact that the browser might make requests too in which case responses would be hard to decode by the browser.

 10)	This code also runs for persistent browser connections.

