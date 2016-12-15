#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

struct carInfos{
	char * name;
	int carIdC;
	int dayTime;
	int section;
	double tempsS1;
	double tempsS2;
	double tempsS3;
	double tempsMeilleurTour;
	double tempsTourPrecedent;
	int nbTour;
	double distanceTotale;
	int isRacing;
	int flagFin;
};

int main(int argc, char **argv){
	int i;
  int shmid;
  key_t key = 5678;
	struct carInfos* carInfosShm;
	char *carCmd[5];
	char *graphicCmd [2] = {"./graphic",NULL};
	char	enter = 0;
	int timeLeft;

	//Chaque pilote a un nom, un numero
char * 	voitures[][22] =	{
{"0","44","Claude"},
{"1","6","Roger"},
{"2","5","Bernard"},
{"3","7","J-B"},
{"4","3","Warwick"},
{"5","33","Adolf"},
{"6","19","Philippe"},
{"7","77","Derek"},
{"8","11","Joakim"},
{"9","27","Raoul"},
{"10","26","Fernando"},
{"11","55","Jimmy"},
{"12","14","Roberto"},
{"13","22","Patrick"},
{"14","9","Piercarlo"},
{"15","12","Aldo"},
{"16","20","Norberto"},
{"17","30","Esteban"},
{"18","8","Nyakwe"},
{"19","21","Rudolf"},
{"20","31","Niki"},
{"21","94","Jacky"}
};
	
	//Creations de toutes les voitures, de l'interface et mise en place de la memoire partagee
	for(i = 0;i<22;i++){
		carCmd[0]  = "./car";
		carCmd[1]  = voitures[i][0];
		carCmd[2]  = voitures[i][1];
		carCmd[3]  = voitures[i][2];
		carCmd[4]  = NULL;
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
		carInfosShm[i].flagFin = 0;
		carInfosShm[i].isRacing = 0;
	}		
	sleep(1);
	
	//Lancement des premiers essais

	enter = 0;
	while (enter != '\r' && enter != '\n') { enter = getchar(); }
	
	timeLeft = 5;
	for(i =0;i<22;i++){
		carInfosShm[i].nbTour = 0;
		carInfosShm[i].distanceTotale = 0;
		carInfosShm[i].tempsS1 = 0;
		carInfosShm[i].tempsS2 = 0;
		carInfosShm[i].tempsS3 = 0;
		carInfosShm[i].section = 0;
		carInfosShm[i].tempsTourPrecedent = 0;
		carInfosShm[i].tempsMeilleurTour = 0;
		carInfosShm[i].dayTime = 1;
		carInfosShm[i].isRacing = 1;
	}

	while(timeLeft > 0){
		timeLeft -= 1;
		sleep(1);
	}
	
	
	/*
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
		carInfosShm[i].dayTime = 4;
	}

	while(carInfosShm[1].dayTime == 4){
		sleep(1);
	}
	*/
	for(i =0;i<22;i++){
		carInfosShm[i].flagFin = 1;
	}
	
	shmdt(carInfosShm);
	
	return 0;
}
