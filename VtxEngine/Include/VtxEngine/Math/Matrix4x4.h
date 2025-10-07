#pragma once
#include <memory>
#include "Vector3D.h"
#include "Vector4D.h"

namespace Vtx {
	class Matrix4x4
	{
	public:
		Matrix4x4() 
		{
			m_mat4[0][0] = 1;
			m_mat4[1][1] = 1;
			m_mat4[2][2] = 1;
			m_mat4[3][3] = 1;
		}

		void SetIdentity() {
			// resets matrix to oringial matrix to preform operations

			::memset(m_mat4, 0, sizeof(float) * 16); // resets the 4x4 matrix to all zeros
			m_mat4[0][0] = 1;
			m_mat4[1][1] = 1;
			m_mat4[2][2] = 1;
			m_mat4[3][3] = 1;
		}

		void SetTranslation(const Vector3D& vector)
		{
			// translate matrix
			m_mat4[3][0] = vector.m_x;
			m_mat4[3][1] = vector.m_y;
			m_mat4[3][2] = vector.m_z;
		}

		void SetMatrix(const Matrix4x4& matrix)
		{
			::memcpy(m_mat4, matrix.m_mat4, sizeof(float) * 16);
		}

		Vector3D GetZDirection()
		{
			return Vector3D(m_mat4[2][0], m_mat4[2][1], m_mat4[2][2]);
		}

		Vector3D GetXDirection()
		{
			return Vector3D(m_mat4[0][0], m_mat4[0][1], m_mat4[0][2]);
		}

		Vector3D GetYDirection() 
		{
			return Vector3D(m_mat4[1][0], m_mat4[1][1], m_mat4[1][2]);
		}

		Vector3D GetTranslation()
		{
			return Vector3D(m_mat4[3][0], m_mat4[3][1], m_mat4[3][2]);
		}

		void SetPerspectiveFOV(float fov, float aspect, float znear, float zfar)
		{
			float yscale = 1.0f / tanf(fov / 2.0f);
			float xscale = yscale / aspect;

			m_mat4[0][0] = xscale;
			m_mat4[1][1] = yscale;
			m_mat4[2][2] = zfar / (zfar - znear);
			m_mat4[2][3] = 1.0f;
			m_mat4[3][2] = (-znear * zfar) / (zfar - znear);
			m_mat4[3][3] = 0.0f;
		}

		void SetOrtho(float width, float height, float nearPlane,  float farPlane)
		{
			
			SetIdentity();
			m_mat4[0][0] = 2.0f / width;
			m_mat4[1][1] = 2.0f / height;
			m_mat4[2][2] = 1.0f / (farPlane - nearPlane);
			m_mat4[3][2] = -(nearPlane / (farPlane - nearPlane));
		}

		void SetScale(const Vector3D& scale)
		{
			// scales the matrix based off the inputs given
			m_mat4[0][0] = scale.m_x;
			m_mat4[1][1] = scale.m_y;
			m_mat4[2][2] = scale.m_z;
		}

		void SetRotationX(float x)
		{
			m_mat4[1][1] = cosf(x);
			m_mat4[1][2] = sinf(x);
			m_mat4[2][1] = -sinf(x);
			m_mat4[2][2] = cosf(x);
		}

		void SetRotationY(float y)
		{
			m_mat4[0][0] = cosf(y);
			m_mat4[0][2] = -sinf(y);
			m_mat4[2][0] = sinf(y);
			m_mat4[2][2] = cosf(y);
		}

		void SetRotationZ(float z)
		{
			m_mat4[0][0] = cosf(z);
			m_mat4[0][1] = sinf(z);
			m_mat4[1][0] = -sinf(z);
			m_mat4[1][1] = cosf(z);
		}

		float GetDeterminant()
		{
			Vector4D minor, v1, v2, v3;
			float det;

			v1 = Vector4D(this->m_mat4[0][0], this->m_mat4[1][0], this->m_mat4[2][0], this->m_mat4[3][0]);
			v2 = Vector4D(this->m_mat4[0][1], this->m_mat4[1][1], this->m_mat4[2][1], this->m_mat4[3][1]);
			v3 = Vector4D(this->m_mat4[0][2], this->m_mat4[1][2], this->m_mat4[2][2], this->m_mat4[3][2]);


			minor.cross(v1, v2, v3);
			det = -(this->m_mat4[0][3] * minor.m_x + this->m_mat4[1][3] * minor.m_y + this->m_mat4[2][3] * minor.m_z +
				this->m_mat4[3][3] * minor.m_w);
			return det;
		}

		void Inverse()
		{
			int a, i, j;
			Matrix4x4 out;
			Vector4D v, vec[3];
			float det = 0.0f;

			det = this->GetDeterminant();
			if (!det) return;
			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					if (j != i)
					{
						a = j;
						if (j > i) a = a - 1;
						vec[a].m_x = (this->m_mat4[j][0]);
						vec[a].m_y = (this->m_mat4[j][1]);
						vec[a].m_z = (this->m_mat4[j][2]);
						vec[a].m_w = (this->m_mat4[j][3]);
					}
				}
				v.cross(vec[0], vec[1], vec[2]);

				out.m_mat4[0][i] = powf(-1.0f, (float)i) * v.m_x / det;
				out.m_mat4[1][i] = powf(-1.0f, (float)i) * v.m_y / det;
				out.m_mat4[2][i] = powf(-1.0f, (float)i) * v.m_z / det;
				out.m_mat4[3][i] = powf(-1.0f, (float)i) * v.m_w / det;
			}

			this->SetMatrix(out);
		}


		void operator *=(const Matrix4x4& matrix)
		{
			// multiples 2 matrices together
			Matrix4x4 matrixProduct;

			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 4; ++j) {
					matrixProduct.m_mat4[i][j] = m_mat4[i][0] * matrix.m_mat4[0][j] +
									m_mat4[i][1] * matrix.m_mat4[1][j] +
									m_mat4[i][2] * matrix.m_mat4[2][j] +
									m_mat4[i][3] * matrix.m_mat4[3][j];
				}
			}

			::memcpy(m_mat4, matrixProduct.m_mat4, sizeof(float) * 16);
		}

		~Matrix4x4() {}

	public:
		float m_mat4[4][4] = {};
	};
}
