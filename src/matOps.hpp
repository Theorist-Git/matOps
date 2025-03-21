#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

#pragma once

#define EPS 1e-12
#define OPENMP_THRESHOLD 10000

/**
 * @class Matrix
 * @brief A simple linear algebra library for matrix operations.
 * @version 0.6.1
 * 
 * This class provides basic matrix operations such as addition, subtraction,
 * multiplication, transposition, determinant calculation, inversion, and row/column insertion.
 *
 * Example Usage:
 * @code
 * Matrix A({{1, 2}, {3, 4}});
 * Matrix B({{5, 6}, {7, 8}});
 * Matrix C = A + B; // Matrix addition
 * Matrix D = A * B; // Matrix multiplication
 * double detA = A.determinant(); // Determinant calculation
 * @endcode
 */
class Matrix {
    private:
        std::vector<std::vector<double>> container; ///< 2D vector holding matrix elements.
        size_t nrows; ///< Number of rows in the matrix.
        size_t ncols; ///< Number of columns in the matrix.

        struct InternalTag {};

        /**
         * @brief Internal constructor for creating a Matrix by moving a pre-validated container.
         *
         * This constructor is intended for internal use only. It bypasses the usual
         * validation performed in the public constructor, assuming that the provided
         * container is already well-formed (i.e., non-empty and with consistent row sizes).
         * The container is moved into the new Matrix object for efficient transfer of data.
         *
         * @param container An rvalue reference to a 2D vector of doubles representing the matrix elements.
         * @param tag A dummy parameter (of type InternalTag) used to differentiate this constructor from the public one.
         * @throws std::invalid_argument if the container is empty or if it contains no columns.
         *
         * @note This constructor does not perform full validation. It assumes that the input container is valid.
         */
        Matrix(std::vector<std::vector<double>>&& container, InternalTag)
            : container(std::move(container)), 
              nrows(this->container.size()),
              ncols(this->container.empty() ? 0 : this->container[0].size()) {

            if (nrows == 0 || ncols == 0) {
                throw std::invalid_argument("Matrix is empty. Expected a non-empty container.");
            }
            // No validation performed. Assumed well formed matrix.
        }

    public:
        /**
         * @brief Returns the dimensions of the matrix.
         *
         * @return A std::pair where first is the number of rows and second is the number of columns.
         */
        std::pair<size_t, size_t> shape() const { return {nrows, ncols}; }


        /**
         * @brief Constructs a Matrix from a given 2D vector container.
         *
         * @param container A 2D vector of doubles representing the matrix.
         * @throws std::invalid_argument if the container is empty or row sizes are inconsistent.
         * @note The shape is determined by the size of the container.
         */
        Matrix(const std::vector<std::vector<double>>& container) {
            this->nrows = container.size();

            if ( this->nrows == 0 ) {
                throw std::invalid_argument("Matrix is empty. Expected `const std::vector<std::vector<double>>& container`");
            } else {
                this->ncols = container[0].size();
            }

            for (const auto& row : container) {
                if (row.size() != ncols) {
                    throw std::invalid_argument("Inconsistent row sizes in matrix");
                }
            }

            this->container = container;
        }

        /**
         * @brief Constructs an Identity Matrix of specified dimensions.
         * @code
         * Matrix A = Matrix::identity(3);
         * A = [
         *      [1, 0, 0]
         *      [0, 1, 0]
         *      [0, 0, 1]
         * ]
         * @endcode
         * @param dim Dimensions of matrix (dim x dim).
         */
        static Matrix identity(size_t dim) {
            std::vector<std::vector<double>> I(dim, std::vector<double>(dim, 0.0));

            for (size_t i = 0; i < dim; ++i) {
                I[i][i] = 1.0;
            }

            return Matrix(std::move(I), InternalTag{});
        }

