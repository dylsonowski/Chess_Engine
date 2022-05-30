#pragma once
#include "Basic_Math.h"

namespace Pale::Math {
	class Matrix {

	public:
		Matrix(): _rows(1),
				  _columns(1) {
			_matrix = { { 0 } };

			PALE_ENGINE_INFO("Matrix.h->Matrix constructor [11]: New Matrix object has been created! Matrix dimensions: {0}x{1}. First matrix value: {2}", _rows, _columns, _matrix.at(0).at(0));
		}
		Matrix(unsigned short int rows, unsigned short int columns, bool randValues): _rows(rows),
																					  _columns(columns) {
			for (int rowIterator = 0; rowIterator < rows; rowIterator++) {
				std::vector<double> tempRow;
				for (int columnIterator = 0; columnIterator < columns; columnIterator++) {
					randValues ? tempRow.emplace_back(GenerateRandomNumber<double>(-1, 1)) : tempRow.emplace_back(0);
				}
				_matrix.emplace_back(tempRow);
			}

			PALE_ENGINE_INFO("Matrix.h->Matrix constructor [17]: New Matrix object has been created! Matrix dimensions: {0}x{1}. First matrix value: {2}", _rows, _columns, _matrix.at(0).at(0));
		}
		Matrix(unsigned short int rows, unsigned short int columns, std::vector<double> values): _rows(rows),
																								 _columns(columns) {
			try {
				if (values.size() != rows * columns)
					throw PaleEngineException("Exception happened!", 'e', "Matrix.h", 30, "Matrix constructor", MATH__INPUT_VECTOR_INVALID_SIZE);

				int vectorIterator = 0;
				for (int rowIterator = 0; rowIterator < rows; rowIterator++) {
					std::vector<double> tempRow;
					for (int columnIterator = 0; columnIterator < columns; columnIterator++) {
						tempRow.emplace_back(values.at(vectorIterator));
						vectorIterator++;
					}
					_matrix.emplace_back(tempRow);
				}

				PALE_ENGINE_INFO("Matrix.h->Matrix constructor [42]: New Matrix object has been created! Matrix dimensions: {0}x{1}. First matrix value: {2}", _rows, _columns, _matrix.at(0).at(0));
			} catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
			}
		}
		Matrix(unsigned short int rows, unsigned short int columns, double fillValue): _rows(rows),
																					   _columns(columns) {
			for (int rowIterator = 0; rowIterator < rows; rowIterator++) {
				std::vector<double> tempRow;
				for (int columnIterator = 0; columnIterator < columns; columnIterator++) {
					tempRow.emplace_back(fillValue);
				}
				_matrix.emplace_back(tempRow);
			}

			PALE_ENGINE_INFO("Matrix.h->Matrix constructor [28]: New Matrix object has been created! Matrix dimensions: {0}x{1}. First matrix value: {2}", _rows, _columns, _matrix.at(0).at(0));
		}
		~Matrix() = default;

