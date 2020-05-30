#include <stdlib.h>
#include <stdio.h>

#define N 10000

int main(){

	/* Iteraciones */
	int numA = 10,
		numB = 5.4,
		i;

	for(i=0; i<N; i++){
		numA+=numB;
		numB+=numA*0.5;
	}
	
	/* Escribir en un archivo */
	
	FILE * fp;
	
	fp = fopen("output.txt","w");
 
   	fprintf (fp, "Texto en un archivo ");
   	
   	/* close the file*/  
   	fclose (fp);

	exit(0);
}
