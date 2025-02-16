// testMatrix.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "matOps.hpp" // This brings in the Matrix class

#include <vector>
#include <stdexcept>
#include <cmath>

TEST_CASE("Matrix Construction and Shape") {
    SUBCASE("Valid construction with 2x2 matrix") {
        std::vector<std::vector<double>> vec = { {1, 2}, {3, 4} };
        Matrix m(vec);
        auto shape = m.shape();
        CHECK(shape.first == 2);
        CHECK(shape.second == 2);
    }

    SUBCASE("Invalid construction: empty container") {
        CHECK_THROWS_AS([](){ 
            Matrix m(std::vector<std::vector<double>>{}); 
        }(), std::invalid_argument);
    }
    
    SUBCASE("Invalid construction: inconsistent row sizes") {
        CHECK_THROWS_AS([](){ 
            Matrix m({ {1, 2}, {3} }); 
        }(), std::invalid_argument);
    }
}

TEST_CASE("Matrix Addition and Subtraction") {
    std::vector<std::vector<double>> a = { {1, 2}, {3, 4} };
    std::vector<std::vector<double>> b = { {5, 6}, {7, 8} };
    Matrix m1(a);
    Matrix m2(b);

    SUBCASE("Matrix addition (matrix + matrix)") {
        Matrix sum = m1 + m2;
        std::vector<std::vector<double>> expected = { {6, 8}, {10, 12} };
        Matrix expectedMat(expected);
        CHECK(sum == expectedMat);
    }

    SUBCASE("Matrix addition with scalar (matrix + k and k + matrix)") {
        Matrix sum1 = m1 + 5;
        std::vector<std::vector<double>> expected1 = { {6, 7}, {8, 9} };
        Matrix expectedMat1(expected1);
        CHECK(sum1 == expectedMat1);

        Matrix sum2 = 5 + m1;
        CHECK(sum2 == expectedMat1);
    }

    SUBCASE("Matrix subtraction (matrix - matrix)") {
        Matrix diff = m2 - m1;
        std::vector<std::vector<double>> expected = { {4, 4}, {4, 4} };
        Matrix expectedMat(expected);
        CHECK(diff == expectedMat);
    }

    SUBCASE("Matrix subtraction with scalar (matrix - k and k - matrix)") {
        Matrix diff1 = m2 - 2;
        std::vector<std::vector<double>> expected1 = { {3, 4}, {5, 6} };
        Matrix expectedMat1(expected1);
        CHECK(diff1 == expectedMat1);

        Matrix diff2 = 10 - m2;
        std::vector<std::vector<double>> expected2 = { {5, 4}, {3, 2} };
        Matrix expectedMat2(expected2);
        CHECK(diff2 == expectedMat2);
    }
}

TEST_CASE("Matrix Multiplication and Division") {
    std::vector<std::vector<double>> a = { {1, 2}, {3, 4} };
    std::vector<std::vector<double>> b = { {2, 0}, {1, 2} };
    Matrix m1(a);
    Matrix m2(b);

    SUBCASE("Matrix multiplication (matrix * matrix)") {
        Matrix prod = m1 * m2;
        std::vector<std::vector<double>> expected = { {4, 4}, {10, 8} };
        Matrix expectedMat(expected);
        CHECK(prod == expectedMat);
    }

    SUBCASE("Scalar multiplication (matrix * k and k * matrix)") {
        Matrix prod1 = m1 * 3;
        std::vector<std::vector<double>> expected1 = { {3, 6}, {9, 12} };
        Matrix expectedMat1(expected1);
        CHECK(prod1 == expectedMat1);

        Matrix prod2 = 3 * m1;
        CHECK(prod2 == expectedMat1);
    }

    SUBCASE("Matrix division by scalar") {
        std::vector<std::vector<double>> c = { {2, 4}, {6, 8} };
        Matrix m(c);
        Matrix result = m / 2;
        std::vector<std::vector<double>> expected = { {1, 2}, {3, 4} };
        Matrix expectedMat(expected);
        CHECK(result == expectedMat);

        // Division by zero should throw a runtime_error
        CHECK_THROWS_AS(m / 0, std::runtime_error);
    }
}

TEST_CASE("Element Access Operator()") {
    std::vector<std::vector<double>> a = { {10, 20}, {30, 40} };
    Matrix m(a);
    CHECK(m(0, 0) == 10);
    CHECK(m(0, 1) == 20);
    CHECK(m(1, 0) == 30);
    CHECK(m(1, 1) == 40);

    SUBCASE("Modification via operator()") {
        m(0, 0) = 99;
        CHECK(m(0, 0) == 99);
    }

    SUBCASE("Out-of-range access") {
        CHECK_THROWS_AS(m(2, 0), std::out_of_range);
        CHECK_THROWS_AS(m(0, 2), std::out_of_range);
    }
}