        /**
         * @brief Creates a matrix with constant values.
         *
         * Function constructs and returns a Matrix object with the specified
         * dimensions, initializing every element to the given constant value.
         * 
         * Example usage:
         * @code
         * // Create a 3x3 matrix filled with 5.0
         * Matrix A = Matrix::constValMatrix(3, 3, 5.0);
         *
         * // Expected Output:
         * //  5.0  5.0  5.0
         * //  5.0  5.0  5.0
         * //  5.0  5.0  5.0
         * @endcode
         *
         * @param rows The number of rows in the matrix.
         * @param cols The number of columns in the matrix.
         * @param val The constant value to initialize each element of the matrix.
         * @return A Matrix object of dimensions (rows x cols) where each element is set to @p val.
         * @throws std::invalid_argument if either @p rows or @p cols is zero.
         */
        static Matrix constValMatrix(size_t rows, size_t cols, double val) {
            if (rows == 0 || cols == 0) {
                throw std::invalid_argument("Matrix cannot have zero dimensions");
            }
            
            return Matrix(std::move(std::vector<std::vector<double>>(rows, std::vector<double>(cols, val))), InternalTag{});
        }

        /**
         * @brief Returns a copy of the matrix data.
         *
         * This function returns a new 2D vector containing the matrix elements.
         * Modifications to the returned vector do not affect the original matrix.
         *
         * @return A copy of the 2D vector representing the matrix.
         */
        std::vector<std::vector<double>> toVector() const {
            return this->container;
        }

        /**
         * @brief Adds two matrices element-wise.
         *
         * @param other The Matrix to add.
         * @return A new Matrix representing the element-wise sum.
         * @throws std::invalid_argument if the dimensions of the two matrices do not match.
         * @note The shape remains unchanged.
         */
        Matrix operator+(const Matrix& other) const {
            if (this->nrows != other.nrows || this->ncols != other.ncols) {
                throw std::invalid_argument(
                    "Matrix dimensions do not match: (" + 
                    std::to_string(this->nrows) + "x" + std::to_string(this->ncols) + 
                    ") vs (" + 
                    std::to_string(other.nrows) + "x" + std::to_string(other.ncols) + 
                    ")"
                );
            }

            Matrix addRes = *this; // addRes = A in A + B

            const size_t totalElements = this->ncols * this->nrows;

            #pragma omp parallel for if(totalElements > OPENMP_THRESHOLD) collapse(2)
            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {
                    addRes.container[i][j] += other.container[i][j];
                }
            }

