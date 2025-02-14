#include <iostream>
#include <vector>
#pragma once

typedef class Matrix {
    private:
        size_t ncols;
        size_t nrows;
        std::vector<std::vector<double>> container;

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
        std::pair<size_t, size_t> shape() const { return {nrows, ncols}; }

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

        Matrix(size_t rows, size_t cols, double initialValue) {
            if (rows == 0 || cols == 0) {
                throw std::invalid_argument("Matrix dimensions must be greater than 0.");
            }

            this->nrows = rows;
            this->ncols = cols;

            this->container = std::vector<std::vector<double>>(rows, std::vector<double>(cols, initialValue));
        }

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

        Matrix operator+(double scalar) const {
            Matrix addRes = *this;

            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {
                    addRes.container[i][j] += scalar;
                }
            }

            return addRes;
        }

        friend Matrix operator+(double scalar, const Matrix& other) {
            return other + scalar;
        }

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

        Matrix operator-(double scalar) const {
            Matrix subRes = *this; // subRes = A in A - B

            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {
                    subRes.container[i][j] -= scalar;
                }
            }

            return subRes;
        }

        friend Matrix operator-(double scalar, const Matrix& other) {
            Matrix subRes = other;

            for (size_t i = 0; i < subRes.nrows; ++i) {
                for (size_t j = 0; j < subRes.ncols; ++j) {
                    subRes.container[i][j] = scalar - subRes.container[i][j];
                }
            }

            return subRes;
        }

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

        Matrix operator*(double scalar) const {
            Matrix mulRes = *this;

            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {
                    mulRes.container[i][j] *= scalar;
                }
            }

            return mulRes;
        }

        friend Matrix operator*(double scalar, const Matrix& other) {
            return other * scalar;
        }

        Matrix operator/(double scalar) const {
            if (scalar == 0) {
                throw std::runtime_error("Division by Zero");
            }

            Matrix devRes = *this;
            return devRes * (1 / scalar);
        }

        double operator()(size_t row, size_t col) const {
            if (row >= this->nrows || col >= this->ncols) {
                throw std::out_of_range("Index out of bounds");
            }

            return this->container[row][col];
        }

        void set(size_t row, size_t col, double val) {
            if (row >= this->nrows || col >= this->ncols) {
                throw std::out_of_range("Index out of bounds");
            }

            this->container[row][col] = val;
        }

        void transpose() {

            if (this->nrows == this->ncols) {
                for (size_t i = 0; i < this->nrows; ++i) {
                    for (size_t j = i + 1; j < this->ncols; ++j) {
                        std::swap(this->container[i][j], this->container[j][i]);
                    }
                }

                return;
            }

            std::vector<std::vector<double>> transposeContainer(this->ncols, std::vector<double>(this->nrows));

            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {
                    transposeContainer[j][i] = this->container[i][j];
                }
            } 

            std::swap(this->nrows, this->ncols);

            this->container = std::move(transposeContainer);
        }

        double determinant() const {
            if (this->nrows != this->ncols) {
                throw std::invalid_argument("Det only for sq. matrices");
            }

            size_t n = this->nrows;
            
            return _det(this->container);
        }

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

        Matrix insertRow(double rowVal, size_t idx) const {
            if (idx > this->nrows) {
                throw std::invalid_argument("Row index out of range");
            }

            Matrix hstackRes = *this;
            hstackRes.container.insert(hstackRes.container.begin() + idx, std::vector<double>(this->ncols, rowVal));
            hstackRes.nrows += 1;

            return hstackRes;
        }

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
} mat;