TEST_CASE("Matrix Transpose") {
    std::vector<std::vector<double>> a = { {1, 2, 3}, {4, 5, 6} };
    Matrix m(a);
    Matrix t = m.transpose();
    std::vector<std::vector<double>> expected = { {1, 4}, {2, 5}, {3, 6} };
    Matrix expectedMat(expected);
    CHECK(t == expectedMat);
}

TEST_CASE("Matrix Determinant") {
    SUBCASE("1x1 Matrix") {
        std::vector<std::vector<double>> a = { {5} };
        Matrix m(a);
        CHECK(m.determinant() == 5);
    }

    SUBCASE("2x2 Matrix") {
        std::vector<std::vector<double>> a = { {1, 2}, {3, 4} };
        Matrix m(a);
        CHECK(m.determinant() == doctest::Approx(-2));
    }

    SUBCASE("3x3 Matrix") {
        std::vector<std::vector<double>> a = { {6, 1, 1},
                                                 {4, -2, 5},
                                                 {2, 8, 7} };
        Matrix m(a);
        CHECK(m.determinant() == doctest::Approx(-306));
    }

    SUBCASE("Non-square Matrix should throw") {
        std::vector<std::vector<double>> a = { {1, 2, 3}, {4, 5, 6} };
        Matrix m(a);
        CHECK_THROWS_AS(m.determinant(), std::invalid_argument);
    }
}

TEST_CASE("Matrix Inverse") {
    SUBCASE("2x2 Invertible Matrix") {
        std::vector<std::vector<double>> a = { {4, 7}, {2, 6} };
        Matrix m(a);
        Matrix inv = m.inverse();
        // Expected inverse: 1/(4*6-7*2)=1/10, so [[6,-7],[-2,4]]
        std::vector<std::vector<double>> expected = { {0.6, -0.7}, {-0.2, 0.4} };
        Matrix expectedInv(expected);
        CHECK(inv == expectedInv);
    }

    SUBCASE("Singular Matrix Inverse should throw") {
        std::vector<std::vector<double>> a = { {1, 2}, {2, 4} };
        Matrix m(a);
        CHECK_THROWS_AS(m.inverse(), std::runtime_error);
    }
}

TEST_CASE("Matrix Row and Column Insertion") {
    std::vector<std::vector<double>> a = { {1, 2}, {3, 4} };
    Matrix m(a);

    SUBCASE("Insert row with vector") {
        std::vector<double> newRow = {5, 6};
        Matrix m2 = m.insertRow(newRow, 1);
        std::vector<std::vector<double>> expected = { {1, 2}, {5, 6}, {3, 4} };
        Matrix expectedMat(expected);
        CHECK(m2 == expectedMat);
    }

    SUBCASE("Insert row with scalar") {
        Matrix m2 = m.insertRow(9, 0);
        std::vector<std::vector<double>> expected = { {9, 9}, {1, 2}, {3, 4} };
        Matrix expectedMat(expected);
        CHECK(m2 == expectedMat);
    }

    SUBCASE("Insert column with vector") {
        std::vector<double> newCol = {7, 8};
        Matrix m2 = m.insertCol(newCol, 1);
        std::vector<std::vector<double>> expected = { {1, 7, 2}, {3, 8, 4} };
        Matrix expectedMat(expected);
        CHECK(m2 == expectedMat);
    }

    SUBCASE("Insert column with scalar") {
        Matrix m2 = m.insertCol(0, 2);
        std::vector<std::vector<double>> expected = { {1, 2, 0}, {3, 4, 0} };
        Matrix expectedMat(expected);
        CHECK(m2 == expectedMat);
    }
}

TEST_CASE("Matrix Horizontal and Vertical Stacking") {
    std::vector<std::vector<double>> a = { {1, 2}, {3, 4} };
    std::vector<std::vector<double>> b = { {5, 6}, {7, 8} };
    Matrix m1(a);
    Matrix m2(b);

    SUBCASE("Horizontal stacking") {
        Matrix h = m1.hStack(m2);
        std::vector<std::vector<double>> expected = { {1, 2, 5, 6}, {3, 4, 7, 8} };
        Matrix expectedMat(expected);
        CHECK(h == expectedMat);
    }

    SUBCASE("Vertical stacking") {
        Matrix v = m1.vStack(m2);
        std::vector<std::vector<double>> expected = { {1, 2}, {3, 4}, {5, 6}, {7, 8} };
        Matrix expectedMat(expected);
        CHECK(v == expectedMat);
    }

    SUBCASE("Stacking with mismatched dimensions should throw") {
        std::vector<std::vector<double>> c = { {1, 2, 3} };
        Matrix m3(c);
        CHECK_THROWS_AS(m1.hStack(m3), std::invalid_argument);
        CHECK_THROWS_AS(m1.vStack(m3), std::invalid_argument);
    }
}

