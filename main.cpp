#include<bits/stdc++.h>
#include "node.h"
#include "utils.h"
using namespace std;
#define pp pair<double,event>
#define fi first
#define se second
double tx=1;
int main(){
	while(true){
		int num=100;
		init(num);
		while(true){
			create_connected_graph();
			if(is_connected()){
				break;
			}
		}
		multiset<pp> simulator;
		for(int i=0;i<num;i++){
			event e;
			e.action=1;
			e.type=0;
			e.t=create_txn(i,num);
			if(e.t!=NULL){
				simulator.insert({get_expo_dist(1/tx),e});
			}
		}
		while(!simulator.empty()){
			pp top=(*simulator.begin());
			while(){

			}

		}
		break;
	}
}