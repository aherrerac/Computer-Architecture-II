#include <emmintrin.h>
#include <smmintrin.h>

#include <stdio.h>


int main()
{
	long int matrix[4][3];

	//Filling matrix
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			printf("\nM[%d][%d]:\n", i, j);
			scanf("%ld", &matrix[i][j]);
		}
	}

	//Matrix printing
	printf("\n\n\nM =\n");
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			printf("%ld\t", matrix[i][j]);
		}
		printf("\n");
	}


	__m128i vector0 = _mm_set_epi32(matrix[3][2], matrix[3][1], matrix[3][0],0);

	__m128i vector1 = _mm_set_epi32(matrix[2][2], matrix[2][1], matrix[2][0],0);

	__m128i vector2 = _mm_set_epi32(matrix[1][2], matrix[1][1], matrix[1][0],0);

	__m128i vector3 = _mm_set_epi32(matrix[0][2], matrix[0][1], matrix[0][0],0);

	__m128i vector01 = _mm_max_epi32(vector0, vector1);

	__m128i vector23 = _mm_max_epi32(vector2, vector3);

	__m128i result = _mm_max_epi32(vector01, vector23);

	printf("\n");
	printf("Result *********************** \n");

	int data = _mm_extract_epi32(result, 1);
	printf("%d \t", data);

	data = _mm_extract_epi32(result, 2);
	printf("%d \t", data);

	data = _mm_extract_epi32(result, 3);
	printf("%d \t", data);

	printf("\n");
	return 1;
}