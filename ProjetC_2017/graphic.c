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
int i;
int j;
int shmid;
key_t key = 5678;
struct carInfos* carInfosShm;
double distanceArray[22];

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

int compare( const void* a, const void* b)
{
     int int_a = * ( (int*) a );
     int int_b = * ( (int*) b );

     if ( int_a == int_b ) return 0;
     else if ( int_a < int_b ) return -1;
     else return 1;
}

void race(){
for(i=0;i<22;i++){
		if(carInfosShm[i].tempsMeilleurTour != 0){distanceArray[i] = carInfosShm[i].tempsMeilleurTour;}
	}
	 qsort(distanceArray, sizeof(distanceArray)/sizeof(distanceArray[0]), sizeof(distanceArray[0]), compare);

		system("clear");
			for(i=0;i<22;i++){
				for(j=0;j<22;j++){
					if(carInfosShm[j].tempsMeilleurTour == distanceArray[i] && carInfosShm[j].tempsMeilleurTour != 0){
						carInfosShm[j].carIdC = i;
					}
				}
			}
		printf("Temps restant: %.2f\n\n",carInfosShm[1].tempsRestant);
		printf("Nom		|Tours	|Temps S1	|Temps S2	|Temps S3	|Temps tour	|Temps meilleur tour	\n");
		printf("------------------------------------------------------------------------------------------------------------------\n");
		for(i =0;i<22;i++){
			printf("%s		|",voitures[carInfosShm[i].carIdC][2]);
			printf("%d	|",carInfosShm[carInfosShm[i].carIdC].nbTour);
			printf("%.2f		|",carInfosShm[carInfosShm[i].carIdC].tempsS1);
			printf("%.2f		|",carInfosShm[carInfosShm[i].carIdC].tempsS2);
			printf("%.2f		|",carInfosShm[carInfosShm[i].carIdC].tempsS3);
			printf("%.2f		|",carInfosShm[carInfosShm[i].carIdC].tempsTourPrecedent);
			printf("%.2f 		\n",carInfosShm[carInfosShm[i].carIdC].tempsMeilleurTour);
		}
		
		printf("\n\nSECTION 1###SECTION 2###SECTION 3 \n\n");
		for(i =0;i<10;i++){
			if(carInfosShm[i].section == 0){
				printf(" #%s#> \n",voitures[i][1]);
			}
			else if(carInfosShm[i].section == 1){
				printf(" 		#%s#> \n",voitures[i][1]);
			}
			else if(carInfosShm[i].section == 2){
				printf(" 				#%s#> \n",voitures[i][1]);
			}
		}
		sleep(2);
}
void printResults(int when){

		system("clear");
		printf("Classement general: \n\n",carInfosShm[1].tempsRestant);
		printf("Nom		|Tours	|Temps S1	|Temps S2	|Temps S3	|Temps tour	|Temps meilleur tour	\n");
		printf("------------------------------------------------------------------------------------------------------------------\n");
		for(i =0;i<22;i++){
			printf("%s		|",voitures[carInfosShm[i].carIdC][2]);
			printf("%d	|",carInfosShm[carInfosShm[i].carIdC].nbTour);
			printf("%.2f		|",carInfosShm[carInfosShm[i].carIdC].tempsS1);
			printf("%.2f		|",carInfosShm[carInfosShm[i].carIdC].tempsS2);
			printf("%.2f		|",carInfosShm[carInfosShm[i].carIdC].tempsS3);
			printf("%.2f		|",carInfosShm[carInfosShm[i].carIdC].tempsTourPrecedent);
			printf("%.2f 		\n",carInfosShm[carInfosShm[i].carIdC].tempsMeilleurTour);
		}
		if(when == 1){
			printf("La seconde séance d'essais va commencer, appuyez sur ENTER pour continuer\n\n");
		} 
		else if (when == 2) {
			printf("La troisème séance d'essais va commencer, appuyez sur ENTER pour continuer\n\n");
		}
		else if (when == 3) {
			printf("La première séance de qualifications va commencer, appuyez sur ENTER pour continuer\n\n");
		}
		carInfosShm[1].dayTime = 0;
		
		sleep(2);
}

int main(int argc, char **argv){

  if((shmid = shmget(key, 2048, IPC_CREAT | 0666)) < 0) {
		printf("Erreur de shmget");
    return -1;
  }

	if ((carInfosShm = shmat(shmid, NULL, 0)) ==  (struct carInfos*)-1) {
		printf("Erreur de shmat");
    return -1;
	}
	
	system("clear");
	printf("###################################\n");
	printf("# Bienvenue au Grand-Prix de 2016 #\n");
	printf("#       a Spa-Francorchamps       #\n");
	printf("###################################\n");
	printf("La première séance d'essais va commencer, appuyez sur ENTER pour continuer\n");
	
	while(carInfosShm[1].dayTime != 999){	
		if(carInfosShm[1].tempsRestant == 0 && carInfosShm[1].dayTime != 0){
			printResults(carInfosShm[1].dayTime);
		}
		else if(carInfosShm[1].dayTime == 1){
			race();
		}
		else if(carInfosShm[1].dayTime == 2){
			race();
		}
		else if(carInfosShm[1].dayTime == 3){
			race();
		}
		else if(carInfosShm[1].dayTime == 4){
			race();
		}
		else{
			sleep(1);
		}


	}
	
	shmdt(carInfosShm);
	return 0;
}
