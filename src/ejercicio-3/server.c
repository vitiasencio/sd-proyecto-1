#include "array.h"

response *add_arr_1_svc(request *argp, struct svc_req *rqstp){

	static response  result;

	int i;

	/* Obtengo la suma de los dos arreglos. */
	for(i=0; i<512;i++){
		result.arr[i] = argp->arr1[i] + argp->arr2[i];
	}

	return &result;

}
