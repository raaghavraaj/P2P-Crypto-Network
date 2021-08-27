#include<bits/stdc++.h>
using namespace std;
struct txn{
	int idx;
	int idy;
	int c;
	int received[1000];
};
struct block{
	block* parent;
	int chain_length;
	vector<int> txn_ids;
	int received[1000];
};
struct event{
	int action; // 0 receive, 1 send
	int type; // 0 txn, 1 block
	txn * t;
	block * b;
};
block * create_block(block * par,int num);
txn * create_txn(int x,int num);