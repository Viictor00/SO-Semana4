#include <stdio.h>

int main(int argc, char *argv[]) {
	
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char respuesta[512];
	//INICIALIZACIONES --->SIEMPRE IGUAL
	//Abrimos socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creando socket");
	//Hacemos el bind al puerto
	
	memset(&serv_adr, 0, sizeof(serv_adr));//inicializa a CERO serv_adr
	serv_adr.sin_family = AF_INET;
	
	//asocia el socket a cualquiera de las IP de la máquina
	//htonl formatea el numero que recibe necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	//escucharemos puerto 9050
	serv_adr.sin_port = htons(9070); //SOLO PODEMOS CAMBIAR NUM. PUERTO
	if(bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf("Error en el bind");
	
	if(listen(sock_listen, 3) < 0)
		printf("Error en el Listen");
	
	int i;
	//Atenderemos solo 5 peticiones
	for(;;)
	{
		printf("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf("He recibido la connexión\n");
		//sock_conn es el socket que usaremos para este cliente
		
		//Ahora recibimos su nombre, que dejamos en buff
		ret=read(sock_conn,peticion, sizeof(peticion));
		printf("Recibido\n");
		
		//Tenemos que añadirle la marca de fin de string
		//para que no escriba lo que hay despues del buffer
		peticion[ret]='\0';
		
		//Escribimos el nombre en consola
		printf("Peticion: %s\n", peticion);
		
		//vamos a ver que quieren
		char *p = strtok(peticion, "/");//corta donde hay una barra "1/Juan"
		int codigo = atoi(p);
		p = strtok(NULL, "/");
		char nombre[20];
		strcpy (nombre, p);
		printf("Codigo: %d, Nombre: %s\n", codigo, nombre);
		
		if (codigo == 1) //piden la longitud del nombre
			printf(respuesta, "%d", strlen(nombre));
		else if (codigo == 2)
		{
			//quieren saber si el nombre es bonito
			if((nombre[0]=='M') || (nombre[0]=='S'))
			strcpy (respuesta, "SI");
			else
				strcpy(respuesta, "NO");
		}
		else if (codigo == 3) //decir si es alto
		{
			p = strtok(NULL, "/");
			float altura = atof(p);
			if (altura > 1.70)
				sprintf(respuesta, "%s: eres alto", nombre);
			else
				sprintf(respuesta, "%s: eres bajo", nombre);
		}
		else if (codigo == 4)
		{
			//saber si nombre es palindromo
			int p_i=0;
			int p_f=strlen(nombre)-1;
			
			while ((p_i<p_f) && (nombre[p_i] == nombre[p_f]))
			{
				p_i++;
				p_f--;
			}
			if (p_i >= p_f)
				sprintf(respuesta, "%s: es palindroma", nombre);
			else
				sprintf(respuesta, "%s: NO es palindroma", nombre);
		}
		else if (codigo == 5)
		{
			int i=0;
			char mensaje[25];
			while(i < strlen(nombre))
			{
				if (islower(nombre[i]) != 0)
					mensaje[i] = toupper(nombre[i]);
				else
					mensaje[i] = nombre[i];
				i++;
			}
			sprintf(respuesta, "%s", mensaje);
		}
		
		
		printf("Respuesta: %s\n", respuesta);
		//y lo enviamos
		write (sock_conn, respuesta, strlen(respuesta));
		
		//se acabó el servicio para este cliente
		close(sock_conn);
	}
}

