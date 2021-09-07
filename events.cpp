#include <bits/stdc++.h>
#include "node.h"
#include "utils.h"
using namespace std;
#define pp pair<double, event>
#define fi first
#define se second
int mining_fee = 50;
int max_txn_amt = 100;
double size_txn = 8.0;
double size_empty_block = 8.0; 
vector<txn> txns;
vector<block> blocks;
int num2;
///// INITIALISES EVENT DATA STRUCTURES /////
void init_event(int n){
	num2 = n;
	txns.clear();
	blocks.clear();
}
//// CREATES A TXN AND RETURNS TXN ID ////
int create_txn(int x, int y){
	txn t;
	t.id = txns.size();
	t.idx = x;
	t.idy = y;
	if(x == -1){
		// COINBASE TXN
		t.c = mining_fee;
	}else{
		t.c = get_random_int(max_txn_amt) + 1;
	}
	txns.push_back(t);
	return t.id;
}
//// CREATES A BLOCK AND RETURNS BLOCK ID ////
int create_block(int x, int par){
	block new_block;
	new_block.id = blocks.size();
	new_block.parent_id = par;
	new_block.idx = x;
	if (par == -1){
		new_block.chain_length = 1;
	}else{
		new_block.chain_length = blocks[par].chain_length + 1;
	}
	new_block.txn_ids.clear();
	blocks.push_back(new_block);
	return new_block.id;
}
//// CREATES SEND TXN EVENT ////
event create_send_txn_event(int x){
	event e;
	e.parent_node_id = -1;
	e.type = 0;
	e.idx = x;
	return e;
}
//// CREATES RECEIVE TXN EVENT ////
event create_receive_txn_event(int x,int par,int txn_id){
	event e;
	e.parent_node_id = par;
	e.type = 0;
	e.idx = x;
	e.txn_id = txn_id;
	return e;
}
//// CREATES SEND BLOCK EVENT ////
event create_send_block_event(int x){
	event e;
	e.parent_node_id = -1;
	e.type = 1;
	e.idx = x;
	e.block_id = get_root_block_id(x);
	return e;
}
//// CREATES RECEIVE BLOCK EVENT ////
event create_receive_block_event(int x,int par,int block_id){
	event e;
	e.parent_node_id = par;
	e.type = 1;
	e.idx = x;
	e.block_id = block_id;
	return e;
}
//// FIRST THE PEER RECEIVES TXN AND THEN IT SENDS TO ALL EXCEPT PARENT ////
void send_txn(int txn_id,int x,multiset<pp> &simulator,double start_time,int par_node_id){
	receive_txn(x, txn_id);
	for(int i=0;i<num2;i++){
		if(i == par_node_id || i == x){
			continue;
		}
		edge e = get_edge(x,i);
		if(e.present){
			double d_mean = 96/e.c;
			double d = get_expo_dist(d_mean);
			double time_receive = e.p + size_txn / e.c + d;
			event et = create_receive_txn_event(i,x,txn_id);
			simulator.insert({time_receive + start_time, et});
		}
	}
}
int balance[1000];
///// ADDS TXNS TO BLOCK (CAN CREATE AN INVALID BLOCK) //////
bool add_invalid_txns_to_block(int x,int block_id){
	for(int i=0;i<num2;i++){
		balance[i]=0;
	}
	vector<int> txn_seen(txns.size(),0);
	vector<int> peer_txn_ids = get_peer_txn_ids(x);
	int root_id = get_root_block_id(x);
	while(root_id != -1){
		for(auto tid:blocks[root_id].txn_ids){
			txn_seen[tid] = 1;
			if(txns[tid].idx != -1){
				balance[txns[tid].idx] -= txns[tid].c;
			}
			balance[txns[tid].idy] += txns[tid].c;
		}
		root_id = blocks[root_id].parent_id;
	}
	int coinbase_txn_id = create_txn(-1,x);
	balance[x] += mining_fee;
	blocks[block_id].txn_ids.push_back(coinbase_txn_id); /// ADDS COINBASE TXN
	for(auto tid:peer_txn_ids){
		if(blocks[block_id].txn_ids.size() == 999){
			break;
		}
		if(!txn_seen[tid]){
			balance[txns[tid].idx] -= txns[tid].c;
			balance[txns[tid].idy] += txns[tid].c;
			blocks[block_id].txn_ids.push_back(tid);
			if(balance[txns[tid].idx]<0){
				return true; //// INVALID BLOCK IS CREATED
			}
		}
	}
	return false; //// VALID BLOCK IS CREATED
}
bool is_block_chain_valid(int root_id){
	if(root_id == -1){
		return true;
	}
	if(!is_block_chain_valid(blocks[root_id].parent_id)){
		return false;
	}
	for(auto tid:blocks[root_id].txn_ids){
		if(txns[tid].idx != -1){
			balance[txns[tid].idx] -= txns[tid].c;
		}
		balance[txns[tid].idy] += txns[tid].c;
	}
	for(int i=0;i<num2;i++){
		if(balance[i]<0){
			return false;
		}
	}
	return true;
}
///// CHECKS IF A BLOCK IS VALID OR NOT ////
bool is_block_valid(int block_id){
	for(int i=0;i<num2;i++){
		balance[i]=0;
	}
	return is_block_chain_valid(block_id);
}
//// ADD TXNS TO CREATE A VALID BLOCK /////
void add_txns_to_block(int x,int block_id){
	for(int i=0;i<num2;i++){
		balance[i]=0;
	}
	vector<int> txn_seen(txns.size(),0);
	vector<int> peer_txn_ids = get_peer_txn_ids(x);
	int root_id = get_root_block_id(x);
	while(root_id != -1){
		for(auto tid:blocks[root_id].txn_ids){
			txn_seen[tid] = 1;
			if(txns[tid].idx != -1){
				balance[txns[tid].idx] -= txns[tid].c;
			}
			balance[txns[tid].idy] += txns[tid].c;
		}
		root_id = blocks[root_id].parent_id;
	}
	int coinbase_txn_id = create_txn(-1,x);
	balance[x] += mining_fee;
	blocks[block_id].txn_ids.push_back(coinbase_txn_id); // ADDS COINBASE TXN
	for(auto tid:peer_txn_ids){
		if(blocks[block_id].txn_ids.size() == 999){
			break;
		}
		if(!txn_seen[tid] && balance[txns[tid].idx]>=txns[tid].c){
			// VALID TXN
			balance[txns[tid].idx] -= txns[tid].c;
			balance[txns[tid].idy] += txns[tid].c;
			blocks[block_id].txn_ids.push_back(tid);
		}
	}
}
//// FIRST RECEIVES A BLOCK AND THEN SENDS IT TO OTHER PEERS EXCEPT PARENT ////
void send_block(int block_id,int x,multiset<pp> &simulator,double start_time,int par_node_id,int mean_tk){
	receive_block(x, block_id, start_time);
	int root_id = get_root_block_id(x);
	if(blocks[block_id].chain_length > blocks[root_id].chain_length){
		// ROOT BLOCK ID OF THE PEER IS UPDATED 
		update_root_block_id(x, block_id);
		event e = create_send_block_event(x);
		simulator.insert({get_expo_dist(mean_tk) + start_time, e});
	}
	for(int i=0;i<num2;i++){
		if(i == par_node_id || i == x){
			continue;
		}
		edge e = get_edge(x,i);
		if(e.present){
			double d_mean = 96/e.c;
			double d = get_expo_dist(d_mean);
			double size_block = size_empty_block + size_txn * (blocks[block_id].txn_ids.size());
			double time_receive = e.p + size_block / e.c + d;
			event et = create_receive_block_event(i,x,block_id);
			simulator.insert({time_receive + start_time, et});
		}
	}	
}
void print_tree(){
	for(auto b:blocks){
		cout<<b.id<<" "<<b.parent_id<<endl;
	}
}
////////// THESE FUNCTIONS ARE CREATED FOR VISUALISATION PURPOSES ////////////
int total_blocks_of_peer(int x){
	int cnt=0;
	for(auto b:blocks){
		if(b.idx == x){
			cnt++;
		}
	}
	return cnt;
}
int blocks_of_peer_in_chain(int x,int root_id){
	int cnt=0;
	while(root_id != -1){
		if(blocks[root_id].idx == x){
			cnt++;
		}
		root_id = blocks[root_id].parent_id;
	}
	return cnt;
}
int get_global_root_block_id(){
	int root_id = -1;
	int root_chain_length = -1;
	for(auto b:blocks){
		if(b.chain_length > root_chain_length){
			root_id = b.id;
			root_chain_length = b.chain_length;
		}
	}
	return root_id;
}
void print_blocks_received(int x)
{
	vector<int> peer_block_ids = get_peer_block_ids(x);
	vector<double> peer_time_arrival = get_peer_time_arrivals(x);
	cout<<"BlockHash BlockNum TimeOfArrival ParentBlockHash"<<endl;
	for(int i=0;i<peer_block_ids.size();i++){
		int block_id = peer_block_ids[i];
		cout<<block_id<<" "<<blocks[block_id].chain_length-1<<" "<<peer_time_arrival[i]<<" "<<blocks[block_id].parent_id<<endl;
	}
}
double get_branching_in_block_tree()
{
	int root_id = get_global_root_block_id();
	return 1.0 * blocks.size() / blocks[root_id].chain_length;
}
double get_avg_txns_in_longest_chain()
{
	int root_id = get_global_root_block_id();
	int num_txns = 0;
	int num_blocks = 0;
	while(root_id != -1){
		num_blocks++;
		num_txns+=blocks[root_id].txn_ids.size();
		root_id = blocks[root_id].parent_id;
	}
	return (1.0 * num_txns / num_blocks);
}