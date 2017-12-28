#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> ///wichtig für Dateikreierung
#include <sys\stat.h>
#include "sha256.h"
#include "declarations.h"


///fopen Modi: "r" lesen, "r+" lesen und schreiben, "w" Datei erstellen (wenn vorhanden, wird alte gelöscht) und beschreiben, "w+" Datei erstellen (wenn vorhanden, wird alte gelöscht) und zum Lesen und Schreiben öffnen; wird "b" an andere Modi angehängt (z.B.:"rb+" für lesen und schreiben), werden diese im Binärmodus verwendet


int saveCampaign(int map, int difficulty, int score)
{
	FILE *savedata;
	char hash[32];
	savedata = fopen("resources/data/savegame.dat", "wb+"); //open file in binary mode (and delete old file (w+)) to succesfully write hash later on
	if (savedata == NULL)
	{
		creat("resources/data/savegame.dat", O_RDWR);
		savedata = fopen("resources/data/savegame.dat", "wb+");
	}

	char cmap[2], cdifficulty[2], cscore[6], save[SAVESIZE];

	sprintf(cmap, "%d", map);
	sprintf(cdifficulty, "%d", difficulty);
	sprintf(cscore, "%d", score);

	//wenn Zehnerpotenz von score zu klein, Rest des arrays mit '\n' auffüllen
	if (score < 10000)
	{
		if (score < 1000)
		{
			if (score < 100)
			{
				if (score < 10)
				{
					cscore[1] = '\n';
				}
				cscore[2] = '\n';
			}
			cscore[3] = '\n';
		}
		cscore[4] = '\n';
		cscore[5] = '\0';//char array zu String abschließen
	}

	SHA256_CTX hashstruct; //hashing arrays for secure saving
	sha256_init(&hashstruct);
	sha256_update(&hashstruct, cmap, strlen(cmap));
	sha256_update(&hashstruct, cdifficulty, strlen(cdifficulty));
	sha256_update(&hashstruct, cscore, strlen(cscore));
	sha256_update(&hashstruct, SALT, SALTSIZE);
	sha256_final(&hashstruct, hash);

	strcpy(save, cmap);
	//printf("%s\n%s\n%s\n", cmap, cdifficulty, cscore);
	strcat(strcat(strcat(strcat(save, "\n"), cdifficulty), "\n"), cscore);

	//printf("%s", save);
	fwrite(save, sizeof(char), SAVESIZE, savedata); //write savedata
	fwrite(hash, sizeof(char), 32, savedata); //write hash
	fclose(savedata);
	return 0;
}

int loadCampaign(int *map, int *difficulty, int *score)
{
	char block[SAVESIZE];
	char cmap[2], cdifficulty[2], cscore[6];
	unsigned char hash[32], hash2[32];
	char *ptrI, *ptrII, *ptrIII;
	FILE *savedata;
	savedata = fopen("resources/data/savegame.dat", "rb"); //Datei im binären Lesemodus öffnen

	if (savedata == NULL) //überprüfen ob Spieldatei vorhanden ist
	{
		printf("Keine Speicherdatei vorhanden.\n");
		return -1;
	}

	fread(block, sizeof(char), SAVESIZE, savedata); //read savedata from file
	fread(hash, sizeof(char), 32, savedata);//read hash from file
	fclose(savedata);

	cmap[0] = block[0];
	cmap[1] = '\0';
	cdifficulty[0] = block[2];
	cdifficulty[1] = '\0';
 	for (int i = 0; i < (sizeof(char) * 5); i++)
	{
		cscore[i] = block[i + 4];
	}
	cscore[5] = '\0';

	SHA256_CTX hashstruct; //hashing arrays to check with hash from saving
	sha256_init(&hashstruct);
	sha256_update(&hashstruct, cmap, (strlen(cmap)));
	sha256_update(&hashstruct, cdifficulty, (strlen(cdifficulty)));
	sha256_update(&hashstruct, cscore, (strlen(cscore)));
	sha256_update(&hashstruct, SALT, SALTSIZE);
	sha256_final(&hashstruct, hash2);


	if (memcmp(hash, hash2, SHA256_BLOCK_SIZE) != 0)
	{
		printf("Speicherdaten korrumpiert.\nLoesche Speicherdaten...\n");
		remove("resources/data/savegame.dat");
		return -1;
	}
	//printf("Hashwert erfolgreich verifiziert.\n");

	*map = strtol(cmap, &ptrI, 10); //Strings in Integer umwandeln
	*difficulty = strtol(cdifficulty, &ptrII, 10);
	*score = strtol(cscore, &ptrIII, 10);

	//printf("\n%d\n%d\n%d\n", *map, *difficulty, *score);
	return 0;

}

