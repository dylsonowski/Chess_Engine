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
        Matrix(unsigned short int rows, unsigned short int columns, std::vector<double> values) {
            for (int rowIterator = 0; rowIterator < rows; rowIterator++) {
                std::vector<double> tempRow;
                for (int columnIterator = 0; columnIterator < columns; columnIterator++) {
                    tempRow.emplace_back(values.at(rowIterator * columnIterator));
                }
                _matrix.emplace_back(tempRow);
            }

            PALE_ENGINE_INFO("Matrix.h->Matrix constructor [28]: New Matrix object has been created! Matrix dimentions: {0}x{1}. First matrix value: {3}", _rows, _columns, _matrix.at(0).at(0));
        }
        ~Matrix() = default;

        //--- Getters & Setters ---//
        inline unsigned short int GetRowsNumber() const { return _rows; }
        inline unsigned short int GetColumnsNumber() const { return _columns; }
        inline double GetValue(unsigned short int row, unsigned short int column) const { return _matrix.at(row).at(column); }

        //--- Overloaded operators ---//
        Matrix& operator+(const Matrix& other) {
            try {
                if(_rows != other._rows || _columns != other._columns)
                    throw PaleEngineException("Exception happened!", 'e', "Matrix.h", 25, "Matrix operator+", MATH__MATRICES_DIMENTIONS_INCORRECT);

                Matrix tempMatrix(_rows, _columns, false);
                for (int rowIterator = 0; rowIterator < _rows; rowIterator++) {
                    for (int columnIterator = 0; columnIterator < _columns; columnIterator++) {
                        tempMatrix._matrix.at(rowIterator).at(columnIterator) = _matrix.at(rowIterator).at(columnIterator) + other._matrix.at(rowIterator).at(columnIterator);
                    }
                }

                PALE_ENGINE_TRACE("Matrix.h->Matrix operator+ [34]: Successful summation of 2 matrices. Output matrix dimentions: {0}x{1}. First matrix value: {3}", tempMatrix._rows, tempMatrix._columns, tempMatrix._matrix.at(0).at(0));
                return tempMatrix;
            }
            catch (PaleEngineException& exception) {
                if (exception.GetType() == 'e')
                    PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
                else if (exception.GetType() == 'w')
                    PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
            }
        }
        Matrix& operator-(const Matrix& other) {
            if (_rows != other._rows || _columns != other._columns)
                throw PaleEngineException("Exception happened!", 'e', "Matrix.h", 46, "Matrix operator-", MATH__MATRICES_DIMENTIONS_INCORRECT);

            Matrix tempMatrix(_rows, _columns, false);
            for (int rowIterator = 0; rowIterator < _rows; rowIterator++) {
                for (int columnIterator = 0; columnIterator < _columns; columnIterator++) {
                    tempMatrix._matrix.at(rowIterator).at(columnIterator) = _matrix.at(rowIterator).at(columnIterator) - other._matrix.at(rowIterator).at(columnIterator);
                }
            }

            PALE_ENGINE_TRACE("Matrix.h->Matrix operator- [55]: Successful subtraction of 2 matrices. Output matrix dimentions: {0}x{1}. First matrix value: {3}", tempMatrix._rows, tempMatrix._columns, tempMatrix._matrix.at(0).at(0));
            return tempMatrix;
        }
        Matrix& operator*(const Matrix& other) {
            if (_columns != other._rows)
                throw PaleEngineException("Exception happened!", 'e', "Matrix.h", 60, "Matrix operator*", MATH__MATRICES_DIMENTIONS_INCORRECT);

            Matrix tempMatrix(_rows, _columns, false);
            for (int rowIterator = 0; rowIterator < _rows; rowIterator++) {
                for (int columnIterator = 0; columnIterator < _columns; columnIterator++) {
                    tempMatrix._matrix.at(rowIterator).at(columnIterator) = 0;
                    for (int multiplicationIterator = 0; multiplicationIterator < _columns; multiplicationIterator++) {
                        tempMatrix._matrix.at(rowIterator).at(columnIterator) += _matrix.at(rowIterator).at(multiplicationIterator) * other._matrix.at(multiplicationIterator).at(columnIterator);
                    }
                }
            }

            PALE_ENGINE_TRACE("Matrix.h->Matrix operator* [72]: Successful multiplication of 2 matrices. Output matrix dimentions: {0}x{1}. First matrix value: {3}", tempMatrix._rows, tempMatrix._columns, tempMatrix._matrix.at(0).at(0));
            return tempMatrix;
        }
        Matrix& operator+(const double value) {
            Matrix tempMatrix(_rows, _columns, false);
            for (int rowIterator = 0; rowIterator < _rows; rowIterator++) {
                for (int columnIterator = 0; columnIterator < _columns; columnIterator++) {
                    tempMatrix._matrix.at(rowIterator).at(columnIterator) = _matrix.at(rowIterator).at(columnIterator) + value;
                }
            }

            PALE_ENGINE_TRACE("Matrix.h->Matrix operator+ [83]: Successful summation of matrix and scalar. Output matrix dimentions: {0}x{1}. First matrix value: {3}", tempMatrix._rows, tempMatrix._columns, tempMatrix._matrix.at(0).at(0));
            return tempMatrix;
        }
        Matrix& operator-(const double value) {
            Matrix tempMatrix(_rows, _columns, false);
            for (int rowIterator = 0; rowIterator < _rows; rowIterator++) {
                for (int columnIterator = 0; columnIterator < _columns; columnIterator++) {
                    tempMatrix._matrix.at(rowIterator).at(columnIterator) = _matrix.at(rowIterator).at(columnIterator) - value;
                }
            }

            PALE_ENGINE_TRACE("Matrix.h->Matrix operator- [94]: Successful subtraction of matrix and scalar. Output matrix dimentions: {0}x{1}. First matrix value: {3}", tempMatrix._rows, tempMatrix._columns, tempMatrix._matrix.at(0).at(0));
            return tempMatrix;
        }
        Matrix& operator*(const double value) {
            Matrix tempMatrix(_rows, _columns, false);
            for (int rowIterator = 0; rowIterator < _rows; rowIterator++) {
                for (int columnIterator = 0; columnIterator < _columns; columnIterator++) {
                    tempMatrix._matrix.at(rowIterator).at(columnIterator) = _matrix.at(rowIterator).at(columnIterator) * value;
                }
            }

            PALE_ENGINE_TRACE("Matrix.h->Matrix operator* [105]: Successful multiplication of matrix and scalar. Output matrix dimentions: {0}x{1}. First matrix value: {3}", tempMatrix._rows, tempMatrix._columns, tempMatrix._matrix.at(0).at(0));
            return tempMatrix;
        }

    private:
        unsigned short int _rows, _columns;
        std::vector<std::vector<double>> _matrix;
    };
}