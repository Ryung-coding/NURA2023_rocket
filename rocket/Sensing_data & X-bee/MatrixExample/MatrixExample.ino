// 아두이노 라이브러리 관리에서 MatrixMath 라이브러리(by Charlie Matlack)를 다운받는다.

#include <MatrixMath.h>  

mtx_type A[2][2];  // 행렬 A를 생성한다. 원소의 자료형은 double이다.
mtx_type B[2][2];
mtx_type C[2][2];

void setup() {
  Serial.begin(9600);
  setMatrix(A);
  Matrix.Copy((mtx_type*)A, 2, 2, (mtx_type*)B);  // A행렬을 B행렬에 복사한다.
  
  Matrix.Print((mtx_type*)A,  2,  2, "A");  // A행렬을 시리얼 모니터에 출력한다.
  Matrix.Print((mtx_type*)B, 2, 2, "B"); 

  Serial.print("\nC is A + B");
  Matrix.Add((mtx_type*)A, (mtx_type*)B, 2, 2, (mtx_type*)C);  // A행렬과 B행렬을 더해 C행렬에 저장한다. 
  Matrix.Print((mtx_type*)C, 2, 2, "C");
  
  Serial.print("\nC is A - B");
  Matrix.Subtract((mtx_type*)A, (mtx_type*)B, 2, 2, (mtx_type*)C);  // A행렬에 B행렬을 빼 C행렬에 저장한다. 
  Matrix.Print((mtx_type*)C, 2, 2, "C");
  
  Serial.print("\nC is A x B");
  Matrix.Multiply((mtx_type*)A, (mtx_type*)B, 2, 2, 2, (mtx_type*)C);  // A행렬과 B행렬을 곱해 C행렬에 저장한다. 중간의 숫자는 각각 A의 행, A의 열(=B의 행), B의 열 개수이다.
  Matrix.Print((mtx_type*)C, 2, 2, "C");

  Serial.print("\nC is Transpose Matrix of A");
  Matrix.Transpose((mtx_type*)A, 2, 2, (mtx_type*)C);  // A행렬의 전치행렬을 C행렬에 저장한다. 
  Matrix.Print((mtx_type*)C, 2, 2, "C");

  Serial.print("\nB x 5");
  Matrix.Scale((mtx_type*)B, 2, 2, 5);  // B행렬에 스칼라배를 한다. 
  Matrix.Print((mtx_type*)B, 2, 2, "B");

  Serial.print("\nInvert Matrix of A");
  Matrix.Invert((mtx_type*)A, 2);  // A행렬의 역행렬을 A에 다시 저장한다. 역행렬은 정방행렬(행의 수  = 열의 수)만 가능하다.
  Matrix.Print((mtx_type*)A, 2, 2, "A");

  
}

void loop() {
  
}

void setMatrix(double Mat[2][2]){  // 2 X 2 행렬을 매개변수로 받아 성분들을 세팅한다.
  // 1행 성분
  Mat[0][0] = 1;
  Mat[0][1] = 2;
  
  // 2행 성분
  Mat[1][0] = 3;
  Mat[1][1] = 4;
}