		//--- Getters & Setters ---//
		inline unsigned short int GetRowsNumber() const { return _rows; }
		inline unsigned short int GetColumnsNumber() const { return _columns; }
		inline double GetValue(unsigned short int row, unsigned short int column) const {
			try {
				if (row >= _rows || column >= _columns)
					throw PaleEngineException("Exception happened!", 'e', "Matrix.h", 72, "SetMatrixValue()", VEC_OUT_OF_RANGE);

				return _matrix.at(row).at(column);
			} catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
			}
		}
		inline std::pair<unsigned short int, unsigned short int> GetMatrixSize() const { return std::make_pair(_rows, _columns); }
		inline void SetMatrixValue(unsigned short int row, unsigned short int column, double value) {
			try {
				if (row >= _rows || column >= _columns)
					throw PaleEngineException("Exception happened!", 'e', "Matrix.h", 72, "SetMatrixValue()", VEC_OUT_OF_RANGE);

				_matrix.at(row).at(column) = value;
			} catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
			}
		}

		//--- Overloaded operators ---//
		inline Matrix operator+(const Matrix& other) {
			try {
				if (_rows != other._rows || _columns != other._columns)
					throw PaleEngineException("Exception happened!", 'e', "Matrix.h", 25, "Matrix operator+", MATH__MATRICES_DIMENSIONS_INCORRECT);

				Matrix tempMatrix(_rows, _columns, false);
				for (int rowIterator = 0; rowIterator < _rows; rowIterator++) {
					for (int columnIterator = 0; columnIterator < _columns; columnIterator++) {
						tempMatrix._matrix.at(rowIterator).at(columnIterator) = _matrix.at(rowIterator).at(columnIterator) + other._matrix.at(rowIterator).at(columnIterator);
					}
				}

				PALE_ENGINE_TRACE("Matrix.h->Matrix operator+ [34]: Successful summation of 2 matrices. Output matrix dimensions: {0}x{1}. First matrix value: {2}", tempMatrix._rows, tempMatrix._columns, tempMatrix._matrix.at(0).at(0));
				return tempMatrix;
			} catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
			}
		}
		inline Matrix operator+=(const Matrix& other) {
			try {
				if (_rows != other._rows || _columns != other._columns)
					throw PaleEngineException("Exception happened!", 'e', "Matrix.h", 25, "Matrix operator+", MATH__MATRICES_DIMENSIONS_INCORRECT);

				for (int rowIterator = 0; rowIterator < _rows; rowIterator++) {
					for (int columnIterator = 0; columnIterator < _columns; columnIterator++) {
						_matrix.at(rowIterator).at(columnIterator) += other._matrix.at(rowIterator).at(columnIterator);
					}
				}

				PALE_ENGINE_TRACE("Matrix.h->Matrix operator+ [34]: Successful summation of 2 matrices. Output matrix dimensions: {0}x{1}. First matrix value: {2}", _rows, _columns, _matrix.at(0).at(0));

				return *this;
			} catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
			}
		}
		inline Matrix operator-(const Matrix& other) {
			if (_rows != other._rows || _columns != other._columns)
				throw PaleEngineException("Exception happened!", 'e', "Matrix.h", 46, "Matrix operator-", MATH__MATRICES_DIMENSIONS_INCORRECT);

			Matrix tempMatrix(_rows, _columns, false);
			for (int rowIterator = 0; rowIterator < _rows; rowIterator++) {
				for (int columnIterator = 0; columnIterator < _columns; columnIterator++) {
					tempMatrix._matrix.at(rowIterator).at(columnIterator) = _matrix.at(rowIterator).at(columnIterator) - other._matrix.at(rowIterator).at(columnIterator);
				}
			}

			PALE_ENGINE_TRACE("Matrix.h->Matrix operator- [55]: Successful subtraction of 2 matrices. Output matrix dimensions: {0}x{1}. First matrix value: {2}", tempMatrix._rows, tempMatrix._columns, tempMatrix._matrix.at(0).at(0));
			return tempMatrix;
		}
		inline Matrix operator*(const Matrix& other) {
			try {
				if (_columns != other._rows)
					throw PaleEngineException("Exception happened!", 'e', "Matrix.h", 60, "Matrix operator*", MATH__MATRICES_DIMENSIONS_INCORRECT);

				Matrix tempMatrix(_rows, other._columns, false);
				for (int rowIterator = 0; rowIterator < _rows; rowIterator++) {
					for (int columnIterator = 0; columnIterator < other._columns; columnIterator++) {
						tempMatrix._matrix.at(rowIterator).at(columnIterator) = 0;
						for (int multiplicationIterator = 0; multiplicationIterator < _columns; multiplicationIterator++) {
							tempMatrix._matrix.at(rowIterator).at(columnIterator) += _matrix.at(rowIterator).at(multiplicationIterator) * other._matrix.at(multiplicationIterator).at(columnIterator);
						}
					}
				}

				PALE_ENGINE_TRACE("Matrix.h->Matrix operator* [72]: Successful multiplication of 2 matrices. Output matrix dimensions: {0}x{1}. First matrix value: {2}", tempMatrix._rows, tempMatrix._columns, tempMatrix._matrix.at(0).at(0));
				return tempMatrix;
			} catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
			}
		}
		inline Matrix operator*=(const Matrix& other) {
			try {
				if (_columns != other._rows)
					throw PaleEngineException("Exception happened!", 'e', "Matrix.h", 60, "Matrix operator*", MATH__MATRICES_DIMENSIONS_INCORRECT);

				for (int rowIterator = 0; rowIterator < _rows; rowIterator++) {
					for (int columnIterator = 0; columnIterator < other._columns; columnIterator++) {
						double tempCellValue = 0;
						for (int multiplicationIterator = 0; multiplicationIterator < _columns; multiplicationIterator++) {
							tempCellValue += _matrix.at(rowIterator).at(multiplicationIterator) * other._matrix.at(multiplicationIterator).at(columnIterator);
						}
						_matrix.at(rowIterator).at(columnIterator) = tempCellValue;
					}
				}

				PALE_ENGINE_TRACE("Matrix.h->Matrix operator*= [117]: Successful multiplication of 2 matrices. Output matrix dimensions: {0}x{1}. First matrix value: {2}", _rows, _columns, _matrix.at(0).at(0));
				return *this;
			} catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
			}
		}
		inline Matrix operator+(const double value) {
			Matrix tempMatrix(_rows, _columns, false);
			for (int rowIterator = 0; rowIterator < _rows; rowIterator++) {
				for (int columnIterator = 0; columnIterator < _columns; columnIterator++) {
					tempMatrix._matrix.at(rowIterator).at(columnIterator) = _matrix.at(rowIterator).at(columnIterator) + value;
				}
			}

			PALE_ENGINE_TRACE("Matrix.h->Matrix operator+ [83]: Successful summation of matrix and scalar. Output matrix dimensions: {0}x{1}. First matrix value: {2}", tempMatrix._rows, tempMatrix._columns, tempMatrix._matrix.at(0).at(0));
			return tempMatrix;
		}
		inline Matrix operator-(const double value) {
			Matrix tempMatrix(_rows, _columns, false);
			for (int rowIterator = 0; rowIterator < _rows; rowIterator++) {
				for (int columnIterator = 0; columnIterator < _columns; columnIterator++) {
					tempMatrix._matrix.at(rowIterator).at(columnIterator) = _matrix.at(rowIterator).at(columnIterator) - value;
				}
			}

			PALE_ENGINE_TRACE("Matrix.h->Matrix operator- [94]: Successful subtraction of matrix and scalar. Output matrix dimensions: {0}x{1}. First matrix value: {2}", tempMatrix._rows, tempMatrix._columns, tempMatrix._matrix.at(0).at(0));
			return tempMatrix;
		}
		inline Matrix operator*(const double value) {
			Matrix tempMatrix(_rows, _columns, false);
			for (int rowIterator = 0; rowIterator < _rows; rowIterator++) {
				for (int columnIterator = 0; columnIterator < _columns; columnIterator++) {
					tempMatrix._matrix.at(rowIterator).at(columnIterator) = _matrix.at(rowIterator).at(columnIterator) * value;
				}
			}

			PALE_ENGINE_TRACE("Matrix.h->Matrix operator* [122]: Successful multiplication of matrix and scalar. Output matrix dimensions: {0}x{1}. First matrix value: {2}", tempMatrix._rows, tempMatrix._columns, tempMatrix._matrix.at(0).at(0));
			return tempMatrix;
		}
		inline Matrix operator*=(const double value) {
			for (int rowIterator = 0; rowIterator < _rows; rowIterator++) {
				for (int columnIterator = 0; columnIterator < _columns; columnIterator++) {
					_matrix.at(rowIterator).at(columnIterator) = _matrix.at(rowIterator).at(columnIterator) * value;
				}
			}

			PALE_ENGINE_TRACE("Matrix.h->Matrix operator*= [142]: Successful multiplication of matrix and scalar. Output matrix dimensions: {0}x{1}. First matrix value: {2}", _rows, _columns, _matrix.at(0).at(0));
			return *this;
		}
		inline Matrix operator~() {
			Matrix tempMatrix(_columns, _rows, false);
			for (int rowIterator = 0; rowIterator < _rows; rowIterator++) {
				for (int columnIterator = 0; columnIterator < _columns; columnIterator++) {
					tempMatrix._matrix.at(columnIterator).at(rowIterator) = _matrix.at(rowIterator).at(columnIterator);
				}
			}

			PALE_ENGINE_TRACE("Matrix.h->Matrix operator~ [133]: Successful transpose of the matrix. Output matrix dimensions: {0}x{1}. First matrix value: {2}", tempMatrix._rows, tempMatrix._columns, tempMatrix._matrix.at(0).at(0));
			return tempMatrix;
		}

		//--- Void functions ---//
		template <class _Fn>
		inline Matrix Map(_Fn function) {
			std::for_each(_matrix.begin(), _matrix.end(), [&function](std::vector<double>& matrixColumn) {
				std::for_each(matrixColumn.begin(), matrixColumn.end(), [&function](double& element) {
					return element = function(element);
				});
			});

			PALE_ENGINE_INFO("Matrix.h->Map() [212]: Function has been successfully implemented on matrix values. First matrix value: {0}.", _matrix.at(0).at(0));
			return *this;
		}

		//--- Returning functions ---//
		inline std::string ToString(bool listView = false) const {
			std::stringstream ss;
			if (!listView) {
				for (int rowIterator = 0; rowIterator < _rows; rowIterator++) {
					for (int columnIterator = 0; columnIterator < _columns; columnIterator++) {
						if (_columns == 1)
							ss << "| " << _matrix.at(rowIterator).at(columnIterator) << " |";
						else if (columnIterator == 0)
							ss << "| " << _matrix.at(rowIterator).at(columnIterator);
						else if (columnIterator == _columns - 1)
							ss << " " << _matrix.at(rowIterator).at(columnIterator) << " |";
						else
							ss << " " << _matrix.at(rowIterator).at(columnIterator);
					}
					ss << "\n";
				}
			} else {
				for (int rowIterator = 0; rowIterator < _rows; rowIterator++) {
					for (int columnIterator = 0; columnIterator < _columns; columnIterator++) {
						if (columnIterator == _columns - 1)
							ss << _matrix.at(rowIterator).at(columnIterator) << ".";
						else
							ss << _matrix.at(rowIterator).at(columnIterator) << ", ";
					}
				}
			}

			return ss.str();
		}
		inline static Matrix HadamardMultiplication(const Matrix& first, const Matrix& second, bool summAllElements) {
			try {
				if (first._rows != second._rows || first._columns != 1 || second._columns != 1)
					throw PaleEngineException("Exception happened!", 'e', "Matrix.h", 166, "HadamardMultiplication", MATH__MATRICES_DIMENSIONS_INCORRECT);

				Matrix tempMatrix(first._rows, first._columns, false);
				for (int matrixIterator = 0; matrixIterator < first._rows; matrixIterator++) {
					tempMatrix._matrix.at(matrixIterator).at(0) = first._matrix.at(matrixIterator).at(0) * second._matrix.at(matrixIterator).at(0);
				}

				if (summAllElements) {
					Matrix summedTempMatrix(1, 1, false);
					for (const auto tempMatrixIterator : tempMatrix._matrix) {
						summedTempMatrix._matrix.at(0).at(0) += tempMatrixIterator.at(0);
					}

					PALE_ENGINE_TRACE("Matrix.h->HadamardMultiplication() [181]: Elemental summary of 2 matrices has been successful. Output matrix dimensions: {0}x{1}. Output value: {2}. Output matrix values has been summed into single value!", summedTempMatrix._rows, summedTempMatrix._columns, summedTempMatrix._matrix.at(0).at(0));
					return summedTempMatrix;
				} else {
					PALE_ENGINE_TRACE("Matrix.h->HadamardMultiplication [183]: Elemental summary of 2 matrices has been successful. Output matrix dimensions: {0}x{1}. First matrix value: {2}.", tempMatrix._rows, tempMatrix._columns, tempMatrix._matrix.at(0).at(0));
					return tempMatrix;
				}
			} catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
			}
		}
		inline double AccumulateMatrixValues() const {
			double accumulatedValues = 0;
			for (int rowIterator = 0; rowIterator < _rows; rowIterator++) {
				for (int columnIterator = 0; columnIterator < _columns; columnIterator++) {
					accumulatedValues += _matrix.at(rowIterator).at(columnIterator);
				}
			}

			PALE_ENGINE_INFO("Matrix.h->AccumulateMatrixValues() [265]: Values of the matrix has been accumulated! Output value: {0}.", accumulatedValues);
			return accumulatedValues;
		}
		inline static Matrix CrossCorrelationOperation(const Matrix& input, const Matrix& kernel, unsigned short int stride = 1) {
			try {
				unsigned short int outputYDimension = floor((input._rows - kernel._rows) / stride) + 1;
				unsigned short int outputXDimension = floor((input._columns - kernel._columns) / stride) + 1;
				std::vector<double> tempOutputValues;

				for (int inputRowsIterator = 0; inputRowsIterator < input._rows; inputRowsIterator += stride) {
					for (int inputColumnsIterator = 0; inputColumnsIterator < input._columns; inputColumnsIterator += stride) {
						if (inputRowsIterator + kernel._rows <= input._rows && inputColumnsIterator + kernel._columns <= input._columns) {
							double tempSum = 0;
							for (int kernelRowsIterator = 0; kernelRowsIterator < kernel._rows; kernelRowsIterator++) {
								for (int kernelColumnsIterator = 0; kernelColumnsIterator < kernel._columns; kernelColumnsIterator++) {
									tempSum += input.GetValue(inputRowsIterator + kernelRowsIterator, inputColumnsIterator + kernelColumnsIterator) * kernel.GetValue(kernelRowsIterator, kernelColumnsIterator);
								}
							}
							tempOutputValues.emplace_back(tempSum);
						}
					}
				}

				PALE_ENGINE_INFO("Matrix.h->ConvolutionOperation() [306]: Successfully performed convolution operation! Output matrix dimension: {0}x{1}. First matrix value: {2}.", outputYDimension, outputXDimension, tempOutputValues.at(0));

				return Matrix(outputYDimension, outputXDimension, tempOutputValues);
			} catch (PaleEngineException& exception) {
				if (exception.GetType() == 'e')
					PALE_ENGINE_ERROR("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo())
				else if (exception.GetType() == 'w')
					PALE_ENGINE_WARN("{0}->{1} [{2}]: {3}", exception.GetFile(), exception.GetFunction(), exception.GetLine(), exception.GetInfo());
			}
		}
		inline static std::pair<Matrix, Matrix> MaxPoolingOperation(const Matrix& input, unsigned short int windowSize = 2, unsigned short int stride = 2) {
			if ((windowSize % 2 == 0 && (input._rows % 2 != 0 || input._columns % 2 != 0)) || (windowSize % 2 != 0 && (input._rows % 2 == 0 || input._columns % 2 == 0)))
				PALE_ENGINE_WARN("Matrix.h->MaxPoolingOperation() [327]: {0}", POSSIBLE_DATA_LOSS);

			unsigned short int outputYDimension = floor((input._rows - windowSize) / stride) + 1;
			unsigned short int outputXDimension = floor((input._columns - windowSize) / stride) + 1;

			Math::Matrix tempInputMask(input._rows, input._columns, 0.f);
			std::vector<double> tempOutputValues;
			for (int inputRowsIterator = 0; inputRowsIterator < input._rows; inputRowsIterator += stride) {
				for (int inputColumnsIterator = 0; inputColumnsIterator < input._columns; inputColumnsIterator += stride) {
					if (inputRowsIterator + windowSize <= input._rows && inputColumnsIterator + windowSize <= input._columns) {
						double tempMaxValue = input.GetValue(inputRowsIterator, inputColumnsIterator);
						std::pair<unsigned short int, unsigned short int> tempMaxValueCoordinates = { inputRowsIterator, inputColumnsIterator };

						for (int windowRowIterator = 0; windowRowIterator < windowSize; windowRowIterator++) {
							for (int windowColumnIterator = 0; windowColumnIterator < windowSize; windowColumnIterator++) {

								if (input.GetValue(inputRowsIterator + windowRowIterator, inputColumnsIterator + windowColumnIterator) > tempMaxValue) {
									tempMaxValue = input.GetValue(inputRowsIterator + windowRowIterator, inputColumnsIterator + windowColumnIterator);
									tempMaxValueCoordinates = { inputRowsIterator + windowRowIterator, inputColumnsIterator + windowColumnIterator };
								}
							}
						}

						tempOutputValues.emplace_back(tempMaxValue);
						tempInputMask._matrix.at(tempMaxValueCoordinates.first).at(tempMaxValueCoordinates.second) = 1;
					}
				}
			}

			PALE_ENGINE_INFO("Matrix.h->MaxPoolingOperation() [348]: Successfully performed max pooling operation! Output matrix dimension: {0}x{1}. First matrix value: {2}.", outputYDimension, outputXDimension, tempOutputValues.at(0));

			return std::make_pair(Matrix(outputYDimension, outputXDimension, tempOutputValues), tempInputMask);
		}
		inline std::vector<double> ConvertToVector() const {
			std::vector<double> matrixValuesVector;
			for (int matrixRowIterator = 0; matrixRowIterator < _rows; matrixRowIterator++) {
				for (int matrixColumnsIterator = 0; matrixColumnsIterator < _columns; matrixColumnsIterator++) {
					matrixValuesVector.emplace_back(_matrix.at(matrixRowIterator).at(matrixColumnsIterator));
				}
			}

			PALE_ENGINE_INFO("Matrix.h->ConvertToVector() [360]: Matrix has been successfully converted into vector! Output vector size: {0}. First vector value: {1}.", matrixValuesVector.size(), matrixValuesVector.at(0));

			return matrixValuesVector;
		}

	private:
		unsigned short int _rows, _columns;
		std::vector<std::vector<double>> _matrix;
	};

	inline std::ostream& operator<<(std::ostream& os, const Matrix& data) {
		return os << data.ToString();
	}
} // namespace Pale::Math