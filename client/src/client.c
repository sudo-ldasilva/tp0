#include "client.h"
#include "utils.h"
#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>
#include <stdlib.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion = 0;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
  log_info(logger, "Hola. Soy un log!!!!");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	if ( !(config = iniciar_config()) ) abort();

	// Usando el config creado previamente, leemos los valores del config y los
	// dejamos en las variables 'ip', 'puerto' y 'valor'
  if ( !(ip = config_get_string_value(config, "IP"))
       || !(puerto = config_get_string_value(config, "PUERTO"))
       || !(valor = config_get_string_value(config, "CLAVE"))
     ) {
    terminar_programa(conexion, logger, config);
    abort();
  }

	// Loggeamos el valor de config
	log_info(logger, "IP: %s | PUERTO: %s | VALOR: %s", ip, puerto, valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	// leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log", "Cliente", true, LOG_LEVEL_DEBUG);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;
	while (strcmp(leido = readline("> "), "")) {
    log_info(logger, "Leí: %s", leido);
    free(leido);
  }
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
  while (strcmp(leido = readline("> "), "")) {
    agregar_a_paquete(paquete, leido, sizeof(leido));
    free(leido);
  }
  enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
  eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config)
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_destroy(logger);
	if (conexion) liberar_conexion(conexion);
	config_destroy(config);
}
