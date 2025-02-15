#include <iostream>
#include <vector>
#pragma once

/**
 * @class Matrix
 * @brief A simple linear algebra library for matrix operations.
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
        size_t ncols; ///< Number of columns in the matrix.
        size_t nrows; ///< Number of rows in the matrix.

        std::vector<std::vector<double>> container; ///< 2D vector holding matrix elements.

        /**
         * @brief Recursively computes the determinant of a given square matrix.
         *
         * @param matrix A 2D vector representing a square matrix.
         * @return The determinant as a double.
         * @note This is a private helper function used by the public determinant() method.
         */
        double _det(const std::vector<std::vector<double>> &matrix) const {

            size_t n = matrix.size();
            
            // Base case: 1x1 matrix
            if (n == 1) {
                return matrix[0][0];
            }
            
            // Base case: 2x2 matrix
            if (n == 2) {
                return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
            }
            
            double det = 0.0;

            for (size_t col = 0; col < n; col++) {
                std::vector<std::vector<double>> minor;
                for (size_t i = 1; i < n; i++) {
                    std::vector<double> row;
                    for (size_t j = 0; j < n; j++) {
                        if (j == col)
                            continue;
                        row.push_back(matrix[i][j]);
                    }
                    minor.push_back(row);
                }
                
                double sign = (col % 2 == 0) ? 1.0 : -1.0;
                det += sign * matrix[0][col] * _det(minor);
            }
            
            return det;
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
                this->ncols = 0;
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
         * @brief Constructs a Matrix with specified dimensions and an initial value.
         *
         * @param rows Number of rows.
         * @param cols Number of columns.
         * @param initialValue The value to initialize each element.
         * @throws std::invalid_argument if rows or cols are 0.
         * @note The shape is (rows x cols).
         */
        Matrix(size_t rows, size_t cols, double initialValue) {
            if (rows == 0 || cols == 0) {
                throw std::invalid_argument("Matrix dimensions must be greater than 0.");
            }

            this->nrows = rows;
            this->ncols = cols;

            this->container = std::vector<std::vector<double>>(rows, std::vector<double>(cols, initialValue));
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
                throw std::invalid_argument("Matrix dims don't align");
            }

            Matrix addRes = *this; // addRes = A in A + B

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
                throw std::invalid_argument("Matrix dims don't align");
            }

            Matrix subRes = *this; // subRes = A in A - B

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

            constexpr double EPS = 1e-9;

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
                throw std::invalid_argument("Incorrect dims: For matrix m x n and p x r, n must be equal to p.");
            }

            std::vector<std::vector<double>> mulResContainer(this->nrows, std::vector<double>(other.ncols));

            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 0; j < other.ncols; ++j) {
                    double sum = 0.0;
                    
                    for (size_t k = 0; k < this->ncols; ++k) {
                        sum += this->container[i][k] * other.container[k][j];
                    }

                    mulResContainer[i][j] = sum;
                }
            }

            return Matrix(mulResContainer);
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

            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {
                    transposeContainer[j][i] = this->container[i][j];
                }
            } 

            return Matrix(transposeContainer);
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
                throw std::invalid_argument("Det only for sq. matrices");
            }

            size_t n = this->nrows;
            
            return _det(this->container);
        }

        /**
         * @brief Computes the inverse of the matrix.
         *
         * @return A new Matrix representing the inverse.
         * @throws std::runtime_error if the matrix is singular (non-invertible).
         * @note The shape remains unchanged.
         */
        Matrix inverse() const {
            double det = this->determinant();
            constexpr double EPS = 1e-9;

            if (std::abs(det) < EPS) {
                throw std::runtime_error("Singular matrix");
            }

            std::vector<std::vector<double>> cof(this->nrows, std::vector<double>(this->ncols, 0.0));

            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {

                    std::vector<std::vector<double>> minor;

                    for (size_t mi = 0; mi < this->nrows; ++mi) {

                        if (mi == i) continue;

                        std::vector<double> row;

                        for (size_t mj = 0; mj < this->ncols; ++mj) {
                            if (mj == j) continue;

                            row.push_back(this->container[mi][mj]);
                        }

                        minor.push_back(row);
                    }

                    double minorDet = _det(minor);
                    double sign = ((i + j) % 2 == 0) ? 1.0 : -1.0;

                    cof[i][j] = sign * minorDet;
                }
            }

            Matrix inv(cof);
            inv.transpose();

            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {
                    inv.container[i][j] = inv.container[i][j] / det;
                }
            }

            return inv;
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
         * @brief Extracts a submatrix from the current Matrix.
         *
         * Given a pair of row indices and a pair of column indices, this function creates
         * and returns a new Matrix containing the submatrix defined by the specified ranges.
         * Both row and column ranges are inclusive, meaning that the elements at both the start
         * and end indices are included in the result.
         *
         * @param rowSlice A std::pair<size_t, size_t> representing the start and end row indices (inclusive).
         * @param colSlice A std::pair<size_t, size_t> representing the start and end column indices (inclusive).
         * @return A new Matrix object containing the extracted submatrix.
         * @throws std::out_of_range If any indices are out of bounds or if the slice ranges are invalid.
         *
         * @note Indices are zero-based (i.e., valid indices range from 0 to size() - 1).
         */
        Matrix extractMatrix(std::pair<size_t, size_t> rowSlice, std::pair<size_t, size_t> colSlice) const {
            size_t rowStart = rowSlice.first;
            size_t rowEnd   = rowSlice.second;

            size_t colStart = colSlice.first;
            size_t colEnd   = colSlice.second;

            if (
                rowStart < 0 || rowStart >= this->nrows ||  // Check if rowStart is out of bounds
                rowEnd < 0 || rowEnd >= this->nrows ||      // Check if rowEnd is out of bounds
                rowStart > rowEnd ||                      // Ensure rowStart comes before rowEnd
                colStart < 0 || colStart >= this->ncols || // Check if colStart is out of bounds
                colEnd < 0 || colEnd >= this->ncols ||      // Check if colEnd is out of bounds
                colStart > colEnd                         // Ensure colStart comes before colEnd
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
                for (size_t j = colStart; j <= colEnd; ++j) {
                    slice[sliceRowIndex].push_back(this->container[i][j]);
                }
                sliceRowIndex++;
            }

            return Matrix(slice);
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
