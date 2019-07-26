#include	<stdio.h>
#include	"utility.h"

TCHAR	currentdirectory[MAX_PATH + 1];

const float PI = 3.1415926535897932384626433832793f;


void ChangeAssetDirectory(TCHAR* assetdirectory) {

	GetCurrentDirectory(sizeof(currentdirectory), currentdirectory);
	printf("%s \n", currentdirectory);
	BOOL sts = SetCurrentDirectory(assetdirectory);
	if (sts == 0) {
		DWORD code = GetLastError();
		printf("ÉJÉåÉìÉgÉfÉBÉåÉNÉgÉäê›íËé∏îs\n %d", code);
	}
	TCHAR	tempdir[MAX_PATH + 1];
	GetCurrentDirectory(sizeof(tempdir), tempdir);
	printf("%s \n", tempdir);
}

void RestoreDirectory() {

	SetCurrentDirectory(currentdirectory);

}

void MatRotateZ(Matrix4x4& mat, float angle) {
	float	cos;
	float	sin;

	cos = cosf((angle*PI) / 180.0f);
	sin = sinf((angle*PI) / 180.0f);

	mat.m2[0][0] = cos;
	mat.m2[0][1] = sin;
	mat.m2[0][2] = 0.0f;
	mat.m2[0][3] = 0.0f;

	mat.m2[1][0] = -sin;
	mat.m2[1][1] = cos;
	mat.m2[1][2] = 0.0f;
	mat.m2[1][3] = 0.0f;

	mat.m2[2][0] = 0.0f;
	mat.m2[2][1] = 0.0f;
	mat.m2[2][2] = 1.0f;
	mat.m2[2][3] = 0.0f;

	mat.m2[3][0] = 0.0f;
	mat.m2[3][1] = 0.0f;
	mat.m2[3][2] = 0.0f;
	mat.m2[3][3] = 1.0f;
}

void MatRotateY(Matrix4x4& mat, float angle) {
	float	cos;
	float	sin;

	cos = cosf((angle*PI) / 180.0f);
	sin = sinf((angle*PI) / 180.0f);

	mat.m2[0][0] = cos;
	mat.m2[0][1] = 0.0f;
	mat.m2[0][2] = sin;
	mat.m2[0][3] = 0.0f;

	mat.m2[1][0] = 0.0f;
	mat.m2[1][1] = 1.0f;
	mat.m2[1][2] = 0.0f;
	mat.m2[1][3] = 0.0f;

	mat.m2[2][0] = -sin;
	mat.m2[2][1] = 0.0f;
	mat.m2[2][2] = cos;
	mat.m2[2][3] = 0.0f;

	mat.m2[3][0] = 0.0f;
	mat.m2[3][1] = 0.0f;
	mat.m2[3][2] = 0.0f;
	mat.m2[3][3] = 1.0f;
}

void MulVectorMatrix(Vector3& vec, const Matrix4x4& mat) {

	Vector3 tmp;
	float w;

	tmp.x = vec.x * mat.m2[0][0] +
		vec.y * mat.m2[1][0] +
		vec.z * mat.m2[2][0] +
		1 * mat.m2[3][0];

	tmp.y = vec.x * mat.m2[0][1] +
		vec.y * mat.m2[1][1] +
		vec.z * mat.m2[2][1] +
		1 * mat.m2[3][1];

	tmp.z = vec.x * mat.m2[0][2] +
		vec.y * mat.m2[1][2] +
		vec.z * mat.m2[2][2] +
		1 * mat.m2[3][2];

	w = vec.x * mat.m2[0][3] +
		vec.y * mat.m2[1][3] +
		vec.z * mat.m2[2][3] +
		1 * mat.m2[3][3];

	vec = tmp;

}


void MulVectorMatrix(const Vector3& vec, const Matrix4x4& mat, Vector3& ans) {

	Vector3 tmp;
	float w;

	tmp.x = vec.x * mat.m2[0][0] +
		vec.y * mat.m2[1][0] +
		vec.z * mat.m2[2][0] +
		1 * mat.m2[3][0];

	tmp.y = vec.x * mat.m2[0][1] +
		vec.y * mat.m2[1][1] +
		vec.z * mat.m2[2][1] +
		1 * mat.m2[3][1];

	tmp.z = vec.x * mat.m2[0][2] +
		vec.y * mat.m2[1][2] +
		vec.z * mat.m2[2][2] +
		1 * mat.m2[3][2];

	w = vec.x * mat.m2[0][3] +
		vec.y * mat.m2[1][3] +
		vec.z * mat.m2[2][3] +
		1 * mat.m2[3][3];

	ans = tmp;

}



