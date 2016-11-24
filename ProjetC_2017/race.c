#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

struct carInfos{
	int carIdC;
	int dayTime;
	int section;
	double tempsS1;
	double tempsS2;
	double tempsS3;
	double tempsMeilleurTour;
	double tempsTourPrecedent;
	int nbTour;
	double tempsRestant;
	double distanceTotale;
};

int main(int argc, char **argv){
	int i;
  int shmid;
  key_t key = 5678;
	struct carInfos* carInfosShm;
	char *carCmd[7];
	char *graphicCmd [2] = {"./graphic",NULL};
	char	enter = 0;

	//Chaque pilote a un nom, un numero, une vitesse min et une vitesse max
char * 	voitures[][5] =	{{"0","34","70","170","Claude"},{"1","64","60","165","Roger"},{"2","69","70","180","Bernard"}
,{"3","78","40","100","J-B"},{"4","98","80","210","Steak"}};
	
	//Creations de toutes les voitures, de l'interface et mise en place de la memoire partagee
	for(i = 0;i<22;i++){
		carCmd[0]  = "./car";
		carCmd[1]  = voitures[i][0];
		carCmd[2]  = voitures[i][1];
		carCmd[3]  = voitures[i][2];
		carCmd[4]  = voitures[i][3];
		carCmd[5]  = voitures[i][4];
		carCmd[6]  = NULL;
		if(fork() == 0){
			execvp(carCmd[0],carCmd);
		}
	}
		if(fork() == 0){
			execvp(graphicCmd[0],graphicCmd);
		}
    if ((shmid = shmget(key, 2048, IPC_CREAT | 0666)) < 0) {
        printf("Erreur de shmget");
        return -1;
    }

	if ((carInfosShm = shmat(shmid, NULL, 0)) ==  (struct carInfos*)-1) {
		printf("Erreur de shmat");
    return -1;
	}

	//Demarrage de la course
	for(i =0;i<22;i++){
		carInfosShm[i].dayTime = 0;
		carInfosShm[i].tempsRestant = 0;
	}		
	sleep(1);
	
	//Lancement des premiers essais

	enter = 0;
	while (enter != '\r' && enter != '\n') { enter = getchar(); }

	for(i =0;i<22;i++){
		carInfosShm[i].nbTour = 0;
		carInfosShm[i].distanceTotale = 0;
		carInfosShm[i].tempsS1 = 0;
		carInfosShm[i].tempsS2 = 0;
		carInfosShm[i].tempsS3 = 0;
		carInfosShm[i].section = 0;
		carInfosShm[i].tempsTourPrecedent = 0;
		carInfosShm[i].tempsMeilleurTour = 0;
		carInfosShm[i].tempsRestant = 5;
		carInfosShm[i].dayTime = 1;
	}

	while(carInfosShm[1].dayTime == 1){
		sleep(1);
	}
	//Lancement des seconds essais
	enter = 0;
	while (enter != '\r' && enter != '\n') { enter = getchar(); }


	for(i =0;i<22;i++){
		carInfosShm[i].nbTour = 0;
		carInfosShm[i].distanceTotale = 0;
		carInfosShm[i].tempsS1 = 0;
		carInfosShm[i].tempsS2 = 0;
		carInfosShm[i].tempsS3 = 0;
		carInfosShm[i].section = 0;
		carInfosShm[i].tempsTourPrecedent = 0;
		carInfosShm[i].tempsMeilleurTour = 0;
		carInfosShm[i].tempsRestant = 5;
		carInfosShm[i].dayTime = 2;
	}

	while(carInfosShm[1].dayTime == 2 || carInfosShm[1].tempsRestant > 0){
		sleep(1);
	}
	//Lancement des troisièmes essais
	enter = 0;
	while (enter != '\r' && enter != '\n') { enter = getchar(); }


	for(i =0;i<22;i++){
		carInfosShm[i].nbTour = 0;
		carInfosShm[i].distanceTotale = 0;
		carInfosShm[i].tempsS1 = 0;
		carInfosShm[i].tempsS2 = 0;
		carInfosShm[i].tempsS3 = 0;
		carInfosShm[i].section = 0;
		carInfosShm[i].tempsTourPrecedent = 0;
		carInfosShm[i].tempsMeilleurTour = 0;
		carInfosShm[i].tempsRestant = 5;
		carInfosShm[i].dayTime = 3;
	}

	while(carInfosShm[1].dayTime == 3 || carInfosShm[1].tempsRestant > 0){
		sleep(1);
	}
	//Qualification n°1
	enter = 0;
	while (enter != '\r' && enter != '\n') { enter = getchar(); }

	for(i =0;i<22;i++){
		carInfosShm[i].nbTour = 0;
		carInfosShm[i].distanceTotale = 0;
		carInfosShm[i].tempsS1 = 0;
		carInfosShm[i].tempsS2 = 0;
		carInfosShm[i].tempsS3 = 0;
		carInfosShm[i].section = 0;
		carInfosShm[i].tempsTourPrecedent = 0;
		carInfosShm[i].tempsMeilleurTour = 0;
		carInfosShm[i].tempsRestant = 5;
		carInfosShm[i].dayTime = 4;
	}

	while(carInfosShm[1].dayTime == 4){
		sleep(1);
	}
	
	/*
	sleep(4);

	shm[30] = 2;
	for(i =0;i<5;i++){
		shm[atoi(voitures[i][0])] = 2;
	}
	sleep(4);
	shm[30] = 30;
	for(i =0;i<5;i++){
		shm[atoi(voitures[i][0])] = 30;
	}*/

	carInfosShm[1].dayTime = 999;
	shmdt(carInfosShm);
	
	return 0;
}
