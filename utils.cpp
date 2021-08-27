#include <bits/stdc++.h>
#include "utils.h"
using namespace std;
default_random_engine generator(time(0));
uniform_real_distribution<double> unif_dist(0.0, 1.0);
double get_expo_dist(double lam)
{
  exponential_distribution<double> expo_dist(lam);
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