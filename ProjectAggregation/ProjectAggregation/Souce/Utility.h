#pragma once
#include	<windows.h>
#include	"FbxModel.h"

void ChangeAssetDirectory(TCHAR* assetdirectory);
void RestoreDirectory();


void MatRotateZ(Matrix4x4& mat, float angle);
void MatRotateY(Matrix4x4& mat, float angle);
void MulVectorMatrix(Vector3& vec, const Matrix4x4& mat);
void MulMatrixMatrix(const Matrix4x4& mat1, const Matrix4x4& mat2, Matrix4x4& ans);
void MulVectorMatrix(const Vector3& vec, const Matrix4x4& mat, Vector3& ans);
