# CN_CHomeworks_4

## Contents

- [Code Documentation](#code-documentation)

  - [NewReno Class](#newreno-class)
  - [Reno Class](#reno-class)

- [Questions](#questions)
  - [Q1](#q1-whats-the-difference-between-congestion-control-and-the-flow-control)
  - [Q2](#q2-describe-new-renos-algorithm-shortly)
  - [Q3](#q3-describe-bbr-algorithm-shortly)
  - [Q4](#q4-whats-the-difference-between-these-three-algorithms)
  - [Q5](#q5-analyze-and-describe-these-three-algorithms)
  - [Q6](#q6-name-some-of-the-congestion-control-algorithms-that-are-not-mentioned-in-this-project)

## Code Documentation

### NewReno Class

#### Public Methods

##### The Constructor

The NewReno constructor initializes the NewReno congestion control algorithm with the following parameters:

cwnd: Initial congestion window size. It represents the number of packets that can be sent without waiting for an acknowledgment.

ssthresh: Initial slow start threshold. It determines when the algorithm transitions from the slow start phase to the congestion avoidance phase.

fileSize: Size of the file being transmitted.

rtt\*: Round-trip time. It represents the time taken for a packet to travel from the sender to the receiver and back.

losstresh\*: Loss threshold. It is used to detect loss events and trigger congestion control mechanisms.

lossScale\*: Loss scale factor. It adjusts the congestion window reduction during congestion events.

time\*: Time counter. It can be used to track the duration of the transmission or measure timing-related aspects of the algorithm.

retransRemain\_: Remaining retransmission attempts. It keeps track of the number of retransmission attempts during the fast retransmit phase.

##### `void sendData()`

The sendData() function uses a switch-case statement to determine the current mode of the algorithm and performs the corresponding actions based on the mode. The different modes include:

SLOW*START: In this mode, if the congestion window (cwnd*) is smaller than the slow start threshold (ssthresh*), the congestion window is doubled (cwnd* \*= 2). Once the congestion window exceeds the slow start threshold, the mode is switched to CONGESTION_AVOIDANCE.

CONGESTION_AVOIDANCE: In this mode, the congestion window is increased by the maximum segment size (MSS) for each successful transmission. This helps to gradually increase the sending rate while avoiding congestion.

FAST*RECOVERY: If the algorithm enters fast recovery mode, the congestion window is reduced by half (cwnd* = cwnd* / 2) and the slow start threshold is set to the current congestion window size (ssthresh* = cwnd\_). The algorithm then switches to FAST_TRANSIMIT mode.

FAST*TRANSIMIT: In this mode, the algorithm keeps track of the remaining retransmission attempts (retransRemain*). If the retransmission attempts are exhausted (retransRemain\_ <= 0), the algorithm switches back to CONGESTION_AVOIDANCE mode.

##### `void onPacketLoss(int)`

On packet loss we go to FAST_RECOVERY mode and set the retransmission attempts.

##### `void log(std::ofstream&) const`

This function logs the current state of the algorithm to the given output stream. It logs the current congestion window size (cwnd*), slow start threshold (ssthresh*), and mode.

##### `void retransmit()`

This function is called when a packet needs to be retransmitted. It decrements the retransmission attempts (retransRemain\_) and switches to FAST_RECOVERY mode.

##### `void run()`

This function runs the algorithm. It first initializes the algorithm by setting the initial congestion window size (cwnd*) and slow start threshold (ssthresh*). It then enters a loop that runs until the congestion window size reaches the maximum value (cwnd* >= maxCwnd*). In each iteration, it calls the sendData() function to send packets and handle congestion. Once the maximum congestion window size is reached, the algorithm terminates.

#### Private Methods:

##### `double lossProbability()`

This function calculates the probability of packet loss based on the current congestion window size (cwnd*) and the maximum congestion window size (maxCwnd*). It uses a linear function to calculate the probability, which increases as the congestion window size approaches the maximum value.

### Reno Class

#### Public Methods

##### The Constructor

The Reno constructor initializes the Reno congestion control algorithm with the following parameters:

cwnd: Initial congestion window size. It represents the number of packets that can be sent without waiting for an acknowledgment.

ssthresh: Initial slow start threshold. It determines when the algorithm transitions from the slow start phase to the congestion avoidance phase.

fileSize: Size of the file being transmitted.

rtt\_: Round-trip time. It represents the time taken for a packet to travel from the sender to the receiver and back.

losstresh\_: Loss threshold. It is used to detect loss events and trigger congestion control mechanisms.

lossScale\_: Loss scale factor. It adjusts the congestion window reduction during congestion events.

unacknowledged\_: Number of unacknowledged packets. It keeps track of the packets that have been sent but not yet acknowledged.

time\_: Time counter. It can be used to track the duration of the transmission or measure timing-related aspects of the algorithm.

retransRemain\_: Remaining retransmission attempts. It keeps track of the number of retransmission attempts during congestion events.

##### `void onSelectiveAck(int)`

In this function we increase retransRemain\_ and go to FAST_RECOVERY mode. Difference between this function and onPacketLoss() is that we don't set the retransmission attempts. Because all lost packet will be handled in one Recovery phase.

### BBR Class

#### Public Methods

##### The Constructor

The BBR constructor initializes the BBR congestion control algorithm with the following parameters:

max_bw: Maximum estimated bottleneck bandwidth. It represents the upper limit of the estimated bandwidth of the network path.

min_rtt: Minimum round-trip time. It represents the lower limit of the round-trip time experienced by the network packets.

fileSize: Size of the file being transmitted.

mode\_: Initial mode of the algorithm, set to STARTUP. The algorithm dynamically adjusts the mode based on network conditions.

cwnd\_: Initial congestion window size, set to 1. It represents the number of packets that can be sent without waiting for an acknowledgment.

rtt\_: Round-trip time, initialized with a default value.

target*cwnd*: Target congestion window size, set to 500. It represents the desired number of packets in flight.

pacing*gain*: Pacing gain factor, set to 2.0. It determines the rate at which packets are transmitted.

gain*cycle*: Gain cycle length, set to 8.0. It influences the pacing gain value.

losstresh\_: Loss threshold. It is used to detect loss events and trigger congestion control mechanisms.

lossScale\_: Loss scale factor. It adjusts the congestion window reduction during congestion events.

time\_: Time counter. It can be used to track the duration of the transmission or measure timing-related aspects of the algorithm.

unacknowledged\_: Number of unacknowledged packets. It keeps track of the packets that have been sent but not yet acknowledged.

retransRemain\_: Remaining retransmission attempts. It keeps track of the number of retransmission attempts during congestion events.

##### `double getSendingRate`

It updates sending rate by pacing*gain* max to max*bandwidth*.

##### `void onBottleneck()`

It switches to Drain phase and decrease cwnd\_ by 25%.

#### Private Methods:

## Questions

### Q1. "What's the difference between congestion control and the flow control?"

Flow control and congestion control are two mechanisms used in computer networks to regulate the flow of data between network devices. Although they both aim to optimize network performance, they address different aspects of data transmission.

Flow control manages the flow of data between a sender and receiver to ensure that the receiving device can handle the incoming data at a pace it can handle. It prevents the sender from overwhelming the receiver with a large amount of data, which could lead to data loss or buffer overflow. Flow control typically uses techniques like buffering, sliding window protocols, and acknowledgement messages to regulate the transmission rate and ensure smooth data delivery.

Congestion control, on the other hand, deals with network-wide traffic congestion, occurring when the overall demand for network resources exceeds the available capacity. It aims to avoid or alleviate congestion by controlling the rate at which data is injected into the network. Congestion control mechanisms monitor network conditions, detect congestion signs (e.g., increased delays, packet loss), and take actions to reduce the amount of traffic being sent. These actions may include reducing the transmission rate, adjusting window sizes, or using congestion notification mechanisms to inform senders about the network state.

In summary, flow control focuses on regulating the data flow between a sender and receiver, ensuring the receiver can handle the incoming data. Congestion control, on the other hand, addresses network-wide congestion by managing the rate at which data is sent into the network to prevent overload and maintain overall network performance.

### Q2. "Describe New Reno's algorithm shortly."

New Reno is an enhancement to the Reno congestion control algorithm used in TCP (Transmission Control Protocol), which is widely employed for reliable data transmission over computer networks. New Reno improves upon Reno's congestion control mechanism by introducing a modification to the way TCP handles packet retransmissions after detecting congestion.

In the original Reno algorithm, when congestion is detected, TCP reduces its congestion window (cwnd) and enters a slow-start phase, gradually increasing the cwnd to probe for available network capacity. However, Reno has a limitation known as "TCP global synchronization" that can cause all TCP connections sharing a bottleneck link to enter congestion avoidance simultaneously, leading to suboptimal network performance.

New Reno addresses this limitation by introducing a concept called "fast recovery" during the congestion avoidance phase. When a TCP sender receives duplicate acknowledgments (ACKs) indicating that a packet has been lost, instead of halving the cwnd and entering slow-start, New Reno enters fast recovery. In fast recovery, the sender increases the cwnd by a smaller value, known as the "fast recovery threshold" or ssthresh, and relies on receiving further duplicate ACKs to retransmit the lost packet.

If the sender receives three duplicate ACKs, it assumes that the next transmitted packet has been lost and performs a fast retransmit by retransmitting the missing packet without waiting for a timeout. After fast retransmit, the sender reduces the cwnd to half its value and enters congestion avoidance, increasing the cwnd more cautiously as before.

The introduction of fast recovery in New Reno allows TCP connections to recover from losses more quickly, avoiding unnecessary slow-start episodes. This improvement helps to mitigate the TCP global synchronization issue, as multiple connections sharing the same bottleneck link can recover from losses at different rates, leading to better overall network performance.

### Q3. "Describe BBR algorithm shortly."

BBR (Bottleneck Bandwidth and Round-Trip Time) is a congestion control algorithm developed by Google for TCP (Transmission Control Protocol) to optimize network performance and throughput. It aims to efficiently utilize available network capacity by dynamically adjusting the sending rate based on network conditions.

BBR takes a different approach compared to traditional congestion control algorithms like Reno or Cubic, which primarily rely on packet loss as an indicator of network congestion. Instead, BBR focuses on two key metrics: the bottleneck bandwidth and the round-trip time (RTT) of the network.

The BBR algorithm estimates the available bottleneck bandwidth by measuring the maximum number of bytes that can be transmitted through the network without causing queuing delay. It does this by continuously probing the network with small bursts of packets and observing the resulting RTT variations.

Based on the observed bottleneck bandwidth and RTT, BBR dynamically adjusts the sending rate to maximize network utilization without causing excessive queuing delay. It strives to maintain a low queue occupancy at the bottleneck link to minimize delay and maximize throughput.

BBR utilizes a congestion model called the "BBR gain cycle" to calculate the sending rate. It consists of four phases: Startup, Drain, ProbeBW, and ProbeRTT. During Startup, BBR rapidly increases the sending rate until it detects congestion. In the Drain phase, it reduces the rate to empty the queue. The ProbeBW phase explores the available bandwidth, and the ProbeRTT phase refines the RTT estimation for more accurate control.

By continuously monitoring the bottleneck bandwidth and RTT, BBR adapts its sending rate in real-time to maintain optimal network utilization while minimizing queuing delay. This allows BBR to achieve higher throughput and better performance in diverse network conditions, making it particularly effective for modern high-speed networks and long-distance connections.

### Q4. "What's the difference between these three algorithms."

Reno, New Reno, and BBR are three different congestion control algorithms used in TCP (Transmission Control Protocol) for managing data transmission over computer networks. Here are the key differences between these algorithms:

1. Reno:
   Reno is one of the earliest and most widely deployed congestion control algorithms. It uses a combination of slow-start and congestion avoidance techniques to regulate the sending rate. When congestion is detected based on packet loss, Reno reduces the congestion window (cwnd) and enters a slow-start phase, gradually increasing cwnd to probe for available network capacity. It then enters congestion avoidance, incrementing cwnd more cautiously to maintain network stability. Reno is known for its simplicity but can suffer from global synchronization issues when multiple connections sharing a bottleneck link experience simultaneous congestion events.

2. New Reno:
   New Reno is an enhancement to the Reno algorithm designed to address the global synchronization problem. When a packet loss is detected, New Reno enters a fast recovery phase instead of slow-start. In fast recovery, it increases the congestion window by a smaller value and relies on receiving duplicate acknowledgments (ACKs) to retransmit lost packets. This approach allows New Reno to recover from losses more quickly and avoid unnecessary slow-start episodes, improving overall network performance. New Reno is backward compatible with Reno and is widely used in modern TCP implementations.

3. BBR (Bottleneck Bandwidth and Round-Trip Time):
   BBR is a congestion control algorithm developed by Google. It takes a different approach from Reno and New Reno by focusing on the estimation of bottleneck bandwidth and round-trip time (RTT) to optimize network performance. BBR dynamically adjusts the sending rate based on these metrics rather than relying solely on packet loss as an indicator of congestion. By continuously probing the network and observing RTT variations, BBR estimates the available bottleneck bandwidth and adjusts the sending rate to maximize network utilization without causing excessive queuing delay. BBR aims to achieve high throughput and low delay by maintaining a low queue occupancy at the bottleneck link. It is particularly effective for high-speed networks and long-distance connections.

In summary, Reno is a classic congestion control algorithm, New Reno is an enhancement to address global synchronization, and BBR is a modern algorithm that optimizes throughput and delay by dynamically adjusting the sending rate based on bottleneck bandwidth and RTT measurements.

### Q5. "Analyze and describe these three algorithms."

First phase of these algorithms are almost same. We have possible loss in all of them but BBR handle it better than others. Also BBR has the best throughput and lowest RTT. When loss happens, BBR goes to Drain phase and decrease the cwnd slowly. But in Reno and NewReno they go to Fast Recovery phase and cwnd halves. If we have multiple loss in one window, NewReno is better than Reno because it doesn't halve the cwnd again and again. But BBR is better than both of them because it doesn't halve the cwnd at all.

Best algorithm for high-speed networks is BBR as we see in images. But for low-speed networks, Reno and NewReno could be as good as BBR.


&emsp;

## Reno
![Alt text](images/reno.jpg)

&emsp;

## NewReno
![Alt text](images/new_reno.jpg)

&emsp;

## BBR
![Alt text](images/bbr.jpg)

### Q6. "Name some of the congestion control algorithms that are not mentioned in this project."

One congestion control algorithm that is commonly used in computer networks is called "Cubic." Cubic is a TCP congestion control algorithm that aims to provide high throughput and fairness in network traffic. It uses a cubic function to determine the appropriate congestion window size based on network conditions.

Cubic is designed to be scalable and efficient in modern high-speed networks. It adjusts the congestion window dynamically by estimating the network's bandwidth and round-trip time. The algorithm strives to achieve fairness by adapting the sending rate to congestion signals while maximizing network utilization.

Cubic has been widely implemented in various operating systems and networking frameworks, and it has become a standard congestion control algorithm in many modern network stacks.

Cubic has several advantages and characteristics that make it a superior congestion control algorithm in certain scenarios. Here are a few reasons why Cubic may be considered superior to other congestion control algorithms in specific criteria:

1. Scalability: Cubic is designed to perform well in high-speed networks, making it particularly suitable for modern networks with large bandwidth capacities. It achieves scalability by using a cubic function to control the congestion window size, allowing it to adapt to varying network conditions more efficiently.

2. Fairness: Cubic aims to provide fairness among competing flows sharing the same network resources. It achieves fairness by adapting the sending rate based on congestion signals and the available bandwidth. This ensures that all flows get a fair share of the network capacity, preventing a single flow from dominating the network.

3. Responsiveness: Cubic is designed to be responsive to changes in network conditions. It uses an algorithm that approximates the congestion window growth in a cubic manner, allowing it to react quickly to network congestion or changes in available bandwidth. This responsiveness helps to mitigate congestion and maintain optimal network performance.

4. Stability: Cubic is known for its stability and low packet loss rates. It uses a window growth model that results in a smooth increase in sending rates, preventing sudden bursts or rapid drops in congestion window size. This stability leads to improved network performance and reduced packet loss.

It's important to note that the effectiveness of congestion control algorithms can vary depending on the network environment and specific use cases. Different algorithms may perform better under different conditions, and the choice of algorithm often depends on factors such as network topology, latency, and available bandwidth. Network administrators and researchers continually evaluate and develop new congestion control algorithms to address the evolving needs of modern networks.
