#include <cmath>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
#include <iostream>


float length(Eigen::Vector3f p) {
    return sqrt(p[0] * p[0] + p[1] * p[1]);
}
int main() {

    // Basic Example of cpp
    std::cout << "Example of cpp \n";
    float a = 1.0, b = 2.0;
    std::cout << a << std::endl; // 1
    std::cout << a / b << std::endl; // 1/2=0.5
    std::cout << std::sqrt(b) << std::endl; // sqrt(2)=1.414
    std::cout << std::acos(-1) << std::endl; // =pi=3.14159
    std::cout << std::sin(30.0 / 180.0 * acos(-1)) << std::endl; // sin(30')=0.5

    // Example of vector
    std::cout << "Example of vector \n";
    // vector definition
    Eigen::Vector3f v(1.0f, 2.0f, 3.0f);
    Eigen::Vector3f w(1.0f, 0.0f, 0.0f);
    // vector output
    std::cout << "Example of output \n";
    std::cout << v << std::endl; // 1,2,3
    // vector add
    std::cout << "Example of add \n";
    std::cout << v + w << std::endl; // (1,2,3)+(1,0,0)=(2,2,3)
    // vector scalar multiply
    std::cout << "Example of scalar multiply \n";
    std::cout << v * 3.0f << std::endl; // (1,2,3)*3=(3,6,9)
    std::cout << 2.0f * v << std::endl; // 2*(1,2,3)=(2,4,6)

    // Example of matrix
    std::cout << "Example of matrix \n";
    // matrix definition
    Eigen::Matrix3f i, j;
    i << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0;
    j << 2.0, 3.0, 1.0, 4.0, 6.0, 5.0, 9.0, 7.0, 8.0;
    // matrix output
    std::cout << "Example of output \n";
    std::cout << i << std::endl;
    std::cout << j << std::endl;
    // matrix add i + j
    Eigen::Matrix3f k = i + j;
    std::cout << "Example of matrix add \n";
    std::cout << k << std::endl;
    /** 3, 5, 4
     *  8, 11, 11
     *  16 15 17 */

    // matrix scalar multiply i * 2.0
    std::cout << "Example of matrix scalar multiply\n";
    std::cout << i * 2.0 << std::endl;
    /** 2, 4, 6
     *  8, 10, 12
     *  14,16,18 */

    // matrix multiply i * j
    std::cout << "Example of matrix multiply\n";
    std::cout << i * j << std::endl;
    /** 37 ... */

    // matrix multiply vector i * v
    std::cout << "Example of matrix multiply vector\n";
    std::cout << i * v << std::endl;
    /** 14 32 50 */

    std::cout << "Homework\n";
    /** homogeneous coordinates
     * extra 1 for point
     * extra 0 for vector
     *
     */
    Eigen::Vector3f p(2.0, 1.0, 1.0);
    float pi = acos(-1);
    float theta = 45./180*pi;
    float cos_theta = cos(theta), sin_theta = sin(theta);
    Eigen::Matrix3f rot;
    rot << cos_theta, -sin_theta, 0,
        sin_theta, cos_theta, 0,
        0, 0, 1;
    Eigen::Vector3f move(1, 2, 0);
    std::cout << p << std::endl;
    std::cout << rot << std::endl;
    std::cout << move << std::endl;
    std::cout << "rotate\n";
    std::cout << rot * p << std::endl;
    std::cout << "length" << length(p) << " " << length(rot * p) << std::endl;
    std::cout << "rotate + move\n";
    std::cout << rot * p + move << std::endl;

    return 0;
}
