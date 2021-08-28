#include <bits/stdc++.h>
#include "node.h"
#include "utils.h"
using namespace std;
#define pp pair<double, event>
#define fi first
#define se second
double tx = 1000;
double mean_tk[1000];
int main()
{
	while (true)
	{
		int num = 100;
		init(num);
		for(int i = 0;i < num;i++)
		{
			mean_tk[i] = 5000 + 50*i;
		}
		while (true)
		{
			create_connected_graph();
			if (is_connected())
			{
				break;
			}
		}
		multiset<pp> simulator;
		for (int i = 0; i < num; i++)
		{
			event e;
			e.action = 1;
			e.type = 0;
			e.idx = i;
			simulator.insert({get_expo_dist(1.0 / tx), e});
		}
		for (int i = 0; i < num; i++)
		{
			event e;
			e.action = 1;
			e.type = 1;
			e.idx = i;
			e.b = get_root_block(i);
			simulator.insert({get_expo_dist(1.0 / mean_tk[i]), e});
		}
		while (!simulator.empty())
		{
			pp top = (*simulator.begin());
			// while(){

			// }
		}
		break;
	}
}