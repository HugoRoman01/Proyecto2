#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include "cabeceras.h"

#define LONGITUD_COMANDO 100

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps);
int ComprobarComando(char *strcomando, char *orden, char *argumento1, char *argumento2);
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup);
int BuscaFich(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
              char *nombre);
void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos);
int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
              char *nombreantiguo, char *nombrenuevo);
int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, 
             EXT_DATOS *memdatos, char *nombre);
int Borrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           char *nombre,  FILE *fich);
int Copiar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos,
           EXT_BYTE_MAPS *ext_bytemaps, EXT_SIMPLE_SUPERBLOCK *ext_superblock,
           EXT_DATOS *memdatos, char *nombreorigen, char *nombredestino,  FILE *fich);
void Grabarinodosydirectorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, FILE *fich);
void GrabarByteMaps(EXT_BYTE_MAPS *ext_bytemaps, FILE *fich);
void GrabarSuperBloque(EXT_SIMPLE_SUPERBLOCK *ext_superblock, FILE *fich);
void GrabarDatos(EXT_DATOS *memdatos, FILE *fich);

int main()

 	{
	 
 
	 char comando[LONGITUD_COMANDO];
	 char orden[LONGITUD_COMANDO];
	 char argumento1[LONGITUD_COMANDO];
	 char argumento2[LONGITUD_COMANDO];
	 
	 int i,j;
	 unsigned long int m;
     EXT_SIMPLE_SUPERBLOCK ext_superblock;
     EXT_BYTE_MAPS ext_bytemaps;
     EXT_BLQ_INODOS ext_blq_inodos;
     EXT_ENTRADA_DIR directorio[MAX_FICHEROS];
     EXT_DATOS memdatos[MAX_BLOQUES_DATOS];
     EXT_DATOS datosfich[MAX_BLOQUES_PARTICION];
     int entradadir;
     int grabardatos;
     FILE *fent;
     
     
     // Lectura del fichero completo de una sola vez
     
     
     fent = fopen("particion.bin","r+b");
     if ((fent = fopen("particion.bin","r+b")) == NULL){
     	
     	printf ( " Error en la apertura ");
     	
	 }
     fread(&datosfich, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, fent);    
     
     
     memcpy(&ext_superblock,(EXT_SIMPLE_SUPERBLOCK *)&datosfich[0], SIZE_BLOQUE);
     memcpy(&directorio,(EXT_ENTRADA_DIR *)&datosfich[3], SIZE_BLOQUE);
     memcpy(&ext_bytemaps,(EXT_BLQ_INODOS *)&datosfich[1], SIZE_BLOQUE);
     memcpy(&ext_blq_inodos,(EXT_BLQ_INODOS *)&datosfich[2], SIZE_BLOQUE);
     memcpy(&memdatos,(EXT_DATOS *)&datosfich[4],MAX_BLOQUES_DATOS*SIZE_BLOQUE);
     
     // Buce de tratamiento de comandos
     for (;;){
		 do {
		 printf (">> ");
		 fflush(stdin);
		 fgets(comando, LONGITUD_COMANDO, stdin);
		 } while (ComprobarComando(comando,orden,argumento1,argumento2) !=0);
	    
		 if (strcmp(orden,"info")==0) {
		 	
		 LeeSuperBloque(&ext_superblock);	 	
		 	
		 }
		 
		  if (strcmp(orden,"bytemaps")==0){
		  	
		  	Printbytemaps(&ext_bytemaps);
		 
		 }
		
		 if (strcmp(orden,"dir")==0) {
            Directorio(directorio,&ext_blq_inodos);
            }
            
         if(strcmp(orden,"rename")==0){
         	switch(Renombrar(directorio,&ext_blq_inodos,argumento1,argumento2)){
         	
			 case -1: printf("ERROR: el fichero %s ya existe\n",argumento2);
			 	break;
			 
			 case 0 : 
			 	printf("ERROR: el fichero %s no encontrado\n",argumento1);	
			 	break;
			
			 case 1: grabardatos =1;
			 break;
         		
         	
			 }
		 }
		 
		 if (strcmp(orden,"dir")==0) {
		 	switch(Imprimir(directorio,&ext_blq_inodos,datosfich,argumento1))
		 	
		 	case 0:
		 		printf("ERROR: el fichero %s no encontrado\n",argumento1);
		 }
         /*
         // Escritura de metadatos en comandos rename, remove, copy     
         Grabarinodosydirectorio(&*directorio,&ext_blq_inodos,fent);
         GrabarByteMaps(&ext_bytemaps,fent);
         GrabarSuperBloque(&ext_superblock,fent);
         if (grabardatos){
		 
           GrabarDatos(&*memdatos,fent);
         grabardatos = 0;
     }
     	if (strcmp(*orden,"rename")==0){
		  	
		 
		 }
		 
		 if (strcmp(*orden,"imprimir")==0){
		  	
		 
		 }
		 
		 if (strcmp(*orden,"remove")==0){
		  	
		 
		 }
		 
		 if (strcmp(*orden,"copy")==0){
		  	
		 
		 }
		
     
     
         //Si el comando es salir se habrÃ¡n escrito todos los metadatos
         //faltan los datos y cerrar
         if (strcmp(*orden,"salir")==0){
            GrabarDatos(&*memdatos,fent);
            fclose(fent);
            return 0;
         }*/
 
        }
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////////////		 
		 
     }
 
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *psup){
	
	printf("El bloque tiene %d bytes", psup->s_block_size);
    printf("\nInodos particion = %d", psup->s_inodes_count);
    printf("\ninodos libres = %d", psup->s_free_inodes_count);
    printf("\nBloques particion = %d", psup->s_blocks_count);
    printf("\nBloques libres = %d", psup->s_free_blocks_count);
    printf("\nPrimer bloque de datos = %d\n\n", psup->s_first_data_block);

}

