#include "events.h"
struct edge{
	int present; // 0 DNE, 1 exist
	double p;
	double c;
};
struct node{
	int speed; // 0 slow, 1 fast
	int root_id; // root block id
	vector<int> txn_ids; // txn ids received
	vector<int> block_ids; // block ids received
};
void init_node(int n, double z);
void create_connected_graph();
bool is_connected();
int get_root_block_id(int x);
void update_root_block_id(int x, int block_id);
void receive_txn(int x, int txn_id);
bool is_txn_received(int x,int txn_id);
void receive_block(int x, int block_id);
bool is_block_received(int x,int block_id);
edge get_edge(int x,int y);
vector<int> get_peer_txn_ids(int x);