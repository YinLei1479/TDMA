
# TDMA_Model

A simple TDMA protocol model.

It detects the number of rx that a node's tx in the network can perceive to determine how many so-called "time slots" there are. The nodes are sent periodically in the order of node ids.


## Simulation environment
OPNET 14.5
## Cons
- The multi-hop environment is not supported. This requires redefining the number of time slots and determining the time slots occupied by this node.

- The packet is not defined, no_packet_form. This requires you to re-determine the packet format and destination address yourself.
