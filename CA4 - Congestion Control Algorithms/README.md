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

### Q3. "Describe BBR algorithm shortly."

### Q4. "What's the difference between these three algorithms."

### Q5. "Analyze and describe these three algorithms."

### Q6. "Name some of the congestion control algorithms that are not mentioned in this project."
