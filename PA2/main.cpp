#include <iostream>
#include "matrix.h"
#include "rotmat.h"
#include <math.h>
#include <cmath>
#include <sstream>
#include <string>

void setMatrixValues(Matrix * m, std::vector<double> v){
    for(int r=0; r<m->getRows(); r++){
        for(int c=0; c<m->getCols(); c++){
            m->at(r,c) = v[r*m->getCols() + c];
        }
    }
}

std::string mat2str(Matrix m){
     std::stringstream ss;
    for(int r=0; r<m.getRows(); r++){
        for(int c=0; c<m.getCols(); c++){
            ss << m(r,c);
            if(c<m.getCols()-1){
                ss << " ";
            }
        }
        ss << ";";
        if(r<m.getRows()-1){
            ss<<" ";
        }
    }
    return ss.str();
}

void test(Matrix test, std::string target, std::string msg){
    std::string mStr = mat2str(test);
    std::string equal = mStr==target ? "Success: " : "Failed:  ";
    std::cout << equal << msg << "\t " << mStr << " " << target << std::endl;
}

template <class T1>
void test(T1 test, T1 target, std::string msg){
    std::string equal = test==target ? "Success: " : "Failed:  ";
    std::cout << equal << msg << "\t " << test << " " << target << std::endl;
}

void testEps(double test, double target, double eps, std::string msg){
    std::string equal = std::pow(std::pow(target-test,2.0),0.5) < eps ? "Success: " : "Failed:  ";
    std::cout << equal << msg << "\t " << test << " " << target << std::endl;
}


int main(){
    //Construction and index access
    Matrix mat(3,4);
    test(mat,"0 0 0 0; 0 0 0 0; 0 0 0 0;", "simple constructor");
    Matrix mat2(3,4,3);
    test<double>(mat2.at(0,0),3,"at method");
    test<double>(mat2(2,3),3,"() operator");

    //Transpose
    setMatrixValues(&mat,std::vector<double>{0,1,2,3,4,5,6,7,8,9,10,11});
    Matrix mat3 = mat; //Make a copy of mat
    test(mat3,"0 1 2 3; 4 5 6 7; 8 9 10 11;", "creation of mat3");
    mat3.transpose();
    test(mat3,"0 4 8; 1 5 9; 2 6 10; 3 7 11;", "transpose()");

    //Addition
    Matrix mat4(4,3);
    setMatrixValues(&mat4,std::vector<double>{0,0,1,1,1,2,2,2,3,3,3,4});
    test(mat4,"0 0 1; 1 1 2; 2 2 3; 3 3 4;", "Creation of mat4");
    Matrix mat5 = mat3.add(mat4);
    test(mat5,"0 4 9; 2 6 11; 4 8 13; 6 10 15;", "add()");
    Matrix mat6 = mat4 + mat3;
    test(mat6,"0 4 9; 2 6 11; 4 8 13; 6 10 15;", "+ operator");

    //Inner product
    test(Matrix(1,3,2)*Matrix(3,1,2),"12;","inner product");

    //outer product
    test(Matrix(3,1,2)*Matrix(1,3,2),"4 4 4; 4 4 4; 4 4 4;","outer product");

    //Multiplication
    mat4.transpose();
    Matrix mat7 = mat4.multiply(mat6);
    Matrix mat8 = mat6*mat4;
    test(mat7,"28 52 82; 28 52 82; 40 80 130;","multiply() (matrix multiplication");
    test(mat8,"9 22 35 48; 11 30 49 68; 13 38 63 88; 15 46 77 108;","* operator (matrix multiplication)");
    Matrix mat9 = mat8 * 0.5;
    test(mat9,"4.5 11 17.5 24; 5.5 15 24.5 34; 6.5 19 31.5 44; 7.5 23 38.5 54;","* operator (scalar multiplication)");

    //Rotation matrices
    RotMat2D identity;
    test(identity,"1 0; 0 1;", "Identity");
    RotMat2D ninety(M_PI/2);
    test(ninety,"6.12323e-17 -1; 1 6.12323e-17;", "ninety");
    ninety.transpose();
    test(ninety,"6.12323e-17 1; -1 6.12323e-17;", "ninety trasposed");
    RotMat2D thirty(30*M_PI/180);
    test(thirty,"0.866025 -0.5; 0.5 0.866025;", "thirty");
    RotMat2D sixty(60*M_PI/180);
    test(sixty,"0.5 -0.866025; 0.866025 0.5;", "sixty");
    Matrix allThree = thirty * sixty;
    /* UPDATED test method */
//    test(allThree,"2.22045e-16 -1; 1 2.22045e-16;", "thirty*sixty");
    testEps(allThree(0,0),0,1e-9,"thirty*sixty");
    testEps(allThree(0,1),-1,1e-9,"thirty*sixty");
    testEps(allThree(1,0),1,1e-9,"thirty*sixty");
    testEps(allThree(1,1),0,1e-9,"thirty*sixty");
    allThree = allThree * ninety;
    /* UPDATED test method */
//    test(allThree,"1 1.60812e-16; -1.60812e-16 1;", "thirty*sixty* ninety (ninety is transposed)");
    testEps(allThree(0,0),1,1e-9,"thirty*sixty* ninety (ninety is transposed)");
    testEps(allThree(0,1),0,1e-9,"thirty*sixty* ninety (ninety is transposed)");
    testEps(allThree(1,0),0,1e-9,"thirty*sixty* ninety (ninety is transposed)");
    testEps(allThree(1,1),1,1e-9,"thirty*sixty* ninety (ninety is transposed)");

    //Rotate v by thirty degrees
    Matrix v(2,1,0);
    v(0,0) = 2;
    v(1,0) = 3;
    test(thirty*v,"0.232051; 3.59808;", "Rotation of vector using rotation matrix");

}