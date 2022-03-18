#pragma once

namespace Pale::Math {
    class Matrix {

    public:
        Matrix() = delete;
        Matrix(unsigned short int rows, unsigned short int columns, bool randValues): _rows(rows), _columns(columns) {
            for (int rowIterator = 0; rowIterator < rows; rowIterator++) {
                std::vector<double> tempRow;
                for (int columnIterator = 0; columnIterator < columns; columnIterator++) {
                    randValues ? tempRow.emplace_back(GenerateRandomNumber<double>(-1, 1)) : 0;
                }
                _matrix.emplace_back(tempRow);
            }

            PALE_ENGINE_INFO("Matrix.h->Matrix constructor [17]: New Matrix object has been created! Matrix dimentions: {0}x{1}. First matrix value: {3}", _rows, _columns, _matrix.at(0).at(0));
        }
        ~Matrix() = default;

        //--- Overloaded operators ---//
        Matrix& operator+(const Matrix& other) {
            try {
                if(_rows != other._rows || _columns != other._columns)
                    throw PaleEngineException("Exception happened!", 'e', "Matrix.h", 25, "Matrix operator+", MATH__MATRICES_DIMENTIONS_INCORECT);

                for (int rowIterator = 0; rowIterator < _rows; rowIterator++) {
                    for (int columnIterator = 0; columnIterator < _columns; columnIterator++) {
                        _matrix.at(rowIterator).at(columnIterator) += other._matrix.at(rowIterator).at(columnIterator);
                    }
                }

                PALE_ENGINE_TRACE("Matrix.h->Matrix operator+ [33]: Successful summation of 2 matrices. Output matrix dimentions: {0}x{1}. First matrix value: {3}", _rows, _columns, _matrix.at(0).at(0));
                return *this;
            }
            catch (PaleEngineException& exception) {
                if (exception.GetType() == 'e')
                    PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
                else if (exception.GetType() == 'w')
                    PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
            }
        }
        Matrix& operator-(const Matrix& other) {
            for (int rowIterator = 0; rowIterator < _rows; rowIterator++) {
                for (int columnIterator = 0; columnIterator < _columns; columnIterator++) {
                    _matrix.at(rowIterator).at(columnIterator) -= other._matrix.at(rowIterator).at(columnIterator);
                }
            }

            PALE_ENGINE_TRACE("Matrix.h->Matrix operator- [50]: Successful subtraction of 2 matrices. Output matrix dimentions: {0}x{1}. First matrix value: {3}", _rows, _columns, _matrix.at(0).at(0));
            return *this;
        }
        Matrix& operator*(const Matrix& other) {
            for (int rowIterator = 0; rowIterator < _rows; rowIterator++) {
                for (int columnIterator = 0; columnIterator < _columns; columnIterator++) {
                    _matrix.at(rowIterator).at(columnIterator) -= other._matrix.at(rowIterator).at(columnIterator);
                }
            }

            PALE_ENGINE_TRACE("Matrix.h->Matrix operator* [60]: Successful subtraction of 2 matrices. Output matrix dimentions: {0}x{1}. First matrix value: {3}", _rows, _columns, _matrix.at(0).at(0));
            return *this;
        }

    private:
        unsigned short int _rows, _columns;
        std::vector<std::vector<double>> _matrix;
    };
}