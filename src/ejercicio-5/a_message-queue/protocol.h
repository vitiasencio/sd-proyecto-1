#define TAMCOLA sizeof(struct mensaje)
#define TAMCOLARES sizeof(struct mensaje_res)

#define PATHREQ "/tmp"
#define IDREQ 'S'

#define PATHRES "/usr"
#define IDRES 'T'

#define MILLION 1000000L

struct mensaje {
	int op_type;
	int cant_operands;
	long decbin;
	float operands[4];
};

struct mensaje_res{
	int op_type;
	int is_neg;
	float float_res;
	char conv_res[30];
};


typedef struct mensaje my_msgbuf;
typedef struct mensaje_res my_msgbuf_res;
