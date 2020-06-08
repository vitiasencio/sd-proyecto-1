import java.io.*;
import java.net.*;
import java.util.concurrent.TimeUnit;


class Cliente {
  private static int controlar(int n){
	  int digito = 0;
	  int resto = n;
	  int esbinario = 1;
	  for(int i=0; i<(Integer.toString(n).length())&&esbinario==1; i++){
	      digito = resto%10;
	      resto = resto/10;
	      if(digito!=1 && digito!=0){
		esbinario = 0;
	      }
	  }
	  return esbinario;
  }

  static final int PUERTO=35557;
  
  public Cliente(String host ) {

	   Socket socket_cliente;
	   OutputStream salida;
	   DataOutputStream flujo_salida;
	   DataInputStream flujo_entrada;
	   InputStream entrada;
      try{

	  socket_cliente = new Socket( host, PUERTO );
	  boolean ejecutando=true;
	  InputStreamReader isr = new InputStreamReader(System.in);
	  BufferedReader br = new BufferedReader (isr);
	  salida = socket_cliente.getOutputStream();
	  flujo_salida= new DataOutputStream(salida);
	  entrada = socket_cliente.getInputStream();
	  flujo_entrada  = new DataInputStream(entrada);
	  System.out.println("Buen Dia! Bienvenido a MINI- OPERACIONES");
	  do 
	  {
	     
	      System.out.println("Elija la operacion que desee realizar");
	      System.out.println("1- Suma de dos operandos");
	      System.out.println("2- Suma de tres operandos");
	      System.out.println("3- Suma de cuatro operandos");
	      System.out.println("4- Resta de dos operandos");
	      System.out.println("5- Resta de tres operandos");
	      System.out.println("6- Resta de cuatro operandos");
	      System.out.println("7- Multiplicacion de dos operandos");
	      System.out.println("8- Division de dos operandos");
	      System.out.println("9- Pasar de binario a decimal");
	      System.out.println("10- Pasar de binario a hexadecimal");
	      System.out.println("11- Salir");
	    
	      String opcion = br.readLine();
	      int request= Integer.valueOf(opcion);
	      
	      String n1=null;
	      String n2=null;
	      String n3=null;
	      String n4=null;
	      
	      switch(request){
		case 1: /*suma dos operandos*/
			System.out.print("Ingrese el primer numero: ");
			n1 = br.readLine();
			System.out.print("Ingrese el segundo numero: ");
			n2 = br.readLine();
			break;
		case 2: /*suma tres operandos*/
			System.out.print("Ingrese el primer numero: ");
			n1 = br.readLine();
			System.out.print("Ingrese el segundo numero: ");
			n2 = br.readLine();
			System.out.print("Ingrese el tercer numero: ");
			n3 = br.readLine();
			break;
		case 3: /*suma cuatro operandos*/
			System.out.print("Ingrese el primer numero: ");
			n1 = br.readLine();
			System.out.print("Ingrese el segundo numero: ");
			n2 = br.readLine();
			System.out.print("Ingrese el tercer numero: ");
			n3 = br.readLine();
			System.out.print("Ingrese el cuarto numero: ");
			n4 = br.readLine();
			break;
		case 4: /*resta dos operandos*/
			System.out.print("Ingrese el primer numero: ");
			n1 = br.readLine();
			System.out.print("Ingrese el segundo numero: ");
			n2 = br.readLine();    		  
			break;
		case 5: /*resta de tres operandos*/
			System.out.print("Ingrese el primer numero: ");
			n1 = br.readLine();
			System.out.print("Ingrese el segundo numero: ");
			n2 = br.readLine();
			System.out.print("Ingrese el tercer numero: ");
			n3 = br.readLine();
			break;
		case 6: /*resta de cuatro operandos*/
			System.out.print("Ingrese el primer numero: ");
			n1 = br.readLine();
			System.out.print("Ingrese el segundo numero: ");
			n2 = br.readLine();
			System.out.print("Ingrese el tercer numero: ");
			n3 = br.readLine();
			System.out.print("Ingrese el cuarto numero: ");
			n4 = br.readLine();
			break;
		case 7: /*multiplicacion de dos operandos*/
			System.out.print("Ingrese el primer numero: ");
			n1 = br.readLine();
			System.out.print("Ingrese el segundo numero: ");
			n2 = br.readLine();    		  
			break;
		case 8: /*division de dos operandos*/
			System.out.print("Ingrese el primer numero: ");
			n1 = br.readLine();
			System.out.print("Ingrese el segundo numero: ");
			n2 = br.readLine();    		  
			while(n2.equals("0")){
			    System.out.print("Ingrese un numero distito de cero para dividir: ");
			    n2 = br.readLine();    		  
			}
			break;
		case 9: /*conversion decimal a binario*/
			System.out.print("Ingrese un numero en decimal: ");
			n1 = br.readLine();
			break;
		case 10: /*conversion binario a hexadecimal*/
			System.out.println("Ingrese un numero en binario, su longitud debe ser multiplo de 4 para convertir");
			System.out.print("Rellene al inicio del numero en caso de ser necesario: ");
			n1 = br.readLine();
			int esbinario = controlar(Integer.parseInt(n1));
			while(Integer.toString(esbinario).equals("0") || n1.length()%4!=0){
			    System.out.print("Ingrese un numero en binario, su longitud debe ser multiplo de 4 para convertir: ");
			    n1 = br.readLine();  
			    esbinario=controlar(Integer.parseInt(n1));
			}
			break;
		case 11:
			ejecutando=false;
			socket_cliente.close();
			break;
		default:
			System.out.println("Operacion inexistente");
			System.out.println();
			break;
		      
	      }
	    

	      long t1 = System.nanoTime();

	      /*Se pasan los datos al servidor*/
	      flujo_salida.writeUTF(opcion);
	      switch(request){
		case 1:
		      flujo_salida.writeUTF(n1);
		      flujo_salida.writeUTF(n2);
		      break;
		case 2:
		      flujo_salida.writeUTF(n1);
		      flujo_salida.writeUTF(n2);	    	  
		      flujo_salida.writeUTF(n3);
		      break;
		case 3:
		      flujo_salida.writeUTF(n1);
		      flujo_salida.writeUTF(n2);	    	  
		      flujo_salida.writeUTF(n3);	    	  
		      flujo_salida.writeUTF(n4);
		      break;	    	  
		case 4:
		      flujo_salida.writeUTF(n1);
		      flujo_salida.writeUTF(n2);	    	                    	    	  
		      break;
		case 5:
		      flujo_salida.writeUTF(n1);
		      flujo_salida.writeUTF(n2);	    	  
		      flujo_salida.writeUTF(n3);
		      break;	
		case 6:
		      flujo_salida.writeUTF(n1);
		      flujo_salida.writeUTF(n2);	    	  
		      flujo_salida.writeUTF(n3);	    	  
		      flujo_salida.writeUTF(n4);	
		      break;
		case 7:
		      flujo_salida.writeUTF(n1);
		      flujo_salida.writeUTF(n2);	
		      break;
		case 8:
		      flujo_salida.writeUTF(n1);
		      flujo_salida.writeUTF(n2);
		      break;	
		case 9:
		      flujo_salida.writeUTF(n1);
		      break;
		case 10:
		      flujo_salida.writeUTF(n1);
		      break;
	      }
	      
	      String resultado=flujo_entrada.readUTF();
	      if(resultado!=null){
		      
		      System.out.println("Resultado : "+resultado);
		      System.out.println();
	      }
	      
	      long t2 = System.nanoTime();
	      System.out.println ("Su operacion fue procesada en: " + (t2-t1)*0.001 + " microsegundos"); 
	      System.out.println();
	  }
	    while(ejecutando==true);
	    socket_cliente.close();
      } catch( Exception e ) 
	{
	      System.out.println( e.getMessage() );
	}
      
  }


  public static void main( String[] arg ) {

    if (arg.length>0)
      new Cliente(arg[0]);
    else
      System.out.println("Error: debe ingresar la IP del servidor");

  }


}
