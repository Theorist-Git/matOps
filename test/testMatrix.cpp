/**
 * @file testMatrix.cpp
 * @brief Unit tests for the Matrix class using doctest.
 */

 #define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
 #include "doctest.h"
 #include "../src/matOps.hpp" // This brings in the Matrix class
 
 #include <vector>
 #include <stdexcept>
 #include <cmath>
 
 /**
  * @brief Tests for Matrix construction and shape reporting.
  */
 TEST_CASE("Matrix Construction and Shape") {
     /** 
      * @brief Valid construction with a 2x2 matrix.
      * Checks that the matrix correctly reports its dimensions.
      */
     SUBCASE("Valid construction with 2x2 matrix") {
         std::vector<std::vector<double>> vec = { {1, 2}, {3, 4} };
         Matrix m(vec);
         auto shape = m.shape();
         CHECK(shape.first == 2);
         CHECK(shape.second == 2);
     }
 
     /** 
      * @brief Invalid construction with an empty container.
      * Expects an invalid_argument exception.
      */
     SUBCASE("Invalid construction: empty container") {
         CHECK_THROWS_AS([](){ 
             Matrix m(std::vector<std::vector<double>>{}); 
         }(), std::invalid_argument);
     }
     
     /** 
      * @brief Invalid construction with inconsistent row sizes.
      * Expects an invalid_argument exception.
      */
     SUBCASE("Invalid construction: inconsistent row sizes") {
         CHECK_THROWS_AS([](){ 
             Matrix m({ {1, 2}, {3} }); 
         }(), std::invalid_argument);
     }
 }
 
 /**
  * @brief Tests for Matrix addition and subtraction operations.
  */
 TEST_CASE("Matrix Addition and Subtraction") {
     std::vector<std::vector<double>> a = { {1, 2}, {3, 4} };
     std::vector<std::vector<double>> b = { {5, 6}, {7, 8} };
     Matrix m1(a);
     Matrix m2(b);
 
     /**
      * @brief Matrix addition (matrix + matrix).
      * Verifies that element-wise addition produces the expected result.
      */
     SUBCASE("Matrix addition (matrix + matrix)") {
         Matrix sum = m1 + m2;
         std::vector<std::vector<double>> expected = { {6, 8}, {10, 12} };
         Matrix expectedMat(expected);
         CHECK(sum == expectedMat);
     }
 
     /**
      * @brief Matrix addition with scalar (matrix + k and k + matrix).
      * Verifies both left and right scalar addition.
      */
     SUBCASE("Matrix addition with scalar (matrix + k and k + matrix)") {
         Matrix sum1 = m1 + 5;
         std::vector<std::vector<double>> expected1 = { {6, 7}, {8, 9} };
         Matrix expectedMat1(expected1);
         CHECK(sum1 == expectedMat1);
 
         Matrix sum2 = 5 + m1;
         CHECK(sum2 == expectedMat1);
     }
 
     /**
      * @brief Matrix subtraction (matrix - matrix).
      * Checks that subtracting one matrix from another yields the correct result.
      */
     SUBCASE("Matrix subtraction (matrix - matrix)") {
         Matrix diff = m2 - m1;
         std::vector<std::vector<double>> expected = { {4, 4}, {4, 4} };
         Matrix expectedMat(expected);
         CHECK(diff == expectedMat);
     }
 
     /**
      * @brief Matrix subtraction with scalar (matrix - k and k - matrix).
      * Verifies subtraction when a scalar is involved.
      */
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
 
 /**
  * @brief Tests for Matrix multiplication and division.
  */
 TEST_CASE("Matrix Multiplication and Division") {
     std::vector<std::vector<double>> a = { {1, 2}, {3, 4} };
     std::vector<std::vector<double>> b = { {2, 0}, {1, 2} };
     Matrix m1(a);
     Matrix m2(b);
 
     /**
      * @brief Matrix multiplication (matrix * matrix).
      * Verifies that the product of two matrices is as expected.
      */
     SUBCASE("Matrix multiplication (matrix * matrix)") {
         Matrix prod = m1 * m2;
         std::vector<std::vector<double>> expected = { {4, 4}, {10, 8} };
         Matrix expectedMat(expected);
         CHECK(prod == expectedMat);
     }
 
     /**
      * @brief Scalar multiplication (matrix * k and k * matrix).
      * Checks that scalar multiplication scales each element appropriately.
      */
     SUBCASE("Scalar multiplication (matrix * k and k * matrix)") {
         Matrix prod1 = m1 * 3;
         std::vector<std::vector<double>> expected1 = { {3, 6}, {9, 12} };
         Matrix expectedMat1(expected1);
         CHECK(prod1 == expectedMat1);
 
         Matrix prod2 = 3 * m1;
         CHECK(prod2 == expectedMat1);
     }
 
     /**
      * @brief Matrix division by scalar.
      * Tests division by a scalar and expects an exception when dividing by zero.
      */
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
 
 /**
  * @brief Tests for element access using operator().
  */
 TEST_CASE("Element Access Operator()") {
     std::vector<std::vector<double>> a = { {10, 20}, {30, 40} };
     Matrix m(a);
     /** @brief Verify correct element access. */
     CHECK(m(0, 0) == 10);
     CHECK(m(0, 1) == 20);
     CHECK(m(1, 0) == 30);
     CHECK(m(1, 1) == 40);
 
     /**
      * @brief Modification via operator().
      * Tests that modifying an element via the accessor works correctly.
      */
     SUBCASE("Modification via operator()") {
         m(0, 0) = 99;
         CHECK(m(0, 0) == 99);
     }
 
     /**
      * @brief Out-of-range access.
      * Checks that accessing an element outside the matrix bounds throws an exception.
      */
     SUBCASE("Out-of-range access") {
         CHECK_THROWS_AS(m(2, 0), std::out_of_range);
         CHECK_THROWS_AS(m(0, 2), std::out_of_range);
     }
 }
 
 /**
  * @brief Tests for Matrix transposition.
  */
 TEST_CASE("Matrix Transpose") {
     std::vector<std::vector<double>> a = { {1, 2, 3}, {4, 5, 6} };
     Matrix m(a);
     Matrix t = m.transpose();
     std::vector<std::vector<double>> expected = { {1, 4}, {2, 5}, {3, 6} };
     Matrix expectedMat(expected);
     CHECK(t == expectedMat);
 }
 
 /**
  * @brief Tests for Matrix determinant computation.
  */
 TEST_CASE("Matrix Determinant") {
     /**
      * @brief Determinant for a 1x1 matrix.
      */
     SUBCASE("1x1 Matrix") {
         std::vector<std::vector<double>> a = { {5} };
         Matrix m(a);
         CHECK(m.determinant() == 5);
     }
 
     /**
      * @brief Determinant for a 2x2 matrix.
      */
     SUBCASE("2x2 Matrix") {
         std::vector<std::vector<double>> a = { {1, 2}, {3, 4} };
         Matrix m(a);
         CHECK(m.determinant() == doctest::Approx(-2));
     }
 
     /**
      * @brief Determinant for a 3x3 matrix.
      */
     SUBCASE("3x3 Matrix") {
         std::vector<std::vector<double>> a = { {6, 1, 1},
                                                  {4, -2, 5},
                                                  {2, 8, 7} };
         Matrix m(a);
         CHECK(m.determinant() == doctest::Approx(-306));
     }
 
     /**
      * @brief Non-square matrix determinant.
      * Expects an exception because the determinant is only defined for square matrices.
      */
     SUBCASE("Non-square Matrix should throw") {
         std::vector<std::vector<double>> a = { {1, 2, 3}, {4, 5, 6} };
         Matrix m(a);
         CHECK_THROWS_AS(m.determinant(), std::invalid_argument);
     }
 }
 
 /**
  * @brief Tests for Matrix inversion.
  */
 TEST_CASE("Matrix Inverse") {
     /**
      * @brief Inversion of a 2x2 invertible matrix.
      * Checks that the computed inverse matches the expected result.
      */
     SUBCASE("2x2 Invertible Matrix") {
         std::vector<std::vector<double>> a = { {4, 7}, {2, 6} };
         Matrix m(a);
         Matrix inv = m.inverse();
         // Expected inverse: 1/(4*6-7*2)=1/10, so [[6,-7],[-2,4]]
         std::vector<std::vector<double>> expected = { {0.6, -0.7}, {-0.2, 0.4} };
         Matrix expectedInv(expected);
         CHECK(inv == expectedInv);
     }
 
     /**
      * @brief Inversion of a singular matrix.
      * Verifies that attempting to invert a non-invertible matrix throws an exception.
      */
     SUBCASE("Singular Matrix Inverse should throw") {
         std::vector<std::vector<double>> a = { {1, 2}, {2, 4} };
         Matrix m(a);
         CHECK_THROWS_AS(m.inverse(), std::runtime_error);
     }
 }
 
 /**
  * @brief Tests for Matrix row and column insertion methods.
  */
 TEST_CASE("Matrix Row and Column Insertion") {
     std::vector<std::vector<double>> a = { {1, 2}, {3, 4} };
     Matrix m(a);
 
     /**
      * @brief Insert row with a provided vector.
      */
     SUBCASE("Insert row with vector") {
         std::vector<double> newRow = {5, 6};
         Matrix m2 = m.insertRow(newRow, 1);
         std::vector<std::vector<double>> expected = { {1, 2}, {5, 6}, {3, 4} };
         Matrix expectedMat(expected);
         CHECK(m2 == expectedMat);
     }
 
     /**
      * @brief Insert row filled with a scalar value.
      */
     SUBCASE("Insert row with scalar") {
         Matrix m2 = m.insertRow(9, 0);
         std::vector<std::vector<double>> expected = { {9, 9}, {1, 2}, {3, 4} };
         Matrix expectedMat(expected);
         CHECK(m2 == expectedMat);
     }
 
     /**
      * @brief Insert column with a provided vector.
      */
     SUBCASE("Insert column with vector") {
         std::vector<double> newCol = {7, 8};
         Matrix m2 = m.insertCol(newCol, 1);
         std::vector<std::vector<double>> expected = { {1, 7, 2}, {3, 8, 4} };
         Matrix expectedMat(expected);
         CHECK(m2 == expectedMat);
     }
 
     /**
      * @brief Insert column filled with a scalar value.
      */
     SUBCASE("Insert column with scalar") {
         Matrix m2 = m.insertCol(0, 2);
         std::vector<std::vector<double>> expected = { {1, 2, 0}, {3, 4, 0} };
         Matrix expectedMat(expected);
         CHECK(m2 == expectedMat);
     }
 }
 
 /**
  * @brief Tests for Matrix horizontal and vertical stacking.
  */
 TEST_CASE("Matrix Horizontal and Vertical Stacking") {
     std::vector<std::vector<double>> a = { {1, 2}, {3, 4} };
     std::vector<std::vector<double>> b = { {5, 6}, {7, 8} };
     Matrix m1(a);
     Matrix m2(b);
 
     /**
      * @brief Horizontal stacking.
      * Verifies that concatenating matrices side-by-side produces the expected result.
      */
     SUBCASE("Horizontal stacking") {
         Matrix h = m1.hStack(m2);
         std::vector<std::vector<double>> expected = { {1, 2, 5, 6}, {3, 4, 7, 8} };
         Matrix expectedMat(expected);
         CHECK(h == expectedMat);
     }
 
     /**
      * @brief Vertical stacking.
      * Verifies that concatenating matrices top-to-bottom produces the expected result.
      */
     SUBCASE("Vertical stacking") {
         Matrix v = m1.vStack(m2);
         std::vector<std::vector<double>> expected = { {1, 2}, {3, 4}, {5, 6}, {7, 8} };
         Matrix expectedMat(expected);
         CHECK(v == expectedMat);
     }
 
     /**
      * @brief Stacking with mismatched dimensions.
      * Ensures that attempting to stack matrices with incompatible dimensions throws an exception.
      */
     SUBCASE("Stacking with mismatched dimensions should throw") {
         std::vector<std::vector<double>> c = { {1, 2, 3} };
         Matrix m3(c);
         CHECK_THROWS_AS(m1.hStack(m3), std::invalid_argument);
         CHECK_THROWS_AS(m1.vStack(m3), std::invalid_argument);
     }
 }
 
 /**
  * @brief Tests for Matrix submatrix extraction functionality.
  */
 TEST_CASE("Matrix Submatrix Extraction") {
     std::vector<std::vector<double>> a = { {1, 2, 3},
                                              {4, 5, 6},
                                              {7, 8, 9} };
     Matrix m(a);
 
     /**
      * @brief Valid submatrix extraction.
      * Extracts a submatrix (rows 0-1, columns 1-2) and compares with the expected result.
      */
     SUBCASE("Valid submatrix extraction") {
         Matrix sub = m.extractMatrix({0, 1}, {1, 2});
         std::vector<std::vector<double>> expected = { {2, 3}, {5, 6} };
         Matrix expectedMat(expected);
         CHECK(sub == expectedMat);
     }
 
     /**
      * @brief Invalid extraction due to row indices out of bounds.
      */
     SUBCASE("Invalid extraction: row indices out of bounds") {
         CHECK_THROWS_AS(m.extractMatrix({0, 3}, {0, 1}), std::out_of_range);
     }
 
     /**
      * @brief Invalid extraction when start index is greater than end index (rows).
      */
     SUBCASE("Invalid extraction: start index greater than end index (rows)") {
         CHECK_THROWS_AS(m.extractMatrix({2, 1}, {0, 1}), std::out_of_range);
     }
 
     /**
      * @brief Invalid extraction when start index is greater than end index (columns).
      */
     SUBCASE("Invalid extraction: start index greater than end index (cols)") {
         CHECK_THROWS_AS(m.extractMatrix({0, 1}, {2, 1}), std::out_of_range);
     }
 }
 
 /**
  * @brief Tests for equality comparisons with tolerance.
  */
 TEST_CASE("Equality Tolerance Test") {
     std::vector<std::vector<double>> a = { {1.0000000001, 2}, {3, 4} };
     std::vector<std::vector<double>> b = { {1, 2}, {3, 4} };
     Matrix m1(a);
     Matrix m2(b);
     // The difference is within EPS (1e-9) so they should be equal.
     CHECK(m1 == m2);
 }
 
 /**
  * @brief Tests for copy construction and assignment operator.
  */
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
 
 /**
  * @brief Tests for 3x3 matrix inversion and verifying the identity.
  */
 TEST_CASE("3x3 Matrix Inversion and Identity Check") {
     std::vector<std::vector<double>> a = { {4, 7, 2},
                                              {3, 6, 1},
                                              {2, 5, 1} };
     Matrix m(a);
     Matrix inv = m.inverse();
     Matrix prod = m * inv;
 
     // Construct a 3x3 identity matrix.
     std::vector<std::vector<double>> identity = { {1, 0, 0},
                                                   {0, 1, 0},
                                                   {0, 0, 1} };
     Matrix id(identity);
 
     // Allow for some numerical tolerance in the product.
     CHECK(prod == id);
 }
 
 /**
  * @brief Tests for submatrix extraction of a single row and a single column.
  */
 TEST_CASE("Single Row and Single Column Submatrix Extraction") {
     std::vector<std::vector<double>> a = { {10, 20, 30},
                                              {40, 50, 60},
                                              {70, 80, 90} };
     Matrix m(a);
     
     /**
      * @brief Extract the second row as a 1x3 matrix.
      */
     Matrix rowExtract = m.extractMatrix({1, 1}, {0, 2});
     std::vector<std::vector<double>> expectedRow = { {40, 50, 60} };
     Matrix expectedRowMat(expectedRow);
     CHECK(rowExtract == expectedRowMat);
     
     /**
      * @brief Extract the third column as a 3x1 matrix.
      */
     Matrix colExtract = m.extractMatrix({0, 2}, {2, 2});
     std::vector<std::vector<double>> expectedCol = { {30}, {60}, {90} };
     Matrix expectedColMat(expectedCol);
     CHECK(colExtract == expectedColMat);
 }
 
 /**
  * @brief Tests for chained mixed arithmetic operations.
  */
 TEST_CASE("Chained Mixed Operations") {
     std::vector<std::vector<double>> a = { {2, 4}, {6, 8} };
     std::vector<std::vector<double>> b = { {1, 1}, {1, 1} };
     Matrix m1(a), m2(b);
     
     // Example chain: ((m1 + m2) * 2) - 3 and then add 5.
     Matrix result = (((m1 + m2) * 2) - 3) + 5;
     
     // Expected result calculated manually:
     // m1 + m2 = { {3, 5}, {7, 9} }
     // *2        = { {6, 10}, {14, 18} }
     // -3        = { {3, 7}, {11, 15} }
     // +5        = { 
 }

 TEST_CASE("Row vector: sum and mean") {
    // A row vector (1 x K matrix)
    Matrix rowVec({ {1, 2, 3, 4} });
    
    // Expected sum: 1 + 2 + 3 + 4 = 10
    CHECK(rowVec.sum() == 10);
    
    // Expected mean: 10 / 4 = 2.5
    CHECK(rowVec.mean() == doctest::Approx(2.5));
}

TEST_CASE("Column vector: sum and mean") {
    // A column vector (K x 1 matrix)
    Matrix colVec({ {1}, {2}, {3}, {4} });
    
    // Expected sum: 1 + 2 + 3 + 4 = 10
    CHECK(colVec.sum() == 10);
    
    // Expected mean: 10 / 4 = 2.5
    CHECK(colVec.mean() == doctest::Approx(2.5));
}

TEST_CASE("Non-vector matrix: sum throws invalid_argument") {
    // A matrix that is neither a single row nor a single column (e.g. 2x2)
    Matrix nonVector({ {1, 2}, {3, 4} });
    
    // Expect the sum() function to throw an invalid_argument exception.
    CHECK_THROWS_AS(nonVector.sum(), std::invalid_argument);
}

TEST_CASE("Matrix exponentiation with scalar 1 returns the same matrix") {
    Matrix m({ {1.0, 2.0},
               {3.0, 4.0} });
    Matrix result = m ^ 1.0;
    // The matrix should be unchanged
    CHECK(result == m);
}

TEST_CASE("Matrix exponentiation with scalar 2 returns element-wise square") {
    Matrix m({ {1.0, 2.0},
               {3.0, 4.0} });
    Matrix result = m ^ 2.0;
    Matrix expected({ {std::pow(1.0, 2.0), std::pow(2.0, 2.0)},
                      {std::pow(3.0, 2.0), std::pow(4.0, 2.0)} });
    CHECK(result == expected);
}

TEST_CASE("Matrix exponentiation with scalar 0 returns element-wise 1 (nonzero elements)") {
    Matrix m({ {1.0, 2.0},
               {3.0, 4.0} });
    Matrix result = m ^ 0.0;
    // For any nonzero x, std::pow(x, 0) is 1.
    Matrix expected({ {1.0, 1.0},
                      {1.0, 1.0} });
    CHECK(result == expected);
}

TEST_CASE("Matrix exponentiation when an element is 0 and exponent is <= 0") {
    // For elements equal to 0 and a non-positive exponent, our operator prints a warning
    // and returns 0. We test that behavior here.
    Matrix m({ {0.0, 2.0},
               {3.0, 0.0} });
    Matrix result = m ^ (-1.0);
    
    // For nonzero numbers, std::pow(x, -1) is the reciprocal.
    Matrix expected({ {0.0, std::pow(2.0, -1.0)},
                      {std::pow(3.0, -1.0), 0.0} });
    // Use Approx for floating point comparisons
    CHECK(result(0,0) == doctest::Approx(expected(0,0)));
    CHECK(result(0,1) == doctest::Approx(expected(0,1)));
    CHECK(result(1,0) == doctest::Approx(expected(1,0)));
    CHECK(result(1,1) == doctest::Approx(expected(1,1)));
}