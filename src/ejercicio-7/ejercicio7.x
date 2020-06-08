
struct tiempo{
	long sec;
	long usec;
};

program EJERCICIO7
{
	version VERSION1
	{
		tiempo reloj(void) = 1;
	}=1;
} = 0x20000100;
