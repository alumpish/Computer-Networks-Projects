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

### Q4. "What's the difference between these three algorithms."

### Q5. "Analyze and describe these three algorithms."

### Q6. "Name some of the congestion control algorithms that are not mentioned in this project."