int initScorelist(char scorearray[17][SCORESIZE]) //muss Dimension 17 haben, obwohl nur 15 benötigt wird, um stapelbasierten Pufferüberlauf zu verhindern
{
	FILE *scorelist;
	char hash[32];

	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < SCORESIZE; j++)
		{
			scorearray[i][j] = 0; //vorherige Daten löschen
		}
	}

	for (int i = 0; i < 15; i++)
	{
		strcpy(scorearray[i], "none            0"); //Kopieren für einheitliche Hashkreierung und Speicherweise mit anderen Score-Funktionen
	}

	scorelist = fopen("resources/data/scorelist.dat", "rb+");

	if (scorelist == NULL)
	{
		scorelist = fopen("resources/data/scorelist.dat", "wb");
	}


	SHA256_CTX hashstruct; //hashing arrays to check with hash from saving
	sha256_init(&hashstruct);

	for (int i = 0; i < 15; i++)
	{
		sha256_update(&hashstruct, scorearray[i], SCORESIZE);
		fwrite(scorearray[i], sizeof(char), SCORESIZE, scorelist); //create scorelist
	}
	sha256_update(&hashstruct, SALT, SALTSIZE);
	sha256_final(&hashstruct, hash);
	fwrite(hash, sizeof(char), 32, scorelist);
	fclose(scorelist);
	return 0;
}

int readScore(char scorearray[17][SCORESIZE])
{
	FILE *scorelist;
	scorelist = fopen("resources/data/scorelist.dat", "rb");

	if (scorelist == NULL)
	{
		initScorelist(scorearray);
		printf("Neue Scoreliste erstellt.\n");
		scorelist = fopen("resources/data/scorelist.dat", "rb");
	}

	char block[SCORESIZE], hash[32], hash2[32];
	for (int i = 0; i < 15; i++)
	{
		fread(block, sizeof(char), SCORESIZE, scorelist);
		memcpy(scorearray[i], block, SCORESIZE);
	}
	fread(hash, sizeof(char), 32, scorelist);
	fclose(scorelist); //Schließen des streams


	//hashing arrays to check with hash from saving
	SHA256_CTX hashstruct;
	sha256_init(&hashstruct);

	for (int i = 0; i < 15; i++)
	{
		sha256_update(&hashstruct, scorearray[i], SCORESIZE);
	}
	sha256_update(&hashstruct, SALT, SALTSIZE);
	sha256_final(&hashstruct, hash2);

	if (memcmp(hash, hash2, SHA256_BLOCK_SIZE) != 0)
	{
		printf("Speicherdaten korrumpiert.\n");
		remove("resources/data/scorelist.dat");
		printf("Scoreliste geloescht.\n");
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < SCORESIZE; j++)
			{
				scorearray[i][j] = 0; //korrupte Daten löschen
			}
		}
		initScorelist(scorearray);
		printf("Neue Scoreliste erstellt.\n");
		return 1;
	}

	return 0;
}

