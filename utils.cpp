#include <bits/stdc++.h>
#include "utils.h"
#define ll unsigned long long
using namespace std;
default_random_engine generator(time(0));
uniform_real_distribution<double> unif_dist(0.0, 1.0);
double get_expo_dist(double mean)
{
  exponential_distribution<double> expo_dist(1.0 / mean);
  return expo_dist(generator);
}
double get_uniform_0to1()
{
  return unif_dist(generator);
}
int get_random_int(int s)
{
  return rand() % s;
}
ll get_time_in_ms(){
  return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();  
}