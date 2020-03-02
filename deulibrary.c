#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_ROOMS 10
#define MAX_KEEPERS 10
#define MAX_STUDENTS 100

//Threads
void *Student(void *num);
void *RoomKeeper(void *num);
void randwait(int secs);
void clearScreen();
int randomNumber();
void printInterface();

int allDone=0;

sem_t Library;//Students arrive library.
sem_t Room[10];//Counting Semaphore for Room.

int main(int argc, char *argv[]){
	pthread_t kptid[MAX_KEEPERS];
	pthread_t stid[MAX_STUDENTS];
	int i; int Number[MAX_STUDENTS];
	printf("A solution to the DEU Library problem with semaphores\n");
	for(i=0;i<MAX_STUDENTS;i++){
		Number[i]=i;
	}
	//Initialize semaphores
	sem_init(&Library,0,MAX_STUDENTS);
	for(i=0;i<MAX_ROOMS;i++){
		sem_init(&Room[i],0,4);
	}
	//Create Keepers
	for(i=0;i<MAX_KEEPERS;i++){
		pthread_create(&kptid[i],NULL,RoomKeeper,(void*)&Number[i]);
	}
	//Create Students
	for(i=0; i<MAX_STUDENTS; i++){
		pthread_create(&stid[i],NULL,Student,(void*)&Number[i]);
	}

	for(i=0; i<MAX_STUDENTS; i++){
		pthread_join(stid[i],NULL);
		sleep(2);
	}
	allDone=1;
	for(i=0;i<MAX_KEEPERS;i++){
		pthread_join(kptid[i],NULL);
	}
	system("PAUSE");
	return 0;
}
void *Student(void *number){
	clearScreen();
	int val,roomNum,i;
	int printvals[10];
	int temp;
		int num = *(int *)number;
		while(1){
			roomNum = randomNumber();
			sem_getvalue(&Room[roomNum],&val);
			if(val!=0){
				break;
			}
		}

	sem_wait(&Library);
	sem_wait(&Room[roomNum]);
	for(i=0;i<MAX_ROOMS;i++){
		sem_getvalue(&Room[i],&temp);
		printvals[i]=temp;
	}
	printInterface(printvals);


}

void *RoomKeeper(void *number){
	int num = *(int *)number;
	int val,i;
	while(!allDone){
			sem_getvalue(&Room[num],&val);
			if(val==0){
			for(i=0;i<4;i++){
				sem_post(&Room[num]);
			}
			}
	}
}

void clearScreen()
{
  const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
  write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

int randomNumber(){
	int num = rand()%10 ;
	return num;
}

void printInterface(int values[]){
	int RemP[10];
	char Stat[10];
	int i,Lib;
	sem_getvalue(&Library,&Lib);
	for(i=0;i<10;i++){
		RemP[i]=4-values[i];
		if(RemP[i]==4){
			Stat[i]='C';
		}
		else if(RemP[i]==0){
			Stat[i]='B';
		}
		else{
			Stat[i]='O';
		}
	}
	printf("---------------------DEU LIBRARY--------------------\n");
  printf("||---R1--||---R2--||---R3--||---R4--||---R5--||\n");
  printf("||Stud:%d ||Stud:%d ||Stud:%d ||Stud:%d ||Stud:%d ||\n",values[0],values[1],values[2],values[3],values[4]);
  printf("||RemP:%d ||RemP:%d ||RemP:%d ||RemP:%d ||RemP:%d ||\n",RemP[0],RemP[1],RemP[2],RemP[3],RemP[4]);
  printf("||Stat:%c ||Stat:%c ||Stat:%c ||Stat:%c ||Stat:%c ||\n",Stat[0],Stat[1],Stat[2],Stat[3],Stat[4]);
  printf("||---R6--||---R7--||---R8--||---R9--||--R10--||\n");
  printf("||Stud:%d ||Stud:%d ||Stud:%d ||Stud:%d ||Stud:%d ||\n",values[5],values[6],values[7],values[8],values[9]);
  printf("||RemP:%d ||RemP:%d ||RemP:%d ||RemP:%d ||RemP:%d ||\n",RemP[5],RemP[6],RemP[7],RemP[8],RemP[9]);
  printf("||Stat:%c ||Stat:%c ||Stat:%c ||Stat:%c ||Stat:%c ||\n",Stat[5],Stat[6],Stat[7],Stat[8],Stat[9]);
  printf("Library:%d----------------------------------------------\n",Lib);
	printf("Stud:Student,RemP:Remaining Place,Stats:C=Cleaning,O=Open,B=Busy\n");
	printf("\n");

}
