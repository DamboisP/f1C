#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

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

int random_int(int min, int max){
	srand(time(NULL));
  return (abs((rand() % (max + 1 - min)) + min));
}

double speedToDistance(speed){
	double convertedDistance = speed/60;
	return convertedDistance;
}

int main(int argc, char **argv){
	int i;
	int shmid;
  key_t key = 5678;
	struct carInfos* carInfosShm;
	int carId = atoi(argv[1]);
	int carNum = atoi(argv[2]);
	int minSpeed = atoi(argv[3]);
	int maxSpeed = atoi(argv[4]);
	char *name = argv[5];
	int randomSpeed;
	double distance = 0;
	int result;

	
	//Circuit divisé en 3 sections (en km)
	int section1 = 1;
	int section2 = 2;
	int section3 = 4;
	
	if ((shmid = shmget(key, 2048, IPC_CREAT | 0666)) < 0) {
    printf("Erreur de shmget");
		return -1;
  }

	if ((carInfosShm = shmat(shmid, NULL, 0)) ==  (struct carInfos*)-1) {
		printf("Erreur de shmat");
    return -1;
	}
	carInfosShm[carId].carIdC = carId;

	
	while(carInfosShm[1].dayTime != 999){	
		if(carInfosShm[carId].tempsRestant != 0){
			carInfosShm[carId].carIdC = carId;
			randomSpeed = random_int(minSpeed,maxSpeed);
			distance += speedToDistance(randomSpeed);
			if(carInfosShm[carId].section == 0){

				if(distance > section1){
					carInfosShm[carId].section = 1;
					carInfosShm[carId].distanceTotale += distance;
					distance -= section1;
					carInfosShm[carId].tempsS1 = ((double)randomSpeed/60)/section1;
				}
			}

			if(carInfosShm[carId].section == 1){

					if(distance > section2){
					carInfosShm[carId].section = 2;
					carInfosShm[carId].distanceTotale += distance;
					distance -= section2;
					carInfosShm[carId].tempsS2 = ((double)randomSpeed/60)/section2;
				}
			}

			if(carInfosShm[carId].section == 2){
				if(distance > section3){
					carInfosShm[carId].nbTour += 1;
					carInfosShm[carId].section = 0;
					carInfosShm[carId].distanceTotale += distance;
					distance -= section3;
					carInfosShm[carId].tempsS3 = ((double)randomSpeed/60)/section3;
					carInfosShm[carId].tempsTourPrecedent = carInfosShm[carId].tempsS1 + carInfosShm[carId].tempsS2 + carInfosShm[carId].tempsS3;
					if(carInfosShm[carId].tempsTourPrecedent < carInfosShm[carId].tempsMeilleurTour || carInfosShm[carId].tempsMeilleurTour == 0 ){
						carInfosShm[carId].tempsMeilleurTour = carInfosShm[carId].tempsTourPrecedent;
					}
				}
			}

			carInfosShm[carId].tempsRestant -= 1;
			sleep(2);
		}
		
	}
	
	shmdt(carInfosShm);
	return 0;
}
