import java.io.* ;
import java.net.* ;
import java.util.*;
import java.text.*;
class Servidor {

static final int PUERTO=35557;

  private static String dectobin(int decimal) {
	if (decimal <= 0) {
		return "0";
	}
	//Almacena los digitos en binario
	StringBuilder binario = new StringBuilder();
	while (decimal > 0) {
		short residuo = (short) (decimal % 2);
		decimal = decimal / 2;
		// Insertar el dígito al inicio de la cadena
		binario.insert(0, String.valueOf(residuo));
	}
	return binario.toString();
  }
  
  private static String bintohex(String binario){
	StringBuilder resultado = new StringBuilder();
	for ( int i = 0 ; i < binario.length() ; i +=4 ) {
	  int    numero  = Integer.parseInt( binario.substring( i, i+4) , 2 );
	  String reprHex = Integer.toString( numero, 16 );
	  resultado.append( reprHex );
	}
	return resultado.toString();
  }

  public Servidor( ) {

	   ServerSocket socket_servidor;
	   DataOutputStream flujo_salida;
	   DataInputStream flujo_entrada;
	   InputStream entrada;
	   OutputStream salida;
	   
    try {

	  //crea el socket servidor
	  socket_servidor = new ServerSocket( PUERTO );
	  System.out.println("Escucho el puerto " + PUERTO );
	  boolean ejecutando=true;
	  
	  //Empieza a atender conexiones de clientes
	  Socket socket_cliente = socket_servidor.accept(); 
	  
	  //la clase get input stream sirve para leer datos del cliente
	  entrada = socket_cliente.getInputStream();
	  //crea un input stream mas adecuado a su necesidad
	  flujo_entrada = new DataInputStream(entrada);
	  
	  //la clase get output stream sirve para enviarle datos al cliente.
	  salida = socket_cliente.getOutputStream();
	  //crea un output stream mas adecuado a su necesidad.
	  flujo_salida= new DataOutputStream(salida);
	  do{
		  
		  
	      String resultado=null;
	      //lee cadena del cliente en formato UTF
	      String opt=flujo_entrada.readUTF();
	      int request=Integer.valueOf(opt);
	       
	      switch(request){
		case 1: /*suma de dos operandos*/
			int n1=Integer.valueOf(flujo_entrada.readUTF());
			int n2=Integer.valueOf(flujo_entrada.readUTF());
			int total=n1+n2;
			resultado=String.valueOf(total);
			break;
		
		case 2: /*suma de tres operandos*/
			n1=Integer.valueOf(flujo_entrada.readUTF());
			n2=Integer.valueOf(flujo_entrada.readUTF());
			int n3=Integer.valueOf(flujo_entrada.readUTF());
			total=n1+n2+n3;
			resultado=String.valueOf(total);
			break;
			
		case 3: /*suma de cuatro operandos*/
			n1=Integer.valueOf(flujo_entrada.readUTF());
			n2=Integer.valueOf(flujo_entrada.readUTF());
			n3=Integer.valueOf(flujo_entrada.readUTF());
			int n4=Integer.valueOf(flujo_entrada.readUTF());
			total=n1+n2+n3+n4;
			resultado=String.valueOf(total);
			break;
		case 4: /*resta de dos operandos*/
			n1=Integer.valueOf(flujo_entrada.readUTF());
			n2=Integer.valueOf(flujo_entrada.readUTF());
			total=(n1)-(n2);
			resultado=String.valueOf(total);
			break;
		case 5: /*resta de tres operandos*/
			n1=Integer.valueOf(flujo_entrada.readUTF());
			n2=Integer.valueOf(flujo_entrada.readUTF());
			n3=Integer.valueOf(flujo_entrada.readUTF());
			total=(n1)-(n2)-(n3);
			resultado=String.valueOf(total);
			break;
		 case 6: /*resta de cuatro operandos*/
			n1=Integer.valueOf(flujo_entrada.readUTF());
			n2=Integer.valueOf(flujo_entrada.readUTF());
			n3=Integer.valueOf(flujo_entrada.readUTF());
			n4=Integer.valueOf(flujo_entrada.readUTF());
			total=(n1)-(n2)-(n3)-(n4);
			resultado=String.valueOf(total);
			break;
		 case 7: /*multiplicacion de dos operandos*/
			n1=Integer.valueOf(flujo_entrada.readUTF());
			n2=Integer.valueOf(flujo_entrada.readUTF());
			total=n1*n2;
			resultado=String.valueOf(total);
			break;
		 case 8: /*division de dos operandos*/
			n1=Integer.valueOf(flujo_entrada.readUTF());
			n2=Integer.valueOf(flujo_entrada.readUTF());
			total=n1/n2;
			resultado=String.valueOf(total);
			break;
		 case 9: /*Conversion decimal a binario*/
			n1=Integer.valueOf(flujo_entrada.readUTF());
			String binario=dectobin(n1);
			
			resultado=String.valueOf(binario);
			break;
		 case 10: /*Conversion binario a hexadecimal*/
			String n=flujo_entrada.readUTF();
			String hexa=bintohex(n);
			
			resultado=String.valueOf(hexa);
			break;
		 case 11:
			ejecutando=false;
			socket_cliente.close();
			break;

		 default:
			System.out.println("Operacion inexistente");
			break;
	      }
		  
	      if(resultado!=null)			
		      flujo_salida.writeUTF(resultado);
		  
	  } while(ejecutando==true);
	      socket_cliente.close();
		
    } catch( Exception e ) 
      {
	  System.out.println( e.getMessage() );
      }

    }


    public static void main( String[] arg ) {

	    new Servidor();

    }

  }
