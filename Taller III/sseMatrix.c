#include <emmintrin.h>
#include <smmintrin.h>

#include <stdio.h>

int main()
{

	float matrix[4][4];

	float vector[4];

	for (int i = 0; i < 4; ++i)
	{
		printf("\nV[%d]:\n", i);
		scanf("%f", &vector[i]);
		for (int j = 0; j < 4; ++j)
		{
			printf("\nM[%d][%d]:\n", i, j);
			scanf("%f", &matrix[i][j]);
		}
	}

	//Matrix printing
	printf("\n\n\nM =\n");
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			printf("%f\t", matrix[i][j]);
		}
		printf("\n");
	}

	printf("\n\n\nV =\n");
	for (int i = 0; i < 4; ++i)
	{
		printf("%f\t ", vector[i]);
	}
	printf("\n");

	float vector_r[4];

	__m128 vectorV = _mm_set_ps(vector[3],vector[2],vector[1],vector[0]);

	__m128 vectorM0 = _mm_set_ps(matrix[3][0],matrix[2][0],matrix[1][0],matrix[0][0]);
	__m128 vectorM1 = _mm_set_ps(matrix[3][1],matrix[2][1],matrix[1][1],matrix[0][1]);
	__m128 vectorM2 = _mm_set_ps(matrix[3][2],matrix[2][2],matrix[1][2],matrix[0][2]);
	__m128 vectorM3 = _mm_set_ps(matrix[3][3],matrix[2][3],matrix[1][3],matrix[0][3]);
	
	
	__m128 r1 = _mm_dp_ps(vectorV,vectorM0,0xFF);
	__m128 r2 = _mm_dp_ps(vectorV,vectorM1,0xFF);
	__m128 r3 = _mm_dp_ps(vectorV,vectorM2,0xFF);
	__m128 r4 = _mm_dp_ps(vectorV,vectorM3,0xFF);

	 vector_r[0] = _mm_cvtss_f32(r1);
	 vector_r[1] = _mm_cvtss_f32(r2);
	 vector_r[2] = _mm_cvtss_f32(r3);
	 vector_r[3] = _mm_cvtss_f32(r4);

	printf("\n\n\nV result:\n");

		for (int i = 0; i < 4; ++i)
		{
			printf("%f\t", vector_r[i]);
		}
		printf("\n");

	return 0;
}