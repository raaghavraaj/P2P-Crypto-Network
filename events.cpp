#include <bits/stdc++.h>
#include "node.h"
#include "utils.h"
using namespace std;
int mining_fee = 50;
vector<int> txns;
block *create_block(block *par, int num)
{
	block *new_block = new block;
	new_block->parent = par;
	if (par == NULL)
	{
		new_block->chain_length = 1;
	}
	else
	{
		new_block->chain_length = par->chain_length + 1;
	}
	new_block->txn_ids.clear();
	for (int i = 0; i < num; i++)
	{
		new_block->received[i] = 0;
	}
	return new_block;
}
int create_txn(int x, int y, int num)
{
	txn t;
	t.idx = x;
	t.idy = y;
	for (int i = 0; i < num; i++)
	{
		t.received[i] = 0;
	}
	t.received[x] = 1;
	int bal = get_balance(x);
	if (bal == 0)
	{
		return -1;
	}
	t.c = get_random_int(bal) + 1;
	txns.push_back(t);
	return txns.size()-1;
}
/*event::bool operator<(const event &e1, const event &e2)
{
	return e1.idx < e2.idx;
}*/