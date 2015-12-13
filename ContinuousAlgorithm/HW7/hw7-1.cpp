/*
Copyright (c) 2015 @myuuuuun
https://github.com/myuuuuun
*/
#include <iostream>
#include <iomanip>
#include <array>
#include <cmath>
#include <limits>
const double EPS = 1.0e-12;
using namespace std;

template <size_t row, size_t col>
  using array_2d = std::array<std::array<double, col>, row>;

// matrix class
template <size_t r_num, size_t c_num> class matrix{
  const int r = r_num;
  const int c = c_num;
  
public:
  array_2d<r_num, c_num> mat;
  const int row(void){return r;};
  const int col(void){return c;};
  matrix();
  matrix(array_2d<r_num, c_num> &base_mat);
  const matrix<r_num, c_num> &copy(const matrix<r_num, c_num> &mat1);
  const matrix<r_num, c_num> &identity(void);
  const matrix<r_num, c_num> &zeros(void);
  const matrix<r_num, c_num> &ones(void);
  const matrix<r_num, c_num> &transpose(const matrix<c_num, r_num> &mat1);
  std::array<double, c_num>& operator[](const int i);
  const bool operator==(const matrix<r_num, c_num> &mat1);
  const bool operator!=(const matrix<r_num, c_num> &mat1);
  const matrix<r_num, c_num> &operator=(const matrix<r_num, c_num> &mat1);
  const matrix<r_num, c_num> operator+(const matrix<r_num, c_num> &mat1);
  const matrix<r_num, c_num> operator+(const double num);
  const matrix<r_num, c_num> operator-(const matrix<r_num, c_num> &mat1);
  const matrix<r_num, c_num> operator-(const double num);
  template <size_t c2_num> 
    const matrix<r_num, c2_num> operator*(const matrix<c_num, c2_num> &mat1);
  const matrix<r_num, c_num> operator*(const double num);
  const matrix<r_num, c_num> operator/(const double num);
  
};

// constructer
template <size_t r_num, size_t c_num>
matrix<r_num, c_num>::matrix(){};

template <size_t r_num, size_t c_num>
matrix<r_num, c_num>::matrix(array_2d<r_num, c_num> &base_mat)
{
  for(int i=0; i<r_num; ++i){
    for(int j=0; j<c_num; ++j){
      (*this).mat[i][j] = base_mat[i][j];
    }
  }
};


// print function for matrix
template <size_t r_num, size_t c_num>
void print(matrix<r_num, c_num> mat)
{
  cout.precision(8);
  for(int i=0; i<mat.row(); ++i){
    for(int j=0; j<mat.col(); ++j){
      cout << setw(9) << mat[i][j];
    }
    cout << endl;
  }
  cout << endl;
}


// print function for std::vector
template <class Type, size_t size>
void print(std::array<Type, size> array)
{
  cout.precision(8);
  for(int i=0; i<size; ++i){
    cout << setw(9) << array[i] << endl;
  }
  cout << endl;
}


// deep copy
template <size_t r_num, size_t c_num> 
const matrix<r_num, c_num> &matrix<r_num, c_num>::copy(const matrix<r_num, c_num> &mat1)
{
  for(int i=0; i<(*this).row(); ++i){
    for(int j=0; j<(*this).col(); ++j){
      (*this).mat[i][j] = mat1.mat[i][j];
    }
  }
  return *this;
}


// identity matrix
template <size_t r_num, size_t c_num> 
const matrix<r_num, c_num> &matrix<r_num, c_num>::identity(void)
{
  for(int i=0; i<(*this).row(); ++i){
    for(int j=0; j<(*this).col(); ++j){
      if(i == j){
        (*this).mat[i][j] = 1;
      }
      else{
        (*this).mat[i][j] = 0;
      }
    }
  }
  return *this;
}


// all-zero matrix
template <size_t r_num, size_t c_num> 
const matrix<r_num, c_num> &matrix<r_num, c_num>::zeros(void)
{
  for(int i=0; i<(*this).row(); ++i){
    for(int j=0; j<(*this).col(); ++j){
      (*this).mat[i][j] = 0;
    }
  }
  return *this;
}


// all-one matrix
template <size_t r_num, size_t c_num> 
const matrix<r_num, c_num> &matrix<r_num, c_num>::ones(void)
{
  for(int i=0; i<(*this).row(); ++i){
    for(int j=0; j<(*this).col(); ++j){
      (*this).mat[i][j] = 1;
    }
  }
  return *this;
}


// transpose matrix
template <size_t r_num, size_t c_num> 
const matrix<r_num, c_num> &matrix<r_num, c_num>::transpose(const matrix<c_num, r_num> &mat1)
{
  for(int i=0; i<(*this).row(); ++i){
    for(int j=0; j<(*this).col(); ++j){
      (*this).mat[i][j] = mat1.mat[j][i];
    }
  }
  return *this;
}


