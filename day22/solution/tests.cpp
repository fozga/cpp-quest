#include "exercise.hpp"
#include <cassert>
#include <iostream>

int main() {
    std::cout << "[day22] Running C++20 Concepts tests...\n";

    Matrix<int> A(2, 2, 1);
    Matrix<int> B(2, 2, 2);
    Matrix<int> C = A + B;
    assert(C(0, 0) == 3);
    assert(C(0, 1) == 3);
    assert(C(1, 0) == 3);
    assert(C(1, 1) == 3);
    std::cout << "  [pass] int matrix addition\n";

    Matrix<double> D(3, 3, 1.5);
    assert(D(0, 0) == 1.5);
    assert(D(2, 2) == 1.5);
    std::cout << "  [pass] double matrix construction\n";

    Matrix<double> E(3, 3, 0.5);
    Matrix<double> F = D + E;
    assert(F(1, 1) == 2.0);
    std::cout << "  [pass] double matrix addition\n";

    Matrix<int> G(1, 1, 7);
    assert(G.rows() == 1);
    assert(G.cols() == 1);
    assert(G(0, 0) == 7);
    std::cout << "  [pass] 1x1 matrix\n";

    bool caught = false;
    try {
        Matrix<int> X(2, 3, 0);
        Matrix<int> Y(3, 2, 0);
        auto Z = X + Y;
        (void)Z;
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);
    std::cout << "  [pass] dimension mismatch throws\n";

    // Matrix<std::string> would fail the Numeric concept check at compile time.

    std::cout << "[day22] All tests passed.\n";
    return 0;
}
