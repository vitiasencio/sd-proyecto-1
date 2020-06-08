#define TAMCOLA sizeof(struct mensaje)
#define TAMCOLARES sizeof(struct mensaje_res)

#define PATHREQ "/tmp"
#define IDREQ 'S'

#define PATHRES "/usr"
#define IDRES 'T'

struct mensaje {
	int op_type;
	int cant_operands;
	int operands[4];
};

struct mensaje_res{
	int op_type;
	int number_res;
	char hexa_res[20];
};


typedef struct mensaje my_msgbuf;
typedef struct mensaje_res my_msgbuf_res;
