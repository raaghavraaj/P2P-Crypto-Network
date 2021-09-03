#include <bits/stdc++.h>
#include "node.h"
#include "utils.h"
using namespace std;
const int max_num = 1000;
node peers_array[max_num];
edge peers_edges[max_num][max_num];
int num;
double prob_choosing_edge;
void init_node(int n, double z)
{
	srand(time(0));
	num = n;
	prob_choosing_edge = 3 * log(num) / num;
	int genesis_id = create_block(-1, -1);
	for (int i = 0; i < num; i++)
	{
		receive_block(i, genesis_id);
		double random_0to1 = get_uniform_0to1();
		peers_array[i].speed = random_0to1 > (z / 100);
		peers_array[i].root_id = genesis_id;
		peers_array[i].txn_ids.clear();
		peers_array[i].block_ids.clear();
	}
}
void create_connected_graph()
{
	int cnt = 0;
	for (int i = 0; i < num; i++)
	{
		for (int j = i + 1; j < num; j++)
		{
			double random_0to1 = get_uniform_0to1();
			if (random_0to1 > prob_choosing_edge)
			{
				peers_edges[i][j].present = 0;
				peers_edges[j][i].present = 0;
			}
			else
			{
				peers_edges[i][j].present = 1;
				peers_edges[i][j].p = get_uniform_0to1() * 490 + 10;
				if (peers_array[i].speed == 1 && peers_array[j].speed == 1)
				{
					peers_edges[i][j].c = 102.4;
				}
				else
				{
					peers_edges[i][j].c = 5.12;
				}
				peers_edges[j][i] = peers_edges[i][j];
			}
		}
	}
}
int visited[max_num];
void dfs(int root)
{
	for (int i = 0; i < num; i++)
	{
		if (!visited[i] && peers_edges[root][i].present)
		{
			visited[i] = 1;
			dfs(i);
		}
	}
}
bool is_connected()
{
	for (int i = 0; i < num; i++)
	{
		visited[i] = 0;
	}
	visited[0] = 1;
	dfs(0);
	for (int i = 0; i < num; i++)
	{
		if (!visited[i])
		{
			return false;
		}
	}
	return true;
}
int get_root_block_id(int x)
{
	return peers_array[x].root_id;
}
void update_root_block_id(int x, int block_id)
{
	peers_array[x].root_id = block_id;
}
vector<int> get_peer_txn_ids(int x)
{
	return peers_array[x].txn_ids;
}
void receive_txn(int x, int txn_id)
{
	peers_array[x].txn_ids.push_back(txn_id);
}
bool is_txn_received(int x, int txn_id)
{
	for (auto tid : peers_array[x].txn_ids)
	{
		if (tid == txn_id)
		{
			return true;
		}
	}
	return false;
}
void receive_block(int x, int block_id)
{
	peers_array[x].block_ids.push_back(block_id);
}
bool is_block_received(int x, int block_id)
{
	for (auto bid : peers_array[x].block_ids)
	{
		if (bid == block_id)
		{
			return true;
		}
	}
	return false;
}
edge get_edge(int x, int y)
{
	return peers_edges[x][y];
}