TEST_CASE("Matrix Submatrix Extraction") {
    std::vector<std::vector<double>> a = { {1, 2, 3},
                                             {4, 5, 6},
                                             {7, 8, 9} };
    Matrix m(a);

    SUBCASE("Valid submatrix extraction") {
        // Extract rows 0 to 1 and columns 1 to 2 (inclusive)
        Matrix sub = m.extractMatrix({0, 1}, {1, 2});
        std::vector<std::vector<double>> expected = { {2, 3}, {5, 6} };
        Matrix expectedMat(expected);
        CHECK(sub == expectedMat);
    }

    SUBCASE("Invalid extraction: row indices out of bounds") {
        CHECK_THROWS_AS(m.extractMatrix({0, 3}, {0, 1}), std::out_of_range);
    }

    SUBCASE("Invalid extraction: start index greater than end index (rows)") {
        CHECK_THROWS_AS(m.extractMatrix({2, 1}, {0, 1}), std::out_of_range);
    }

    SUBCASE("Invalid extraction: start index greater than end index (cols)") {
        CHECK_THROWS_AS(m.extractMatrix({0, 1}, {2, 1}), std::out_of_range);
    }
}

TEST_CASE("Equality Tolerance Test") {
    std::vector<std::vector<double>> a = { {1.0000000001, 2}, {3, 4} };
    std::vector<std::vector<double>> b = { {1, 2}, {3, 4} };
    Matrix m1(a);
    Matrix m2(b);
    // The difference is within EPS (1e-9) so they should be equal.
    CHECK(m1 == m2);
}

TEST_CASE("Copy Constructor and Assignment Operator") {
    std::vector<std::vector<double>> a = { {1, 2}, {3, 4} };
    Matrix original(a);
    Matrix copy1 = original;   // Uses copy constructor
    Matrix copy2(a);           // Another copy
    original(0, 0) = 99;       // Modify original

    // The copies should remain unchanged.
    CHECK(copy1(0, 0) == 1);
    CHECK(copy2(0, 0) == 1);

    // Test self-assignment (should not alter the matrix)
    original = original;
    CHECK(original(0, 0) == 99);
}

TEST_CASE("3x3 Matrix Inversion and Identity Check") {
    std::vector<std::vector<double>> a = { {4, 7, 2},
                                             {3, 6, 1},
                                             {2, 5, 1} };
    Matrix m(a);
    Matrix inv = m.inverse();
    Matrix prod = m * inv;

    // Construct a 3x3 identity matrix
    std::vector<std::vector<double>> identity = { {1, 0, 0},
                                                  {0, 1, 0},
                                                  {0, 0, 1} };
    Matrix id(identity);

    // Allow for some numerical tolerance in the product.
    CHECK(prod == id);
}

TEST_CASE("Single Row and Single Column Submatrix Extraction") {
    std::vector<std::vector<double>> a = { {10, 20, 30},
                                             {40, 50, 60},
                                             {70, 80, 90} };
    Matrix m(a);
    
    // Extract the second row as a 1x3 matrix
    Matrix rowExtract = m.extractMatrix({1, 1}, {0, 2});
    std::vector<std::vector<double>> expectedRow = { {40, 50, 60} };
    Matrix expectedRowMat(expectedRow);
    CHECK(rowExtract == expectedRowMat);
    
    // Extract the third column as a 3x1 matrix
    Matrix colExtract = m.extractMatrix({0, 2}, {2, 2});
    std::vector<std::vector<double>> expectedCol = { {30}, {60}, {90} };
    Matrix expectedColMat(expectedCol);
    CHECK(colExtract == expectedColMat);
}

TEST_CASE("Chained Mixed Operations") {
    std::vector<std::vector<double>> a = { {2, 4}, {6, 8} };
    std::vector<std::vector<double>> b = { {1, 1}, {1, 1} };
    Matrix m1(a), m2(b);
    
    // Example chain: ((m1 + m2) * 2) - 3 and then add 5
    Matrix result = (((m1 + m2) * 2) - 3) + 5;
    
    // Calculate expected result manually:
    // m1 + m2 = { {3, 5}, {7, 9} }
    // *2        = { {6, 10}, {14, 18} }
    // -3        = { {3, 7}, {11, 15} }
    // +5        = { {8, 12}, {16, 20} }
    std::vector<std::vector<double>> expected = { {8, 12}, {16, 20} };
    Matrix expectedMat(expected);
    
    CHECK(result == expectedMat);
}

TEST_CASE("Zero Matrix Determinant") {
    std::vector<std::vector<double>> zeroMatrix = { {0, 0}, {0, 0} };
    Matrix m(zeroMatrix);
    CHECK(m.determinant() == 0);
}

TEST_CASE("Division by Floating Point Scalar") {
    std::vector<std::vector<double>> a = { {2.5, 5.0}, {7.5, 10.0} };
    Matrix m(a);
    Matrix result = m / 2.5;
    std::vector<std::vector<double>> expected = { {1.0, 2.0}, {3.0, 4.0} };
    Matrix expectedMat(expected);
    CHECK(result == expectedMat);
}