            return addRes;
        }

        /**
         * @brief Adds a scalar value to each element of the matrix. (MATRIX + K)
         *
         * @param scalar A double value to add.
         * @return A new Matrix with the scalar added to each element.
         * @note The shape remains unchanged.
         */
        Matrix operator+(double scalar) const {
            Matrix addRes = *this;
            
            const size_t totalElements = this->ncols * this->nrows;

            #pragma omp parallel for if(totalElements > OPENMP_THRESHOLD) collapse(2)
            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {
                    addRes.container[i][j] += scalar;
                }
            }

            return addRes;
        }

        /**
         * @brief Adds a scalar to each element of a matrix. (K + MATRIX)
         *
         * @param scalar The scalar value.
         * @param other The Matrix to add the scalar to.
         * @return A new Matrix with the result.
         * @note The shape remains unchanged.
         */
        friend Matrix operator+(double scalar, const Matrix& other) {
            return other + scalar;
        }

        /**
         * @brief Subtracts one matrix from another element-wise.
         *
         * @param other The Matrix to subtract.
         * @return A new Matrix representing the element-wise difference.
         * @throws std::invalid_argument if the dimensions of the two matrices do not match.
         * @note The shape remains unchanged.
         */
        Matrix operator-(const Matrix& other) const {
            if (this->nrows != other.nrows || this->ncols != other.ncols) {
                throw std::invalid_argument(
                    "Matrix dimensions do not match: (" + 
                    std::to_string(this->nrows) + "x" + std::to_string(this->ncols) + 
                    ") vs (" + 
                    std::to_string(other.nrows) + "x" + std::to_string(other.ncols) + 
                    ")"
                );
            }

            Matrix subRes = *this; // subRes = A in A - B

            const size_t totalElements = this->ncols * this->nrows;

            #pragma omp parallel for if(totalElements > OPENMP_THRESHOLD) collapse(2)
            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {
                    subRes.container[i][j] -= other.container[i][j];
                }
            }

            return subRes;
        }

        /**
         * @brief Subtracts a scalar from each element of the matrix. (MATRIX - K)
         *
         * @param scalar The scalar value to subtract.
         * @return A new Matrix with each element reduced by the scalar.
         * @note The shape remains unchanged.
         */
        Matrix operator-(double scalar) const {
            Matrix subRes = *this; // subRes = A in A - B

            const size_t totalElements = this->ncols * this->nrows;

            #pragma omp parallel for if(totalElements > OPENMP_THRESHOLD) collapse(2)
            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {
                    subRes.container[i][j] -= scalar;
                }
            }

            return subRes;
        }
        
        /**
         * @brief Subtracts each element of the matrix from a scalar. (K - MATRIX)
         *
         * @param scalar The scalar value.
         * @param other The Matrix whose elements are subtracted from the scalar.
         * @return A new Matrix with the result.
         * @note The shape remains unchanged.
         */
        friend Matrix operator-(double scalar, const Matrix& other) {
            Matrix subRes = other;

            const size_t totalElements = other.ncols * other.nrows;

            #pragma omp parallel for if(totalElements > OPENMP_THRESHOLD) collapse(2)
            for (size_t i = 0; i < subRes.nrows; ++i) {
                for (size_t j = 0; j < subRes.ncols; ++j) {
                    subRes.container[i][j] = scalar - subRes.container[i][j];
                }
            }

            return subRes;
        }

        /**
         * @brief Compares two matrices for equality.
         *
         * @param other The Matrix to compare with.
         * @return True if the matrices are equal (within a tolerance), false otherwise.
         */
        bool operator==(const Matrix& other) const {
            if (this->nrows != other.nrows || this->ncols != other.ncols) {
                return false;
            }

            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {
                    if ( std::abs(this->container[i][j] - other.container[i][j]) > EPS) {
                        return false;
                    }
                }
            }

            return true;
        }

        /**
         * @brief Compares two matrices for inequality.
         *
         * This operator checks if two matrices are not equal by comparing their dimensions and
         * individual elements. Two matrices are considered not equal if:
         *  - Their dimensions differ, or
         *  - At least one pair of corresponding elements differs by more than EPS.
         *
         * @param other The matrix to compare with.
         * @return true if the matrices differ by at least one element more than EPS; false otherwise.
         */
        bool operator!=(const Matrix& other) const {
            return !(*this == other);
        }

        /**
         * @brief Multiplies two matrices.
         *
         * @param other The Matrix to multiply with.
         * @return A new Matrix resulting from matrix multiplication.
         * @throws std::invalid_argument if the number of columns of the first matrix
         *         does not match the number of rows of the second.
         * @note The shape of the result is (nrows of first, ncols of second).
         */
        Matrix operator*(const Matrix& other) const {
            if (this->ncols != other.nrows) {
                throw std::invalid_argument(
                    "Incorrect dimensions: For matrices (m x n) and (p x r), n must be equal to p. "
                    "Given: (" + std::to_string(this->nrows) + "x" + std::to_string(this->ncols) + 
                    ") and (" + std::to_string(other.nrows) + "x" + std::to_string(other.ncols) + ")."
                );
            }

            std::vector<std::vector<double>> mulResContainer(this->nrows, std::vector<double>(other.ncols, 0.0));

            const size_t totalElements = this->ncols * this->nrows;

            #pragma omp parallel for if(totalElements > OPENMP_THRESHOLD) collapse(2)
            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t k = 0; k < this->ncols; ++k) {
                    
                    double Aik = this->container[i][k]; // Cache the A[i][k] element.

                    for (size_t j = 0; j < other.ncols; ++j) {
                        mulResContainer[i][j] += Aik * other.container[k][j];
                    }
                }
            }

            return Matrix(std::move(mulResContainer), InternalTag{});
        }

        /**
         * @brief Multiplies each element of the matrix by a scalar. (MATRIX * K)
         *
         * @param scalar The scalar value.
         * @return A new Matrix with each element multiplied by the scalar.
         * @note The shape remains unchanged.
         */
        Matrix operator*(double scalar) const {
            Matrix mulRes = *this;
            const size_t totalElements = this->ncols * this->nrows;

            #pragma omp parallel for if(totalElements > OPENMP_THRESHOLD) collapse(2)
            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {
                    mulRes.container[i][j] *= scalar;
                }
            }

            return mulRes;
        }

        /**
         * @brief Multiplies a scalar by a matrix. (K * MATRIX)
         *
         * @param scalar The scalar value.
         * @param other The Matrix to multiply.
         * @return A new Matrix with each element multiplied by the scalar.
         * @note The shape remains unchanged.
         */
        friend Matrix operator*(double scalar, const Matrix& other) {
            return other * scalar;
        }

        /**
         * @brief Divides each element of the matrix by a scalar.
         *
         * @param scalar The scalar value.
         * @return A new Matrix with each element divided by the scalar.
         * @throws std::runtime_error if scalar is zero.
         * @note The shape remains unchanged.
         */
        Matrix operator/(double scalar) const {
            if (scalar == 0) {
                throw std::runtime_error("Division by Zero");
            }

            Matrix devRes = *this;
            return devRes * (1 / scalar);
        }

        Matrix operator^(double scalar) const {

            if (scalar == 1) {
                return *this;
            }

            Matrix expRes = *this;

            for (std::vector<double>& row: expRes.container) {
                for (double& num: row) {
                    if (num == 0 && scalar <= 0) {
                        throw std::runtime_error("Division by zero occured. (0 ^ ( <=0 ))");
                    }
                    num = pow(num, scalar);
                }
            }

            return expRes;
        }

        /**
         * @brief Accesses an element of the matrix at a specified row and column.
         *
         * @param row The row index.
         * @param col The column index.
         * @return Reference to the value at the specified position. (modifiable)
         * @throws std::out_of_range if the indices are out of bounds.
         */
        double& operator()(size_t row, size_t col) {
            if (row >= this->nrows || col >= this->ncols) {
                throw std::out_of_range("Index out of bounds");
            }

            return this->container[row][col];
        }

        /**
         * @brief Transposes the matrix.
         * @return A new matrix of dim (mxn) for a calling matrix of dim (nxm)
         * Example:
         * @code
         * Matrix A({{1, 2}, {3, 4}});
         * A = A.transpose();
         * // A becomes:
         * // [
         * //   [1, 3],
         * //   [2, 4]
         * // ]
         * @endcode
         */
        Matrix transpose() const {

            std::vector<std::vector<double>> transposeContainer(this->ncols, std::vector<double>(this->nrows));

            const size_t totalElements = this->ncols * this->nrows;

            #pragma omp parallel for if(totalElements > OPENMP_THRESHOLD) collapse(2)
            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {
                    transposeContainer[j][i] = this->container[i][j];
                }
            } 

            return Matrix(std::move(transposeContainer), InternalTag{});
        }

        /**
         * @brief Computes the determinant of the matrix.
         *
         * @return The determinant as a double.
         * @throws std::invalid_argument if the matrix is not square.
         * @note The shape of the matrix remains unchanged.
         */
        double determinant() const {
            if (this->nrows != this->ncols) {
                throw std::invalid_argument(
                    "Determinant is only defined for square matrices. Given: " + 
                    std::to_string(nrows) + "x" + std::to_string(ncols)
                );
            }
            
            size_t n = this->container.size();

            std::vector<std::vector<double>> LU = this->container;
            int numRowSwaps = 0;
        
            // Perform LU Decomposition with partial pivoting.
            for (size_t i = 0; i < n; i++) {
                // Find the pivot in column i.
                double maxVal = std::abs(LU[i][i]);
                size_t pivotRow = i;
                for (size_t k = i + 1; k < n; k++) {
                    double val = std::abs(LU[k][i]);
                    if (val > maxVal) {
                        maxVal = val;
                        pivotRow = k;
                    }
                }

                if (std::abs(maxVal) < EPS) {
                    return 0.0;
                }

                if (pivotRow != i) {
                    std::swap(LU[i], LU[pivotRow]);
                    numRowSwaps++;
                }

                for (size_t j = i + 1; j < n; j++) {
                    LU[j][i] /= LU[i][i];
                    for (size_t k = i + 1; k < n; k++) {
                        LU[j][k] -= LU[j][i] * LU[i][k];
                    }
                }
            }

            double det = (numRowSwaps % 2 == 0) ? 1.0 : -1.0;
            for (size_t i = 0; i < n; i++) {
                det *= LU[i][i];
            }

            return det;
        }

        /**
         * @brief Computes the inverse of the matrix.
         *
         * @return A new Matrix representing the inverse.
         * @throws std::runtime_error if the matrix is singular (non-invertible).
         * @note The shape remains unchanged.
         */
        Matrix inverse() const {
            if (nrows != ncols) {
                throw std::invalid_argument(
                    "Matrix must be square to invert. Given: " + 
                    std::to_string(nrows) + "x" + std::to_string(ncols) + "."
                );
            }

            size_t n = nrows;
            
            // Make copies: one for the working matrix (A) and one for the identity (I)
            Matrix A(*this);
            Matrix I = Matrix::identity(n);
            
            // Gauss–Jordan elimination.
            for (size_t i = 0; i < n; ++i) {
                
                size_t pivot = i;
                for (size_t j = i + 1; j < n; ++j) {
                    if (std::abs(A.container[j][i]) > std::abs(A.container[pivot][i])) {
                        pivot = j;
                    }
                }
                if (std::abs(A.container[pivot][i]) < EPS) {
                    throw std::runtime_error("Singular matrix");
                }
                
                
                std::swap(A.container[i], A.container[pivot]);
                std::swap(I.container[i], I.container[pivot]);
                
                // Normalize the pivot row.
                double pivotVal = A.container[i][i];
                for (size_t j = 0; j < n; ++j) {
                    A.container[i][j] /= pivotVal;
                    I.container[i][j] /= pivotVal;
                }
                
                
                for (size_t k = 0; k < n; ++k) {

                    if (k == i) {
                        continue;
                    }

                    double factor = A.container[k][i];
                    for (size_t j = 0; j < n; ++j) {
                        A.container[k][j] -= factor * A.container[i][j];
                        I.container[k][j] -= factor * I.container[i][j];
                    }
                }
            }
            
            return I;
        }

        /**
         * @brief Inserts a new row into the matrix.
         *
         * @param row A vector representing the new row.
         * @param idx The index at which to insert the row.
         * @return A new Matrix with the row inserted.
         * @throws std::invalid_argument if the row size is inconsistent or if idx is out of range.
         */
        Matrix insertRow(std::vector<double> row, size_t idx) const {
            if ( this->ncols != row.size() ) {
                throw std::invalid_argument("Ill formed row. Should be of same size as the rest of the matrix");
            }

            if (idx > this->nrows) {
                throw std::invalid_argument("Row index out of range");
            }

            Matrix hstackRes = *this;
            hstackRes.container.insert(hstackRes.container.begin() + idx, row);
            hstackRes.nrows += 1;

            return hstackRes;
        }

        /**
         * @brief Inserts a new row filled with a constant value into the matrix.
         *
         * @param rowVal The constant value to fill the new row.
         * @param idx The index at which to insert the row.
         * @return A new Matrix with the row inserted.
         * @throws std::invalid_argument if idx is out of range.
         */
        Matrix insertRow(double rowVal, size_t idx) const {
            if (idx > this->nrows) {
                throw std::invalid_argument("Row index out of range");
            }

            Matrix hstackRes = *this;
            hstackRes.container.insert(hstackRes.container.begin() + idx, std::vector<double>(this->ncols, rowVal));
            hstackRes.nrows += 1;

            return hstackRes;
        }

        /**
         * @brief Inserts a new column into the matrix.
         *
         * @param col A vector representing the new column.
         * @param idx The index at which to insert the column.
         * @return A new Matrix with the column inserted.
         * @throws std::invalid_argument if the column size is inconsistent or if idx is out of range.
         */
        Matrix insertCol(std::vector<double> col, size_t idx) const {
            if (this->nrows != col.size()) {
                throw std::invalid_argument("Ill formed column. Should be of same size as the rest of the matrix");
            }

            if (idx > this->ncols) {
                throw std::invalid_argument("Column index out of range");
            }

            Matrix vstackRes = *this;

            size_t colIter = 0;

            for (auto& row : vstackRes.container) {
                row.insert(row.begin() + idx, col[colIter]);
                colIter++;
            }

            vstackRes.ncols += 1;

            return vstackRes;
        }

        /**
         * @brief Inserts a new column filled with a constant value into the matrix.
         *
         * @param colVal The constant value to fill the new column.
         * @param idx The index at which to insert the column.
         * @return A new Matrix with the column inserted.
         * @throws std::invalid_argument if idx is out of range.
         */
        Matrix insertCol(double colVal, size_t idx) const {
            if (idx > this->ncols) {
                throw std::invalid_argument("Column index out of range");
            }

            Matrix vstackRes = *this;

            for (auto& row : vstackRes.container) {
                row.insert(row.begin() + idx, colVal);
            }

            vstackRes.ncols += 1;

            return vstackRes;
        }

        /**
         * @brief Horizontally concatenates two matrices.
         *
         * This function creates and returns a new Matrix by appending the columns of the given matrix
         * to the right of the calling matrix. Both matrices must have the same number of rows.
         *
         * @param other The Matrix whose columns will be appended to the calling matrix.
         * @return A new Matrix representing the horizontal concatenation of the two matrices.
         * @throws std::invalid_argument if the two matrices do not have the same number of rows.
         *
         * @note This implementation reserves the necessary capacity before inserting to minimize reallocations.
         */
        Matrix hStack(const Matrix& other) const {
            if (this->nrows != other.nrows) {
                throw std::invalid_argument("Horizontal stack requires alignment of no. of rows");
            }

            size_t rowIter = 0;
            Matrix hStackRes = *this;

            for (auto& rows: other.container) {
                for (double val: rows) {
                    hStackRes.container[rowIter].push_back(val);
                }
                rowIter++;
            }

            hStackRes.ncols += other.ncols;

            return hStackRes;
        }

        /**
         * @brief Vertically concatenates two matrices.
         *
         * This function creates and returns a new Matrix by appending the rows of the given matrix
         * below the rows of the calling matrix. Both matrices must have the same number of columns.
         *
         * @param other The Matrix whose rows will be appended to the calling matrix.
         * @return A new Matrix representing the vertical concatenation of the two matrices.
         * @throws std::invalid_argument if the two matrices do not have the same number of columns.
         *
         * @note The function appends each row of the second matrix to the container of the first,
         *       and updates the total row count accordingly.
         */
        Matrix vStack(const Matrix& other) const {
            if (this->ncols != other.ncols) {
                throw std::invalid_argument("Vertical stack requires alignment of no. of cols");
            }

            Matrix vStackRes = *this;

            for (auto& row: other.container) {
                vStackRes.container.push_back(row);
            }

            vStackRes.nrows += other.nrows;

            return vStackRes;
        }

        /**
         * @brief Shuffles the rows of the matrix using a random seed.
         *
         * This function uses a random seed generated by std::random_device to
         * initialize the random number generator, ensuring that the shuffle
         * is non-deterministic.
         */
        void shuffleRows() {

            std::random_device rd;
            std::mt19937 gen(rd());

            std::shuffle(this->container.begin(), this->container.end(), gen);
        }

        /**
         * @brief Shuffles the rows of the matrix using a specified seed.
         *
         * This function initializes the random number generator with the provided
         * seed (random_state), allowing for reproducible shuffling.
         *
         * @param random_state The seed value used to initialize the random number generator.
         */
        void shuffleRows(size_t random_state) {

            std::mt19937 gen(random_state);

            std::shuffle(this->container.begin(), this->container.end(), gen);
        }

        /**
         * @brief Extracts a submatrix from the current Matrix.
         *
         * Given a pair of row indices and a pair of column indices, this function creates
         * and returns a new Matrix containing the submatrix defined by the specified ranges [start, end).

         * @param rowSlice A std::pair<size_t, size_t> representing the start and end row indices.
         * @param colSlice A std::pair<size_t, size_t> representing the start and end column indices.
         * @return A new Matrix object containing the extracted submatrix.
         * @throws std::out_of_range If any indices are out of bounds or if the slice ranges are invalid.
         */
        Matrix extractMatrix(std::pair<size_t, size_t> rowSlice, std::pair<size_t, size_t> colSlice) const {
            /* Convert [start, end) to container indices */
            size_t rowStart = rowSlice.first;       
            size_t rowEnd   = rowSlice.second - 1;

            size_t colStart = colSlice.first;
            size_t colEnd   = colSlice.second - 1;

            if (
                rowStart >= this->nrows ||  // Check if rowStart is out of bounds
                rowEnd   >= this->nrows ||  // Check if rowEnd is out of bounds
                rowStart >  rowEnd      ||  // Ensure rowStart comes before rowEnd
                colStart >= this->ncols ||  // Check if colStart is out of bounds
                colEnd   >= this->ncols ||  // Check if colEnd is out of bounds
                colStart > colEnd           // Ensure colStart comes before colEnd
            ) {
                throw std::out_of_range("Slice indices are out of bounds or invalid.");
            }

            std::vector<std::vector<double>> slice(rowEnd - rowStart + 1); 
            /*
            slice = something like this
            {
                {},
                {},
                ...
            }
            */
            size_t sliceRowIndex = 0;

            for (size_t i = rowStart; i <= rowEnd; ++i) {
                slice[sliceRowIndex].reserve(colEnd - colStart + 1);
                for (size_t j = colStart; j <= colEnd; ++j) {
                    slice[sliceRowIndex].push_back(this->container[i][j]);
                }
                sliceRowIndex++;
            }

            return Matrix(std::move(slice), InternalTag{});
        }

        /**
         * @brief Extracts a specific row from the current Matrix.
         *
         * This function creates and returns a new Matrix object containing the row at the specified index.
         * The row is extracted as a single-row submatrix.
         *
         * @param rowIdx The zero-based index of the row to extract.
         * @return A new Matrix object containing the extracted row.
         * @throws std::invalid_argument If the specified row index is out of range.
         */
        Matrix extractRow(size_t rowIdx) const {
            if (rowIdx >= this->nrows) {
                throw std::invalid_argument("Row index out of range.");
            }

            return Matrix({this->container[rowIdx]}, InternalTag{});
        }

        /**
         * @brief Extracts a specific column from the current Matrix.
         *
         * This function calls Matrix::extractMatrix() under the hood
         * as extracting a column will require the exact same logic.
         *
         * @param colIdx The zero-based index of the column to extract.
         * @return A new Matrix object containing the extracted column.
         * @throws std::invalid_argument If the specified column index is out of range.
         */
        Matrix extractCol(size_t colIdx) const {
            if (colIdx >= this->ncols) {
                throw std::invalid_argument("Column index out of range.");
            }

            return this->extractMatrix({0, this->nrows}, {colIdx, colIdx + 1});
        }

        /**
         * @brief Computes the sum of the elements of a vector.
         *
         * Calculates the sum of elements for matrices that are
         * considered as vectors. It supports both column vectors (K x 1) and
         * row vectors (1 x K). 
         *
         * @return The sum of all elements in the vector.
         * @throws std::invalid_argument If the matrix is not a one-dimensional vector.
         */
        double sum() const {
            bool colMatrix = this->ncols == 1;
            bool rowMatrix = this->nrows == 1;

            if ( !colMatrix && !rowMatrix ) {
                throw std::invalid_argument("Sum can only be calculated for (K, 1) or (1, K) dim matrices");
            }

            double total = 0.0;

            if ( rowMatrix ) {
                for (auto& num: this->container[0]) {
                    total += num;
                }

                return total;
            }

            for (auto& row : this->container) {
                total += row[0];
            }

            return total;
        }

        /**
         * @brief Computes the sum of all elements raised to a specified power.
         *
         * This function calculates the sum of each element in the matrix after raising
         * it to the given @p power. The function only works for matrices that are either
         * a row matrix (1 x K) or a column matrix (K x 1). If the matrix has any other dimensions,
         * an std::invalid_argument exception is thrown.
         *
         * Additionally, if an element is zero and the @p power is less than or equal to zero,
         * the function will throw an std::runtime_error to indicate a division by zero scenario,
         * since 0 raised to a non-positive power is undefined.
         *
         * @param power The exponent to which each element in the matrix is raised.
         * @return The sum of all elements raised to the specified power.
         *
         * @throws std::invalid_argument if the matrix dimensions are not (1, K) or (K, 1).
         * @throws std::runtime_error if any element is zero and @p power is less than or equal to zero.
         */
        double sum(double power) const {
            bool colMatrix = this->ncols == 1;
            bool rowMatrix = this->nrows == 1;

            if ( !colMatrix && !rowMatrix ) {
                throw std::invalid_argument("Sum can only be calculated for (K, 1) or (1, K) dim matrices");
            }

            double total = 0.0;

            if ( rowMatrix ) {
                for (auto& num: this->container[0]) {
                    if (num == 0 && power <= 0) {
                        throw std::runtime_error("Division by zero occured. (0 ^ ( <=0 ))");
                    }
                    total += pow(num, power);
                }

                return total;
            }

            for (auto& row : this->container) {
                if (row[0] == 0 && power <= 0) {
                    throw std::runtime_error("Division by zero occured. (0 ^ ( <=0 ))");
                }
                total += pow(row[0], power);
            }

            return total;
        }

        /**
         * @brief Computes the mean (average) of the elements of a vector.
         *
         * Calculates the mean value for matrices that are considered as vectors.
         * It supports both row vectors (1 x K) and column vectors (K x 1) by dividing the sum
         * of the elements by the number of elements in the vector.
         *
         * @return The mean (average) value of the vector elements.
         * @throws std::invalid_argument If the matrix is not a one-dimensional vector.
         */
        double mean() const {
            
            double sum   = this->sum();
            size_t count = (this->nrows == 1) ? this->ncols : this->nrows;
            
            return sum / count;
        }

        /**
         * @brief Computes the trace of an nxn square matrix.
         *
         * @return Trace of calling Matrix object.
         * @throws std::invalid_argument If the matrix is not sqaure.
         */
        double trace() const {
            double tr = 0.0;

            if (this->nrows != this->ncols) {
                throw std::invalid_argument("Trace is only defined for square matrices.");
            }

            for (size_t i = 0; i < this->nrows; ++i) {
                tr += this->container[i][i];
            }

            return tr;
        }

        /**
         * @brief Outputs the matrix to an output stream.
         *
         * @param os The output stream.
         * @param m The Matrix to output.
         * @return A reference to the output stream.
         */
        friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
            os << "[\n";

            for (size_t i = 0; i < m.nrows; ++i) {
                os << "  [";
                for (size_t j = 0; j < m.ncols; ++j) {
                    os << m.container[i][j];

                    if (j < m.ncols - 1) os << ", ";
                }
                os << "]" << (i < m.nrows - 1 ? ",\n" : "\n");
            }
            os << "]\n";
            return os;
        }
};

inline std::ostream& operator<<(std::ostream& os, const std::pair<size_t, size_t>& shape) {
    os << "(" << shape.first << ", " << shape.second << ")";

    return os;
}

inline std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<double>>& container) {
    size_t nrows = container.size();
    size_t ncols = container[0].size();

    os << "[\n";

    for (size_t i = 0; i < nrows; ++i) {
        os << "  [";
        for (size_t j = 0; j < ncols; ++j) {
            os << container[i][j];

            if (j < ncols - 1) os << ", ";
        }
        os << "]" << (i < nrows - 1 ? ",\n" : "\n");
    }
    os << "]\n";
    return os;
}