int putScore(int difficulty, int score, char name[])
{
	char scorearray[17][SCORESIZE];
	FILE *scorelist;
	char hash[32];
	char newscore[SCORESIZE] = {0};

	/*printf("Bitte Spielername eingeben: ");//Spielerinformationen abfragen
	scanf("%s", name);
	while (strlen(name) > 14)
	{
		printf("\nMaximal 14 Zeichen.\nBitte gebe deinen Namen ein:");
		scanf("%s", name);
	}
	printf("\n");*/


	scorelist = fopen("resources/data/scorelist.dat", "rb+"); // datei zum schreiben und lesen öffnen
	if (scorelist == NULL)
	{
		initScorelist(scorearray);
		scorelist = fopen("resources/data/scorelist.dat", "rb+");
	}
	readScore(scorearray); //alten score auslesen


	//erneutes Öffnen, da Stream in readScore geschlossen wird
	scorelist = fopen("resources/data/scorelist.dat", "rb+");

	// creating new string for score of player
	sprintf(newscore, "                %d", score);
	strncpy(newscore, name, strlen(name));

	//initializing hashing
	SHA256_CTX hashstructscore;
	sha256_init(&hashstructscore);

	//Fallunterscheidung für Schwierigkeitsgrade
	int activescore;
	if (difficulty == 1)
	{
		for (int i = 0; i < 5; i++)
		{
		    sscanf(scorearray[i] + 16, "%d", &activescore);
			if (score > activescore)//überprüfen, ob Spieler in Liste aufgenommen werden soll
			{
				for (int j = 4; j > i; j--)//score einen weiter "schieben"
				{
					memcpy(scorearray[j], scorearray[j - 1], SCORESIZE);
				}
				memcpy(scorearray[i], newscore, SCORESIZE);//neuen score einfügen
                break;
			}
		}
	}
	else if (difficulty == 2)
	{
		for (int i = 5; i < 10; i++)
		{
			sscanf(scorearray[i] + 16, "%d", &activescore);
			if (score > activescore)//überprüfen, ob Spieler in Liste aufgenommen werden soll
			{
				for (int j = 9; j > i; j--)//score einen weiter "schieben"
				{
					memcpy(scorearray[j], scorearray[j - 1], SCORESIZE);
				}
				memcpy(scorearray[i], newscore, SCORESIZE);//neuen score einfügen
				break;
			}
		}
	}
	else if (difficulty == 3)
	{
		for (int i = 10; i < 15; i++)
		{
			sscanf(scorearray[i] + 16, "%d", &activescore);
			if (score > activescore)//überprüfen, ob Spieler in Liste aufgenommen werden soll
			{
				for (int j = 14; j > i; j--)//score einen weiter "schieben"
				{
					memcpy(scorearray[j], scorearray[j - 1], SCORESIZE);
				}
				memcpy(scorearray[i], newscore, SCORESIZE);//neuen score einfügen

			}
		}
	}
	else
	{
		printf("sth. went wrong with the difficulty.");
	}
	for (int i = 0; i < 15; i++) {
        sha256_update(&hashstructscore, scorearray[i], SCORESIZE);
        fwrite(scorearray[i], sizeof(char), SCORESIZE, scorelist);
	}

	//hash abschließen
	sha256_update(&hashstructscore, SALT, SALTSIZE);
	sha256_final(&hashstructscore, hash);

	//Datei zu Ende schreiben und stream schließen
	fwrite(hash, sizeof(char), 32, scorelist);
	fclose(scorelist);
	return 0;
}



///Als erstes wird die gesamte Anzahl der Kurven ausgelesen, danach die Felder, danach die Werte für Startpunkte und dann die Richtung und Länge für die Wege, am Ende kommen die Waveinformationen dran.


