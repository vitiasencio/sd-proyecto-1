struct aritmetic_req{
	int cant_op;
	float arr[4];
};

struct number_conv{
	char num[50];
};

program MINIOPERACIONES{
	version MINIOPERACIONESVERS{
		float ADD(aritmetic_req)=1;
		float SUB(aritmetic_req)=2;
		float MUL(aritmetic_req)=3;
		float DIV(aritmetic_req)=4;
		number_conv DECTOBIN(long)=5;
		number_conv BINTOHEXA(long)=6;
	}=1;
}=0x21212121;
