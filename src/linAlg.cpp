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
        std::pair<size_t, size_t> shape;

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
            this->shape     = {nrows, ncols};
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

        Matrix operator-(const Matrix& other) const {
            if (this->nrows != other.nrows || this->ncols != other.ncols) {
                throw std::invalid_argument("Matrix dims don't align");
            }

            Matrix addRes = *this; // addRes = A in A + B

            for (size_t i = 0; i < this->nrows; ++i) {
                for (size_t j = 0; j < this->ncols; ++j) {
                    addRes.container[i][j] -= other.container[i][j];
                }
            }

            return addRes;
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
                std::cout << this->ncols << " " << other.nrows << std::endl;
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

        double operator()(size_t row, size_t col) {
            if (row >= this->nrows || col >= this->ncols) {
                throw std::out_of_range("Index out of bounds");
            }

            return this->container[row][col];
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

        Matrix inverse() {
            double det = this->determinant();

            if (det == 0) {
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

        void showMatrix() {
            std::cout << "[\n";  

            for (size_t i = 0; i < this->nrows; ++i) {
                std::cout << "  [";
                for (size_t j = 0; j < this->ncols; ++j) {
                    std::cout << this->container[i][j];  
                    if (j < this->ncols - 1) {
                        std::cout << ", ";  
                    }
                }
                std::cout << "]";  
                if (i < this->nrows - 1) {
                    std::cout << ",\n";  
                }
                else {
                    std::cout << "\n";  
                }
            }
        
            std::cout << "]\n";  
        }
} mat;
