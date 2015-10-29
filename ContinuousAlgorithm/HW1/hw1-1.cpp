/*
Copyright (c) 2015 @myuuuuun
https://github.com/myuuuuun
*/
#include <iostream>
#include <limits>
#include <cmath>
using namespace std;

int main(void)
{
  float x = 0.02;
  cout.precision(numeric_limits<float>::max_digits10);
  cout << 1 - cos(x) << endl;
  cout << 2 * pow(sin(x / 2), 2) << endl;

  return 0;
}