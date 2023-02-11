// Optimize this function
/**/
#include <immintrin.h>
#include<istream>

/*
static void transposematrix(int *matB,int n)
{
    int *temp=malloc(n*n*sizeof(int));     //need to create a temporary array. 
    memcpy(temp,array,n*n*sizeof(int));
    int i, j;

    for (i = 0; i < n; ++i) 
    {
        for (j = 0; j < n; ++j)
        {
            array[j*n+i]=temp[i*n+j];
        }
    }

    free(temp);
// }  */
void singleThread(int N, int *matA, int *matB, int *output)
{
 // transposematrix(int matB,int N);
  assert( N>=4 and N == ( N &~ (N-1)));
  for(int rowA = 0; rowA < N; rowA +=2) {
    for(int colB = 0; colB < N; colB += 2){
      int sum = 0;
      __m256i res1=_mm256_setzero_si256();

      __m256i res2=_mm256_setzero_si256();

      __m256i res3=_mm256_setzero_si256();

      __m256i res4=_mm256_setzero_si256();
      for(int iter = 0; iter < N; iter=iter+8)

      {
         __m256i w= _mm256_loadu_si256((__m256i*) (matA+rowA*N+iter));

         __m256i x= _mm256_loadu_si256((__m256i*) (matA+(rowA+1)*N+iter));
            
          __m256i y= _mm256_setr_epi32(matB[iter*N+colB], matB[(iter+1)*N+colB], matB[(iter+2)*N+colB], matB[(iter+3)*N+colB], matB[(iter+4)*N+colB], matB[(iter+5)*N+colB], matB[(iter+6)*N+colB], matB[(iter+7)*N+colB]);

         __m256i z= _mm256_setr_epi32(matB[iter*N+colB+1], matB[(iter+1)*N+colB+1], matB[(iter+2)*N+colB+1], matB[(iter+3)*N+colB+1], matB[(iter+4)*N+colB+1], matB[(iter+5)*N+colB+1], matB[(iter+6)*N+colB+1], matB[(iter+7)*N+colB+1]);

         //__m256i y= _mm256_loadu_si256((__m256*) matB+colB*N+iter);

         //__m256i z= _mm256_loadu_si256((__m256*) matB+(colB+1)*N+iter); 


         res1+=_mm256_mullo_epi32( w, y);
         res2+=_mm256_mullo_epi32( w, z);
         res3+=_mm256_mullo_epi32( x, y);
         res4+=_mm256_mullo_epi32( x, z);

         //res1+=res2+res3+res4;
          //a=res1;
      }
      // compute output indices
      int *A=(int*)&res1;
      int *A1=(int*)&res2;
      int *A2=(int*)&res3;
      int *A3=(int*)&res4;
      for(int j=0;j<8;j++)
          sum=sum+A[j];
      for(int j=0;j<8;j++)
          sum=sum+A1[j];
        for(int j=0;j<8;j++)
          sum=sum+A2[j];
          for(int j=0;j<8;j++)
          sum=sum+A3[j];    
      int rowC = rowA>>1;
      int colC = colB>>1;
      int indexC = rowC * (N>>1) + colC;
      output[indexC] = sum;
    }
  }
}
