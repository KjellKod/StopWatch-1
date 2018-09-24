This utility, tightly dependent on StopWatch, is nifty when you want to do comparisons and measurements of how time expensive function calls are. 

Some interesting stats from our `master` branch modified for getting some metrics

Multiple measurements of roughly 6.8 million packets measured each time

**ZMQ Routing Thread Fire to Qosmos Worker**

| Min  | Max | Average |
| ------------- | ------------- | ------------- |
| 2 us  | 25 ms  | 8-15 us |


**Hashing of packets** Multiple measurements of roughly 7 million packets measured each time

| Min  | Max | Average |
| ------------- | ------------- | ------------- |
| 90 ns  | 23 ms  | 0.6 - 1 us |


**FULL packet routing loop (hashing etc) MINUS the "get packets"**
Multiple measurements of roughly 200,000 loops (each 1 packet)

| Min  | Max | Average |
| ------------- | ------------- | ------------- |
| 26 ns (no work)  | 154 ms  | 400 - 600 us |


**Routing thread: Get PACKETS from I/F capture thread**
Multiple measurements of roughly 150, 000 - 200,000 loops 

| Type:  | Min |  Max | Average |  
| ------------- | ------------- | ------------- | ------------- | 
| ZMQ | 6 us   | 23 ms  |  1020 us*|  
| MutexQ |  0.2us | 358ms | 18 ms* |
| Circular lock-free  synchronized atomics |  1.1 us | 0.5 - 1.3 ms | 2 - 3 us |
| Circular lock-free relaxed atomics |  1.1 us | 0.5-1 ms |  1 - 2.5 us
                
**I/F Push to  Routing Thread:**

| Type:  | Min |  Max | Average |  
| ------------- | ------------- | ------------- | ------------- | 
| ZMQ | 7 us   | 36 ms  |  53 us|  
| MutexQ |  01.5us | 32ms | 6 us |
| Circular lock-free synchronized atomics |  1.1 us | 6.5 ms | 2 us |
| Circular lock-free relaxed atomics |  1 us | 0.9 -1.3 us |  1 us


**Read CHUNK packets from I/F** 
Measured 100,000 times

| Min  | Max | Average |
| ------------- | ------------- | ------------- |
| 97 us   | 65 ms  | 2.8 ms |
