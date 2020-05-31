struct response{
	int arr[512];
};

struct request{
	int arr1[512];
	int arr2[512];
};

program ARRAYPROG{
	version ARRAYPROGVERS{
		response ADD_ARR(request)=1;
	}=1;
}=0x21212121;
