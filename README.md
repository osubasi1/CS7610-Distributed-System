# CS7610 Foundation Of Distributed System Project

A project that implements a laptop factory as a distributed system with load balancer and cache. 
The project implemented to run on Khoury machines which is a linux envoriment.
## Step by step usage
- Need to ssh to Khoury machine
- Make to build program. It will create four executable files (server, client, loadbalancer, sysclient)
- Each server, client, and loadbalancer need to run on different Khoury machine. 
- You can access different VDI machines via ssh by specifying their addresses. VDI machine addresses range from vdi-linux-030.ccs.neu.edu to vdi-linux-070.ccs.neu.edu.
- To run the server (generic)
  - ./server [unique id] [0]
- To run the load balancer
  - ./loadbalancer [port #] [# of servers] [cache size] [number of replicas] [algorithm] (repeat [ID] [IP] [port #])
    1. Port number
    2. Number of servers
    3. Cache size
    4. Number of replicas each server has
    5. Algorithm (1 for consistent hashing algorith, 0 for random distribution)
    6. Unique ID, IP, and port of all peer factories
 - To run client
  - ./client [ip address] [port #] [# of customer] [# of orders] [request type]
    1.ip address of loadbalancer
    2.Port number of loadbalancer
    3.Number of customer
    4.Number of orders
    5.Request type
      - 1 for laptop order request (write request)
      - 2 for customer record read request (need to pass 1 as number of customer)
      - 3 print all the customer record (need to pass 1 as number of customer)
      - 5 print all the cache content (need to pass 1 as number of customer)
      - 6 read request for a specific customer (need to pass customer id as number of customer)
 - To run sysclient
  - ./sysclient [ip address] [port #] [# of servers] (repeat [ID] [IP] [port #])
    - 1 ip address of loadbalancer
    - 2 Port number of loadbalancer
    - 3 ID, IP, and port number of peer servers


