#pragma once

#include <vector>
#include <cmath>
#include <stdexcept>

#include "Vector.h"

namespace math {

	/**********************************************
	class Mat4
	@brief Class used to represent 4x4 matrices
	************************************************/
	template<typename T>
	class Mat4
	{
	public:

		// default matrix is identity
		T mat[4][4] = { {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };

		Mat4() {};

		// work around since you can't assign arrays after intialize
		// seems beter than using std::array
		Mat4(T m00, T m01, T m02, T m03,
			T m10, T m11, T m12, T m13,
			T m20, T m21, T m22, T m23,
			T m30, T m31, T m32, T m33)
		{
			mat[0][0] = m00; mat[0][1] = m01; mat[0][2] = m02; mat[0][3] = m03;
			mat[1][0] = m10; mat[1][1] = m11; mat[1][2] = m12; mat[1][3] = m13;
			mat[2][0] = m20; mat[2][1] = m21; mat[2][2] = m22; mat[2][3] = m23;
			mat[3][0] = m30; mat[3][1] = m31; mat[3][2] = m32; mat[3][3] = m33;
		};

		// this constructor just makes setting up the tests easier
		Mat4(std::vector< std::vector<T> > v)
		{
			if (v.size() != 4 || v[0].size() != 4)
				fprintf(stderr, "Passed in vector of wrong size! Must be  4x4.");
			else
			{
				for (int row = 0; row < 4; row++)
					for (int column = 0; column < 4; column++)
						mat[row][column] = v[row][column];
			}
		}

		void set(
			T m00, T m01, T m02, T m03,
			T m10, T m11, T m12, T m13,
			T m20, T m21, T m22, T m23,
			T m30, T m31, T m32, T m33)
		{
			mat[0][0] = m00; mat[0][1] = m01; mat[0][2] = m02; mat[0][3] = m03;
			mat[1][0] = m10; mat[1][1] = m11; mat[1][2] = m12; mat[1][3] = m13;
			mat[2][0] = m20; mat[2][1] = m21; mat[2][2] = m22; mat[2][3] = m23;
			mat[3][0] = m30; mat[3][1] = m31; mat[3][2] = m32; mat[3][3] = m33;
		};

		// swap rows are columns of the matrix
		Mat4<T> transpose()
		{
			Matrix4x4<T> result;

			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					result.mat[i][j] = mat[j][i];

			return result;
		}

		// computes the inverse of the matrix using the Gauss-Jordan elimination method
		Mat4<T> inverse()
		{
			// a copy of the original matrix is needed to perform the neccessary operations on
			Mat4 orig(*this);

			// an identity matrix is needed to keep track of all the operations done on the original matrix
			// this ends up becoming the inverse
			Mat4 inv;

			// loop through matrix to set all elements to 0 except for the pivot points
			for (int i = 0; i < 4; i++)
			{
				// pivot points are the elements on the diagonal
				int pivot = orig.mat[i][i];

				// if a pivot is 0 it must be swapped with another row that is a value other than 0
				if (pivot == 0)
				{
					// current best choice for row to swap with
					int bestOption = i;

					// check all elements in the column for the largest absolute value
					for (int j = 0; j < 4; j++)
					{
						if (std::abs(orig.mat[j][i]) > std::abs(orig.mat[bestOption][i]))
						{
							bestOption = j;
						}
					}

					// if all the elements in the column are 0 then this matrix is Singular
					if (orig.mat[i][bestOption] == 0)
					{
						fprintf(stderr, "Matrix has no inverse!\n");
					}
					else // otherwise swap the current row the best option row
					{
						T temp;

						for (int j = 0; j < 4; j++)
						{
							temp = orig.mat[i][j];
							orig.mat[i][j] = orig.mat[bestOption][j];
							orig.mat[bestOption][j] = temp;

							temp = inv.mat[i][j];
							inv.mat[i][j] = inv.mat[bestOption][j];
							inv.mat[bestOption][j] = temp;
						}
					}
				}

				// set rest of elements in column to 0
				// this is done by calculating a coefficient q based on the element that we want to set to 0 over the pivot
				// then subtracting the desired row by q * the current row
				for (int j = 0; j < 4; j++)
				{
					if (i != j)
					{
						T q = orig.mat[j][i] / orig.mat[i][i];

						if (q != 0)
						{
							for (int k = 0; k < 4; k++)
							{
								orig.mat[j][k] -= q * orig.mat[i][k];
								inv.mat[j][k] -= q * inv.mat[i][k];
							}
						}

					}
				}
			}

			// now set all pivots to one to get the identity matrix
			for (int i = 0; i < 4; i++)
			{

				if (orig.mat[i][i] != 1)
				{
					for (int j = 0; j < 4; j++)
					{
						inv.mat[i][j] /= orig.mat[i][i];
					}
				}
			}


			return inv;
		}

		// add two matrices together
		Mat4 operator + (Mat4 const& obj)
		{
			Mat4 res;

			for (int row = 0; row < 4; row++)
				for (int column = 0; column < 4; column++)
					res.mat[row][column] = mat[row][column] + obj.mat[row][column];

			return res;
		}

		// subtract matrix from another
		Mat4 operator - (Mat4 const& obj)
		{
			Mat4 res;

			for (int row = 0; row < 4; row++)
				for (int column = 0; column < 4; column++)
					res.mat[row][column] = mat[row][column] - obj.mat[row][column];

			return res;
		};

		void matmulVec(Vec3<T>& vec, Vec3<T>& newVec)
		{
			float w;

			newVec.x = vec.x * mat[0][0] + vec.y * mat[1][0] + vec.z * mat[2][0] + mat[3][0];
			newVec.y = vec.x * mat[0][1] + vec.y * mat[1][1] + vec.z * mat[2][1] + mat[3][1];
			newVec.z = vec.x * mat[0][2] + vec.y * mat[1][2] + vec.z * mat[2][2] + mat[3][2];

			w = vec.x * mat[0][3] + vec.y * mat[1][3] + vec.z * mat[2][3] + mat[3][3];

			if (w != 0)
			{
				newVec.x /= w;
				newVec.y /= w;
				newVec.z /= w;
			}
		}

		// take the dot product of two matrices
		void matmulMat(Mat4<T>& mat1, Mat4<T>& resmat)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					resmat.mat[i][j] = mat[i][0] * mat1.mat[0][j] +
						mat[i][1] * mat1.mat[1][j] +
						mat[i][2] * mat1.mat[2][j] +
						mat[i][3] * mat1.mat[3][j];
				}
			}
		}

		// allows value to be accessed easier
		float operator () (int i, int j) { return mat[i][j]; };

	};

}