//-------------------------------------------------
//	äTóvÅF	ÇSçsÇSóÒÇÃçsóÒÇÃä|éZÇçsÇ§
//	ì¸óÕÅF	mat1	çsóÒÇP
//			mat2	çsóÒÇQ
//			ans		çsóÒÇPÅ~çsóÒÇQ
//-------------------------------------------------
void MulMatrixMatrix(const Matrix4x4& mat1, const Matrix4x4& mat2, Matrix4x4& ans)
{
	Matrix4x4 temp;

	temp.m2[0][0] = mat1.m2[0][0] * mat2.m2[0][0] +
		mat1.m2[0][1] * mat2.m2[1][0] +
		mat1.m2[0][2] * mat2.m2[2][0] +
		mat1.m2[0][3] * mat2.m2[3][0];

	temp.m2[0][1] = mat1.m2[0][0] * mat2.m2[0][1] +
		mat1.m2[0][1] * mat2.m2[1][1] +
		mat1.m2[0][2] * mat2.m2[2][1] +
		mat1.m2[0][3] * mat2.m2[3][1];

	temp.m2[0][2] = mat1.m2[0][0] * mat2.m2[0][2] +
		mat1.m2[0][1] * mat2.m2[1][2] +
		mat1.m2[0][2] * mat2.m2[2][2] +
		mat1.m2[0][3] * mat2.m2[3][2];

	temp.m2[0][3] = mat1.m2[0][0] * mat2.m2[0][3] +
		mat1.m2[0][1] * mat2.m2[1][3] +
		mat1.m2[0][2] * mat2.m2[2][3] +
		mat1.m2[0][3] * mat2.m2[3][3];


	temp.m2[1][0] = mat1.m2[1][0] * mat2.m2[0][0] +
		mat1.m2[1][1] * mat2.m2[1][0] +
		mat1.m2[1][2] * mat2.m2[2][0] +
		mat1.m2[1][3] * mat2.m2[3][0];

	temp.m2[1][1] = mat1.m2[1][0] * mat2.m2[0][1] +
		mat1.m2[1][1] * mat2.m2[1][1] +
		mat1.m2[1][2] * mat2.m2[2][1] +
		mat1.m2[1][3] * mat2.m2[3][1];

	temp.m2[1][2] = mat1.m2[1][0] * mat2.m2[0][2] +
		mat1.m2[1][1] * mat2.m2[1][2] +
		mat1.m2[1][2] * mat2.m2[2][2] +
		mat1.m2[1][3] * mat2.m2[3][2];

	temp.m2[1][3] = mat1.m2[1][0] * mat2.m2[0][3] +
		mat1.m2[1][1] * mat2.m2[1][3] +
		mat1.m2[1][2] * mat2.m2[2][3] +
		mat1.m2[1][3] * mat2.m2[3][3];

	temp.m2[2][0] = mat1.m2[2][0] * mat2.m2[0][0] +
		mat1.m2[2][1] * mat2.m2[1][0] +
		mat1.m2[2][2] * mat2.m2[2][0] +
		mat1.m2[2][3] * mat2.m2[3][0];

	temp.m2[2][1] = mat1.m2[2][0] * mat2.m2[0][1] +
		mat1.m2[2][1] * mat2.m2[1][1] +
		mat1.m2[2][2] * mat2.m2[2][1] +
		mat1.m2[2][3] * mat2.m2[3][1];

	temp.m2[2][2] = mat1.m2[2][0] * mat2.m2[0][2] +
		mat1.m2[2][1] * mat2.m2[1][2] +
		mat1.m2[2][2] * mat2.m2[2][2] +
		mat1.m2[2][3] * mat2.m2[3][2];

	temp.m2[2][3] = mat1.m2[2][0] * mat2.m2[0][3] +
		mat1.m2[2][1] * mat2.m2[1][3] +
		mat1.m2[2][2] * mat2.m2[2][3] +
		mat1.m2[2][3] * mat2.m2[3][3];

	temp.m2[3][0] = mat1.m2[3][0] * mat2.m2[0][0] +
		mat1.m2[3][1] * mat2.m2[1][0] +
		mat1.m2[3][2] * mat2.m2[2][0] +
		mat1.m2[3][3] * mat2.m2[3][0];

	temp.m2[3][1] = mat1.m2[3][0] * mat2.m2[0][1] +
		mat1.m2[3][1] * mat2.m2[1][1] +
		mat1.m2[3][2] * mat2.m2[2][1] +
		mat1.m2[3][3] * mat2.m2[3][1];

	temp.m2[3][2] = mat1.m2[3][0] * mat2.m2[0][2] +
		mat1.m2[3][1] * mat2.m2[1][2] +
		mat1.m2[3][2] * mat2.m2[2][2] +
		mat1.m2[3][3] * mat2.m2[3][2];

	temp.m2[3][3] = mat1.m2[3][0] * mat2.m2[0][3] +
		mat1.m2[3][1] * mat2.m2[1][3] +
		mat1.m2[3][2] * mat2.m2[2][3] +
		mat1.m2[3][3] * mat2.m2[3][3];

	ans = temp;

}