void Printbytemaps(EXT_BYTE_MAPS *ext_bytemaps){
    printf("Inodos :");
    int i, j;
    for(i = 0;i< MAX_INODOS; i++){
        printf("%d ", ext_bytemaps ->bmap_inodos[i]);
    }
    printf("\nBloques [0-25] :");
    for(j=0; j<25; j++){
        printf("%d ", ext_bytemaps ->bmap_bloques[j]);
    }
    printf("\n");

}


void Directorio(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos){
	
	int i,j;
	
	for(i=1;MAX_FICHEROS;i++){
		if((directorio+i)->dir_inodo!=0){
			printf("%s\ttamaño: %d\tinodo: %d\tbloques:",(directorio+i)->dir_nfich, inodos->blq_inodos[(directorio+i)->dir_inodo].size_fichero, (directorio+i)->dir_inodo);
			for(j=0; inodos ->blq_inodos[(directorio+i)->dir_inodo].i_nbloque[j]!=NULL_INODO;j++){
				
					printf("%d", inodos->blq_inodos[(directorio+i)->dir_inodo].i_nbloque[j]);
			}
			printf("\n");
		}
	
	}

}


int Renombrar(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, char *nombreantiguo, char *nombrenuevo){
	
	short unsigned int comprobar=0;
	short unsigned int wardI;
	int i;
	
	for( i=1; ((directorio+i)->dir_inodo)!=NULL_INODO;i++){
		
		if((strcmp(nombreantiguo,(directorio+i)->dir_nfich)==0)){
			
			comprobar++;
			wardI=i;
			
		
		}
		if(strcmp(nombrenuevo,(directorio+i)->dir_nfich)==0){
			return -1;
		}
	}
	
	if(comprobar=1){
		strcpy((directorio+wardI)->dir_nfich,nombrenuevo);
	}
	return comprobar;
	
	
}

int Imprimir(EXT_ENTRADA_DIR *directorio, EXT_BLQ_INODOS *inodos, EXT_DATOS *memdatos, char *nombre){
	
	short unsigned int comprobar=0;
	short unsigned int wardI;
	int i,j;
	
	for( i=1; ((directorio+i)->dir_inodo)!=NULL_INODO;i++){
		
		if((strcmp(nombre,(directorio+i)->dir_nfich)==0)){
			
			comprobar++;
			wardI=i;
			continue;
		
		}	
}

	if(comprobar){
		for( i=0; inodos->blq_inodos[(directorio+wardI)->dir_inodo].i_nbloque[i]!=NULL_INODO;i++){
	/*		unsigned char var[SIZE_BLOQUE];
			memcpy(var,memdatos[inodos->blq_inodos[(directorio+wardI)->dir_inodo].i_nbloque[i]].dato, SIZE_BLOQUE);	
			var[SIZE_BLOQUE]='\0';
			printf("%s",var);*/
			for ( j = 0; j < SIZE_BLOQUE; j++){
      			printf("%c", memdatos[inodos->blq_inodos[(directorio+wardI)->dir_inodo].i_nbloque[i]-4].dato[j]);
     		}
		}
		printf("\n");
	}
	return comprobar;
}







int ComprobarComando( char *strcomando, char *orden, char *argumento1, char *argumento2){

	//VACIAMOS LOS COMANDOS
	
	strcpy(orden,"");
	strcpy(argumento1,"");
	strcpy(argumento2,"");
	
	char temp[strlen(strcomando)];
	strncpy(temp, strcomando, strlen(strcomando)-1);
	temp[strlen(strcomando)-1]= '\0';
	char *token = strtok(temp," ");
	
	//DIVIDIR EL COMANDO
	
	if(token != NULL){
		int i;
		for(i =0; token != NULL; i++){
			
			switch(i){
			
			
				case 0:
						strcpy(orden, token);
						token = strtok(NULL," ");
						break;
						
				case 1:
						strcpy(argumento1, token);
						token = strtok(NULL," ");
						break;
				
				case 2:
						strcpy(argumento2, token);
						token = strtok(NULL," ");
						break;
				
				default:
						return 1;
												
		}
	}
	
}else{
		return 1;
}
		
		if(strcmp(orden,"info")==0){
			return 0;
		}else if(strcmp(orden,"bytemaps")==0){
			return 0;
		}else if(strcmp(orden,"dir")==0){
			return 0;
		}else if(strcmp(orden,"rename")==0){
			return !(strlen(argumento1)>0 && strlen(argumento2)>0);
		}else if(strcmp(orden,"imprimir")==0){
			return !(strlen(argumento1)>0);
		}


}



