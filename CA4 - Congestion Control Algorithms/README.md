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

### Reno Class

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
