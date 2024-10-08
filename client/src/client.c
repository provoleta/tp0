#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	log_info(logger, "Hola! Soy un Log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	log_info(logger, "Lei la ip %s y puerto %s\n", ip, puerto);


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/
	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	nuevo_logger = log_create("tp0.log", "TP0", 1, LOG_LEVEL_INFO);
	if(nuevo_logger == NULL) {
		printf("No pude crear el logger.");
		exit(1);
	}
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	nuevo_config = config_create("../cliente.config");
	if(nuevo_config == NULL) {
		printf("No pude leer el config");
		exit(2);
	}
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;
	while(1){
		leido = readline("> ");
		if (leido) {
			add_history(leido);
		}
		if (!strncmp(leido, "", 4)) {
			free(leido);
			break;
		}
		printf("%s\n", leido);
		free(leido);
	}
}


void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	while(1){
		leido = readline("> ");
		if (leido) {
			add_history(leido);
			agregar_a_paquete(paquete, leido, sizeof(leido));
		}
		if (!strncmp(leido, "", 4)) {
			free(leido);
			break;
		}
		printf("%s\n", leido);
		free(leido);
	}
	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	if(logger != NULL) {
		log_destroy(logger);
	}

	if(config != NULL) {
		config_destroy(config);
	}

	liberar_conexion(conexion);
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
}
