#include <iostream>
#include <vector>
#pragma once

typedef class Matrix {
    private:
        size_t ncols;
        size_t nrows;
        std::vector<std::vector<double>> container;

    public:
        std::pair<size_t, size_t> shape;

        Matrix(const std::vector<std::vector<double>>& container) {
            this->nrows = container.size();

            if ( this->nrows == 0 ) {
                this->ncols = 0;
                std::cerr << "[Warning]: Matrix is empty!" << std::endl;
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
