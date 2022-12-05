Nadesh Seen 21q050003
Shashank 21q05r002
Gunjan 213050062

-> Change directory to KV_store
-> Create 4 new directories (If not present already):
    1) mkdir Input
    2) mkdir Output
    3) mkdir build 
-> Create another directory inside Output folder using command: mkdir KV_files  //Server will automatically store persistent key value pairs in different files in this folder. Do not modify this folder after creation. 
-> Place "batch.txt" and "CONFIG" files in "Input" folder //Placing CONFIG file is mandatory and "batch.txt" is only required if MODE is set to BATCH in CONFIG file.
-> change directory to "build"
-> Clear any previous files/folders in "build" using command: rm -rf *
-> Run command: cmake ..
-> Run command: make 
-> To start server, run command from build directory: ./KV_Server
-> To start client, open another terminal and run command from build directory: ./KV_Client

//Chord Protocol Syntax
Command for 1st Server join:
./KV_Server <portnumber>

Command for firther servers to join:
./KV_Server <portnumber of new server> <portnumber of any exisiting server>

Command for updating key-value store
./KV_Client <portnumber of server client wants to ping>
Now use PUT/GET/DELETE in Batch interactive mode as usual

How to test distributed KV_store ?
1. Start the first server with only 1 argument i.e. its own port_no
2. To start more servers 2 arguments is to be provided by user.
	a. First argument is the port_no of the server itself
	b. 2nd argument is the port_no of the server it will ping.
3. Now run clients with 1 argument i.e. port_no of the server it wants to ping.

Important Information:
Format of "batch.txt"
-> "batch.txt" must contain GET/PUT/DEL requests along with corresponding key and values
->Each line of "batch.txt" contains either of the following 3 requests:
    1) GET <key>             
    2) PUT <key> <val>
    3) DEL <key>
-> All entries of "batch.txt" must be single-space separated

Format of "CONFIG"
-> Each line of CONFIG file is:<PARAMETER NAME> <PARAMETER VALUE>
-> Both name and values are single space separated
-> You must provide 5 paramater names along with their values
-> Following are the 5 parameter names (Names are case sensitive. So, provide exactly same name)
    1) LISTENING_PORT <value>                 //Takes port number
    2) CACHE_REPLACEMENT_TYPE <value>        //Takes 2 possible values: LRU/LFU 
    3) CACHE_SIZE <value>                    //Takes cache size
    4) THREAD_POOL_SIZE <value>              //Takes thread pool size from server end   
    5) MODE <value>                         // Takes 2 possible values: BATCH/INTERACTIVE. Batch mode takes input from "batch.txt". Interactive mode takes input from user

-> LISTENING_PORT is not used in the new distributed key value store as the values are given by user.



-> NOTE: If mode is set to INTERACTIVE , Unlike "batch.txt", manual input for requests will be taken from the user in the same format as "batch.txt" file.
-> For more clarity, try running client in INTERACTIVE mode                                         


Files generated in Output folder:
1) <client id>_client_log.txt   //Captures response time of each request sent by the client. Unique for each client beacuse of client id
2) <client_id>_client_stat.txt  //Captures statistics like: Min/Max/Avg response time for all requests in a single experiment
3) server_log_<server_ip_port>.txt. For each incoming request, following 3 information is captured:
    ->Client id and the request type of client (GET/PUT/DEL along with key and value)
    ->Thread id of server handling the request and the response from server
    ->Keys of cache after request is served 
4) Total_time.txt: Total time elapsed in (ms) between handling first and last request
5) Graph folder has performance analysis graph



Graphs Explanation:
We  conducted batch experiments for multiple clients and plotted 2 parameters:
1) Average response time against client load
2) Throughput against client load

Avg Response Time = Sum of all response time for all requests across all clients/ Sum of all requests across all clients

Throughput = Total requests processed across all clients / Time elapsed between 1st and last request. (1st and Last request can come from different clients) 

Client Load Description:
Each client runs a batch file containing 60,000 PUT/DEL/GET requests. 
We varied client load on X axis from 1 to 8. This means total requests sent to server varied from 60k to 480k (for 8 clients)

Server Config Description:
LISTENING_PORT - 50051
CACHE_REPLACEMENT_TYPE - LFU
CACHE_SIZE - 5
THREAD_POOL_SIZE - 6
MODE - BATCH


References:

Overall structure of the client server code - https://github.com/grpc/grpc/tree/master/examples/cpp
Flow of the async code - https://blog.miigon.net/posts/so-difference-between-sync-and-async-grpc/
Registering multiple RPCs in async - https://stackoverflow.com/questions/49318889/asynchronous-model-in-grpc-c
How to get current time in string in C++ - https://stackoverflow.com/questions/16357999/current-date-and-time-as-string
Curr time in millisecond - https://stackoverflow.com/questions/41077377/how-to-get-current-time-in-milliseconds
https://github.com/cartographer-project/async_grpc/blob/master/README.md
Sync server will automatically increase the number of threads - https://groups.google.com/g/grpc-io/c/yCzzroDbPa0
Async server we have to create our own thread pool - https://groups.google.com/g/grpc-io/c/Cul6fd7cOB0/m/8X1jI_sdBAAJ?utm_medium=email&utm_source=footer
Sync server threading model is not accessible by the user level API - https://groups.google.com/g/grpc-io/c/Cul6fd7cOB0
Async Server Code - https://groups.google.com/g/grpc-io/c/yCzzroDbPa0
Mechanism of completion queue - https://groups.google.com/g/grpc-io/c/JpGURpAANss
Adding multiple rpcs in async server model - https://stackoverflow.com/questions/41732884/grpc-multiple-services-in-cpp-async-server
https://github.com/coderaavan/Multi-threaded-Key-Value-Server
https://grpc.io/docs/languages/cpp/quickstart/
