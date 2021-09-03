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
void init_event(int n)
{
	num2 = n;
	txns.clear();
	blocks.clear();
}
int create_txn(int x, int y)
{
	txn t;
	t.id = txns.size();
	t.idx = x;
	t.idy = y;
	if (x == -1)
	{
		t.c = mining_fee;
	}
	else
	{
		t.c = get_random_int(max_txn_amt) + 1;
	}
	txns.push_back(t);
	return t.id;
}
int create_block(int x, int par)
{
	block new_block;
	new_block.id = blocks.size();
	new_block.parent_id = par;
	new_block.idx = x;
	if (par == -1)
	{
		new_block.chain_length = 1;
	}
	else
	{
		new_block.chain_length = blocks[par].chain_length + 1;
	}
	new_block.txn_ids.clear();
	blocks.push_back(new_block);
	return new_block.id;
}
event create_send_txn_event(int x)
{
	event e;
	e.parent_node_id = -1;
	e.type = 0;
	e.idx = x;
	return e;
}
event create_receive_txn_event(int x, int par, int txn_id)
{
	event e;
	e.parent_node_id = par;
	e.type = 0;
	e.idx = x;
	e.txn_id = txn_id;
	return e;
}
event create_send_block_event(int x)
{
	event e;
	e.parent_node_id = -1;
	e.type = 1;
	e.idx = x;
	e.block_id = get_root_block_id(x);
	return e;
}
event create_receive_block_event(int x, int par, int block_id)
{
	event e;
	e.parent_node_id = par;
	e.type = 1;
	e.idx = x;
	e.block_id = block_id;
	return e;
}
void send_txn(int txn_id, int x, multiset<pp> &simulator, double start_time, int par_node_id)
{
	receive_txn(x, txn_id);
	for (int i = 0; i < num2; i++)
	{
		if (i == par_node_id || i == x)
		{
			continue;
		}
		edge e = get_edge(x, i);
		if (e.present)
		{
			double d_mean = 96 / e.c;
			double d = get_expo_dist(d_mean);
			double time_receive = e.p + size_txn / e.c + d;
			event et = create_receive_txn_event(i, x, txn_id);
			simulator.insert({time_receive + start_time, et});
		}
	}
}
int balance[1000];
void add_txns_to_block(int x, int block_id)
{
	for (int i = 0; i < num2; i++)
	{
		balance[i] = 0;
	}
	vector<int> txn_seen(txns.size(), 0);
	vector<int> peer_txn_ids = get_peer_txn_ids(x);
	int root_id = get_root_block_id(x);
	while (root_id != -1)
	{
		for (auto tid : blocks[root_id].txn_ids)
		{
			txn_seen[tid] = 1;
			if (txns[tid].idx != -1)
			{
				balance[txns[tid].idx] -= txns[tid].c;
			}
			balance[txns[tid].idy] += txns[tid].c;
		}
		root_id = blocks[root_id].parent_id;
	}
	int coinbase_txn_id = create_txn(-1, x);
	balance[x] += mining_fee;
	blocks[block_id].txn_ids.push_back(coinbase_txn_id);
	for (auto tid : peer_txn_ids)
	{
		if (!txn_seen[tid] && balance[txns[tid].idx] >= txns[tid].c)
		{
			balance[txns[tid].idx] -= txns[tid].c;
			balance[txns[tid].idy] -= txns[tid].c;
			blocks[block_id].txn_ids.push_back(tid);
		}
	}
}
void send_block(int block_id, int x, multiset<pp> &simulator, double start_time, int par_node_id)
{
	receive_block(x, block_id);
	int root_id = get_root_block_id(x);
	if (blocks[block_id].chain_length > blocks[root_id].chain_length)
	{
		update_root_block_id(x, block_id);
	}
	for (int i = 0; i < num2; i++)
	{
		if (i == par_node_id || i == x)
		{
			continue;
		}
		edge e = get_edge(x, i);
		if (e.present)
		{
			double d_mean = 96 / e.c;
			double d = get_expo_dist(d_mean);
			double size_block = size_empty_block + size_txn * (blocks[block_id].txn_ids.size());
			double time_receive = e.p + size_block / e.c + d;
			event et = create_receive_block_event(i, x, block_id);
			simulator.insert({time_receive + start_time, et});
		}
	}
}