// [] operator
template <size_t r_num, size_t c_num> 
std::array<double, c_num> &matrix<r_num, c_num>::operator[](const int i)
{ 
  return mat[i];
}


// == operator
template <size_t r_num, size_t c_num> 
const bool matrix<r_num, c_num>::operator==(const matrix<r_num, c_num> &mat1)
{
  for(int i=0; i<(*this).row(); ++i){
    for(int j=0; j<(*this).col(); ++j){
      double cond = ((*this).mat[i][j] - mat1.mat[i][j]) / (*this).mat[i][j];
      if(abs(cond) > EPS ){
        return false;
      }
    }
  }
  return true;
}


// != operator
template <size_t r_num, size_t c_num> 
const bool matrix<r_num, c_num>::operator!=(const matrix<r_num, c_num> &mat1)
{
  return !(*this == mat1);
}


// = operator
template <size_t r_num, size_t c_num> 
const matrix<r_num, c_num> &matrix<r_num, c_num>::operator=(const matrix<r_num, c_num> &mat1)
{
  for(int i=0; i<(*this).row(); ++i){
    for(int j=0; j<(*this).col(); ++j){
      (*this).mat[i][j] = mat1.mat[i][j];
    }
  }
  return *this;
}


// + operator
template <size_t r_num, size_t c_num> 
const matrix<r_num, c_num> matrix<r_num, c_num>::operator+(const matrix<r_num, c_num> &mat1)
{
  matrix<r_num, c_num> result;
  for(int i=0; i<(*this).row(); ++i){
    for(int j=0; j<(*this).col(); ++j){
      result[i][j] = (*this).mat[i][j] + mat1.mat[i][j];
    }
  }
  return result;
}

template <size_t r_num, size_t c_num> 
const matrix<r_num, c_num> matrix<r_num, c_num>::operator+(const double num)
{
  matrix<r_num, c_num> result;
  for(int i=0; i<(*this).row(); ++i){
    for(int j=0; j<(*this).col(); ++j){
      result[i][j] = (*this).mat[i][j] + num;
    }
  }
  return result;
}

template <size_t r_num, size_t c_num> 
const matrix<r_num, c_num> operator+(const int num, const matrix<r_num, c_num> &mat1)
{
  matrix<r_num, c_num> result;
  for(int i=0; i<r_num; ++i){
    for(int j=0; j<c_num; ++j){
      result[i][j] = num + mat1.mat[i][j];
    }
  }
  return result;
}


// - operator
template <size_t r_num, size_t c_num> 
const matrix<r_num, c_num> matrix<r_num, c_num>::operator-(const matrix<r_num, c_num> &mat1)
{
  matrix<r_num, c_num> result;
  for(int i=0; i<(*this).row(); ++i){
    for(int j=0; j<(*this).col(); ++j){
      result[i][j] = (*this).mat[i][j] - mat1.mat[i][j];
    }
  }
  return result;
}

template <size_t r_num, size_t c_num> 
const matrix<r_num, c_num> matrix<r_num, c_num>::operator-(const double num)
{
  matrix<r_num, c_num> result;
  for(int i=0; i<(*this).row(); ++i){
    for(int j=0; j<(*this).col(); ++j){
      result[i][j] = (*this).mat[i][j] - num;
    }
  }
  return result;
}

template <size_t r_num, size_t c_num> 
const matrix<r_num, c_num> operator-(const int num, const matrix<r_num, c_num> &mat1)
{
  matrix<r_num, c_num> result;
  for(int i=0; i<r_num; ++i){
    for(int j=0; j<c_num; ++j){
      result[i][j] = num - mat1.mat[i][j];
    }
  }
  return result;
}


// * operator
template <size_t r_num, size_t c_num>
template <size_t c2_num> 
const matrix<r_num, c2_num> matrix<r_num, c_num>::operator*(const matrix<c_num, c2_num> &mat1)
{
  matrix<r_num, c2_num> result;

  for(int i=0; i<result.row(); ++i){
    for(int j=0; j<result.col(); ++j){
      for(int k=0; k<(*this).col(); ++k){
        result[i][j] += (*this).mat[i][k] * mat1.mat[k][j];
      }
    }
  }
  return result;
}

template <size_t r_num, size_t c_num> 
const matrix<r_num, c_num> matrix<r_num, c_num>::operator*(const double num)
{
  matrix<r_num, c_num> result;
  for(int i=0; i<(*this).row(); ++i){
    for(int j=0; j<(*this).col(); ++j){
      result[i][j] = (*this).mat[i][j] * num;
    }
  }
  return result;
}

