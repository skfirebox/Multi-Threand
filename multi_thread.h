#include <pthread.h>
#include <immintrin.h>
struct parameters{
	int start;
	int end;
	int N;
	int *matA;
	int *matB;
	int *output;
};
// Create other necessary functions here
void *singleThread(void* params)
{
    struct parameters *p = (struct parameters*)params;
	int start = p->start;
	int end = p->end;
	int N = p->N;
	int *matA = p->matA;
	int *matB = p->matB;
	int *output = p->output;
 // transposematrix(int matB,int N);
 // assert( N>=4 and N == ( N &~ (N-1)));
  for(int rowA = start; rowA <= end; rowA +=2) {
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
  return NULL;
}


// Fill in this function
void multiThread(int N, int *matA, int *matB, int *output)
{
    int num_threads = 8;
    
		pthread_t threads[num_threads];
        int div = N/num_threads;
		struct parameters p[num_threads];
        int s=0;
		for(int i=0;i<num_threads;i++){
			
				p[i].start=s;
				p[i].end = s+div-1;
				p[i].N = N;
				p[i].matA = matA;
				p[i].matB = matB;
				p[i].output = output;
				s+=div;
			
        }
		for(int i=0;i<num_threads;i++){
			pthread_create( &threads[i], NULL, singleThread, (void*) &p[i]);
		}
		
		for(int i=0;i<num_threads;i++){
			pthread_join( threads[i], NULL);
		}  

}
