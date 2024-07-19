package main

import (
	"strconv"
)

type Node struct {
	id   int    // Node ID
	cnt  int    // Number of blocks of this node
	tail *block // Tail of the chain
	evil bool   // Evil node
}

func NewNode(id int, evil bool) *Node {
	return &Node{
		id:   id,
		cnt:  0,
		tail: nil,
		evil: evil,
	}
}

func (node *Node) Run(tail_chan chan *block, round_chan chan bool) {
	for {
		// Evil node's attack hasn't begun
		if node.evil && !evil_attack {
			continue
		}
		// Wait for round start-signal
		<-round_chan

		if node.evil {
			// Do not care the broadcast
			<-tail_chan
			// Set computation power of node
			q := evil_node_power
			// Try to generate new block
			for i := 0; i < q; i++ {
				if evil_r_block == nil && Random(hash_probability) {
					// Protect the very one block
					mu.Lock()
					new_block := block{
						id:       "N" + strconv.Itoa(node.id) + "-" + "B" + strconv.Itoa(node.cnt),
						source:   node.id,
						previous: node.tail,
						time:     round,
					}
					node.tail = &new_block
					evil_r_block = &new_block
					node.cnt++
					mu.Unlock()
					break // For each round only one block
				}
			}
		} else {
			// Update the longest chain
			node.tail = <-tail_chan
			// Set computation power of node
			q := chances_per_round
			// Try to generate new block
			for i := 0; i < q; i++ {
				if r_block == nil && Random(hash_probability) {
					// Protect the very one block
					mu.Lock()
					new_block := block{
						id:       "N" + strconv.Itoa(node.id) + "-" + "B" + strconv.Itoa(node.cnt),
						source:   node.id,
						previous: node.tail,
						time:     round,
					}
					node.tail = &new_block
					r_block = &new_block
					node.cnt++
					mu.Unlock()
					break // For each round only one block
				}
			}
		}

		// Round finished
		wg.Done()
	}
}