template <size_t r_num, size_t c_num> 
const matrix<r_num, c_num> operator*(const int num, const matrix<r_num, c_num> &mat1)
{
  matrix<r_num, c_num> result;
  for(int i=0; i<r_num; ++i){
    for(int j=0; j<c_num; ++j){
      result[i][j] = num * mat1.mat[i][j];
    }
  }
  return result;
}


// / operator
template <size_t r_num, size_t c_num> 
const matrix<r_num, c_num> matrix<r_num, c_num>::operator/(const double num)
{
  matrix<r_num, c_num> result;
  for(int i=0; i<(*this).row(); ++i){
    for(int j=0; j<(*this).col(); ++j){
      result[i][j] = 1.0 * (*this).mat[i][j] / num;
    }
  }
  return result;
}

template <size_t r_num, size_t c_num> 
const matrix<r_num, c_num> operator/(const int num, const matrix<r_num, c_num> &mat1)
{
  matrix<r_num, c_num> result;
  for(int i=0; i<r_num; ++i){
    for(int j=0; j<c_num; ++j){
      result[i][j] = 1.0 * num / mat1.mat[i][j];
    }
  }
  return result;
}


// n行n列の要素に対して, 以降の行でpivot選択を実行する
// 絶対値が0よりも大きなpivotが無ければfalseを返す
template <size_t row, size_t col>
bool partial_pivot(matrix<row, col> &mat1, const int n)
{
  double v;

  // n列の絶対値が最大の行を探索
  double max_v = abs(mat1.mat[n][n]);
  int max_r = n;
  for(int i=n; i<row; ++i){
    if(abs(mat1.mat[i][n]) > max_v){
      max_v = mat1.mat[i][n];
      max_r = i;
    }
  }

  // 有効なpivotがない場合、その方程式は解けない(rank落ちの可能性が高い)
  if(max_v < EPS){
    return false;
  }

  // swap
  if(max_r != n){
    for(int i=0; i<col; ++i){
      v = mat1.mat[n][i];
      mat1.mat[n][i] = mat1.mat[max_r][i];
      mat1.mat[max_r][i] = v;
    }
  }

  return true;
}


// LU decomposition(with partial pivoting)
template <size_t size>
const matrix<size, size+1> lu_decomposition(const matrix<size, size> &mat1)
{
  matrix<size, size+1> result;

  // mat1をcopy
  for(int i=0; i<size; ++i){
    for(int j=0; j<size; ++j){
      result[i][j] = mat1.mat[i][j];
    }
  }

  // swapする前の元の行列番号を最終列に記録
  for(int i=0; i<size; ++i){
    result[i][size] = i;
  }

  // LU分解
  for(int i=0; i<size; ++i){
    // pivoting
    if (partial_pivot(result, i) == false){
      exit(-1);
    };

    double d = 1.0 / result.mat[i][i];
    for(int j=i+1; j<size; ++j){
      result.mat[j][i] = result.mat[j][i] * d;
      for(int k=i+1; k<size; ++k){
        result.mat[j][k] -= result.mat[j][i] * result.mat[i][k];
      }
    }
  }

  return result;
}


// solve LU*x=v using LU matrix and v vector
template <size_t size>
const std::array<double, size> lu_solve(const matrix<size, size+1> &mat1, const std::array<double, size> &v)
{
  // vectorを(pivot選択の結果に従って)並び替え
  std::array<double, size> ordered_v;
  int ordered_r;
  for(int i=0; i<size; ++i){
    ordered_r = mat1.mat[i][size];
    ordered_v[i] = v[ordered_r];
  }

  // 前進代入
  for(int i=0; i<size; ++i){
    double sub = 0;
    for(int j=0; j<i; ++j){
      sub += ordered_v[j] * mat1.mat[i][j];
    }
    ordered_v[i] = ordered_v[i] - sub;
  }

  // 後退代入
  for(int i=size-1; i>=0; --i){
    double sub = 0;
    for(int j=size-1; j>i; --j){
      sub += ordered_v[j] * mat1.mat[i][j];
    }
    ordered_v[i] = (ordered_v[i] - sub) / mat1.mat[i][i];
  }

  return ordered_v;
}


int main(){
  // 問題2
  /*
  std::array<double, 3> b = {10, 13, 6}, x;
  array_2d<3, 3> A_base={{{2, 3, -1}, {4, 4, -3}, {-2, 3, -1}}};
  matrix<3, 3> A(A_base);
  matrix<3, 4> LU;

  array_2d<2, 3> c_base={{{1, 2, 3}, {4, 5, 6}}};
  array_2d<3, 2> d_base={{{1, 2}, {3, 4}, {5, 6}}};
  matrix<2, 3> c(c_base);
  matrix<3, 2> d(d_base);
  print(c);
  print(d);
  print(c*d);
  */


  // オプション
  /*
  LU = lu_decomposition(A);
  print(LU);

  x = lu_solve(LU, b);
  print(x);
  */
  return 0;
}