int loadMap(GameObject *game, Field *map, int mapnum, int difficulty)
{
	char hash[32], hash_old[32];
	int redirections, starts, waves;
	char mapname[24] = "resources/data/map";
	char maptemp[2];
	sprintf(maptemp, "%d", mapnum);
	strcat(mapname, maptemp);
	strcat(mapname, ".dat");

	SHA256_CTX hashstruct;
	sha256_init(&hashstruct);

	FILE *mapfile = fopen(mapname, "r");
	if (mapfile == NULL) {
        printf("Something went horribly wrong while trying to load the map!\nTry to restart the game. If the error persists, reinstall the game.\n");
        return -1;
	}
	fscanf(mapfile, "%d,", &redirections);
	sha256_update(&hashstruct, &redirections, 4);
	fscanf(mapfile, "%d,", &starts);
	sha256_update(&hashstruct, &starts, 4);
	fscanf(mapfile, "%d,", &waves);
	sha256_update(&hashstruct, &waves, 4);

	Path *base = malloc(sizeof(Path));
	if (base == NULL)
		return -1;
	PathStart *start = malloc(starts * sizeof(PathStart));
	if (start == NULL)
		return -1;
	PathElement *path = malloc(redirections * sizeof(PathElement));
	if (path == NULL)
		return -1;



	for (int i = 0; i < MAPSIZE; i++)
	{
		fscanf(mapfile, "%d,", &map[i].type);
		map[i].turret = NULL;
		sha256_update(&hashstruct, &map[i].type, 4);
	}

	for (int i = 0; i < starts; i++)
	{
		int p;
		fscanf(mapfile, "%d,%d,%d,%d,", &start[i].x, &start[i].y, &start[i].remain, &p);
		sha256_update(&hashstruct, &start[i].x, 4);
		sha256_update(&hashstruct, &start[i].y, 4);
		sha256_update(&hashstruct, &start[i].remain, 4);
		sha256_update(&hashstruct, &p, 4);
		start[i].elem = &path[p];
	}


	int direction, length, follow;
	for (int i = 0; i < redirections; i++)
	{
		fscanf(mapfile, "%d,%d,%d,", &direction, &length, &follow);
		sha256_update(&hashstruct, &direction, 4);
		sha256_update(&hashstruct, &length, 4);
		sha256_update(&hashstruct, &follow, 4);
		path[i].dir = direction;
		path[i].length = length;
		if (follow != 0)
			path[i].next = path + follow;
		else
			path[i].next = NULL;
	}
	Wave *temp, *waveStart, *active;
	for (int i = 0; i < waves; i++)
	{
		active = malloc(sizeof(Wave));
		if (active == NULL) {
            printf("A pointer is Null where it should not be!\n(wave loading)\n");
            fclose(mapfile);
			return -1;
		}
		fscanf(mapfile, "%d,", &active->type);
		sha256_update(&hashstruct, &active->type, 4);
		fscanf(mapfile, "%d,", &active->health);
		sha256_update(&hashstruct, &active->health, 4);
		fscanf(mapfile, "%d,", &active->amount);
		sha256_update(&hashstruct, &active->amount, 4);

		switch (difficulty)
		{
		case 1:
			active->health *= 0.8;
			active->amount *= 0.9;
			break;
		case 3:
			active->health *= 1.4;
			active->amount *= 1.2;
			break;
		}

		if (i == 0)
			waveStart = active;
        else
            temp->next = active;

		temp = active;
	}
	temp->next = NULL;

	base->startcount = starts;
	base->starts = start;
	base->arrayStart = path;


	//GameObject "befüllen"
	game->path = base;
	game->waves = waveStart;
	fscanf(mapfile, "%d,", &(game->missionStaff));
	sha256_update(&hashstruct, &(game->missionStaff), 4);
	fscanf(mapfile, "%d,", &(game->missionEnd));
	sha256_update(&hashstruct, &(game->missionEnd), 4);

	sha256_update(&hashstruct, SALT, SALTSIZE);
	sha256_final(&hashstruct, hash);

	for (int i = 0; i < 32; i++)
	{
		int hint;
		fscanf(mapfile, "%d,", &hint);
		hash_old[i] = (char) hint;
		if (hash_old[i] != hash[i]) {
            fclose(mapfile);
            //printf("hash failed: got %d, expected %d on %d\n", hash_old[i], hash[i], i);
			return -1;
        }
		//printf("%d ",hash[i]); //zum Auslesen und manuellen Einfügen des Hashes in die mapX.dat

	}
	fclose(mapfile);
	return 0;
}

