#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

typedef struct koordinat {
    int x, y;
} Koordinat;

typedef struct data {
    char name[15];
    int pasien;
} Data;

void panduan() {

    system("cls");

    printf("Panduan Gerakan :\n\n");
    printf("ketik 'w' lalu enter untuk ke-atas\n");
    printf("ketik 's' lalu enter untuk ke-bawah\n");
    printf("ketik 'a' lalu enter untuk ke-kiri\n");
    printf("ketik 'd' lalu enter untuk ke-kanan\n\n");
    printf("------------------------------------\n");
    printf("Panduan menu :\n\n");
    printf("ketik 'm' lalu enter untuk Menyimpan data\n");
    printf("ketik 'e' lalu enter untuk Keluar dari game\n");
    printf("------------------------------------\n");

    printf("ketik enter untuk melanjutkan game\n");
    getchar();

}

void save_data(Data *data, Koordinat *pasien, Koordinat *player) {
    char playerFile[50];
    snprintf(playerFile, sizeof(playerFile), "%s.csv", data->name);

    FILE *fpPlayer = fopen(playerFile, "w");
    if (!fpPlayer) {
        printf("Gagal membuka file untuk menyimpan data pemain.\n");
        return;
    }

    fprintf(fpPlayer, "Nama,Pasien,PasienX,PasienY,PlayerX,PlayerY\n");
    fprintf(fpPlayer, "%s,%d,%d,%d,%d,%d\n", 
            data->name, 
            data->pasien, 
            pasien->x, 
            pasien->y, 
            player->x, 
            player->y);

    fclose(fpPlayer); 

    FILE *fpList = fopen("playerList.csv", "a+");
    if (!fpList) {
        printf("Gagal membuka file daftar pemain.\n");
        return;
    }

    char buffer[256];
    bool playerExists = false;

    fseek(fpList, 0, SEEK_SET);
    while (fgets(buffer, sizeof(buffer), fpList)) {
        buffer[strcspn(buffer, "\n")] = 0;
        if (strcmp(buffer, data->name) == 0) {
            playerExists = true;
            break;
        }
    }

    if (!playerExists) {
        fprintf(fpList, "%s\n", data->name);
    }

    fclose(fpList);
    printf("Data pemain berhasil disimpan.\n");
}


bool load_data(Data *data, Koordinat *pasien, Koordinat *player) {

    FILE *fpList = fopen("playerList.csv", "r");
    if (!fpList) {
        return false;
    }


    printf("Pilih pemain untuk dimuat:\n");
    char buffer[256];
    int index = 0;
    char playerNames[100][50];

    while (fgets(buffer, sizeof(buffer), fpList)) {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(playerNames[index++], buffer);
        printf("%d. %s\n", index, buffer);
    }
    fclose(fpList);

    if (index == 0) {
        printf("Tidak ada pemain yang tersedia untuk dimuat.\n");
        return false;
    }

    int choice;
    printf("Masukkan nomor pemain: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > index) {
        printf("Pilihan tidak valid.\n");
        return false;
    }

    char selectedPlayerFile[50];
    snprintf(selectedPlayerFile, sizeof(selectedPlayerFile), "%s.csv", playerNames[choice - 1]);

    FILE *fpPlayer = fopen(selectedPlayerFile, "r");
    if (!fpPlayer) {
        printf("Gagal membuka file pemain %s.\n", playerNames[choice - 1]);
        return false;
    }

    fgets(buffer, sizeof(buffer), fpPlayer);

    if (fgets(buffer, sizeof(buffer), fpPlayer)) {
        sscanf(buffer, "%[^,],%d,%d,%d,%d,%d", 
               data->name, 
               &data->pasien, 
               &pasien->x, 
               &pasien->y, 
               &player->x, 
               &player->y);

        fclose(fpPlayer);
        return true;
    }

    fclose(fpPlayer);
    printf("File tidak memiliki data valid.\n");
    return false;
}

void game(Data *data, Koordinat *pasien, Koordinat *player, Koordinat *map, bool munculkan_pasien) {
    
    if (munculkan_pasien == false) {
        do {
            pasien->x = rand() % map->x + 1;
            pasien->y = rand() % map->y + 1;
        } while (pasien->x == player->x && pasien->y == player->y);
    }

    do {


        system("cls");

        if (player->x == pasien->x && player->y == pasien->y) {
            data->pasien++;
            do {
                pasien->x = rand() % map->x + 1;
                pasien->y = rand() % map->y + 1;
            } while (pasien->x == player->x && pasien->y == player->y);
        }

        printf("\nKoordinat Dokter (@): %i, %i\n", player->x, player->y);
        printf("Koordinat Pasien (X): %i, %i\n", pasien->x, pasien->y);
        printf("Pasien Ditolong: %i\n", data->pasien);

        for (int y = 0; y <= map->y; y++) {
            for (int x = 0; x <= map->x; x++) {
                if (player->y == y && player->x == x) {
                    printf("@");
                } else if (pasien->x == x && pasien->y == y) {
                    printf("X");
                } else {
                    printf("_");
                }
            }
            printf("\n");
        }

     
        printf("\nInputan: ");
        char input = getchar();
        while (getchar() != '\n');

        
        switch (tolower(input)) {
        case 'w': if (player->y > 0) player->y--; break;
        case 's': if (player->y < map->y) player->y++; break;
        case 'a': if (player->x > 0) player->x--; break;
        case 'd': if (player->x < map->x) player->x++; break;
        case 'm': save_data(data, pasien, player); break;
        case 'p': panduan();break;
        default : break;
        }
       
        if (input == 'e'){
            printf("kembali kemenu utama ? (y/n)");
            char input = getchar();
            while (getchar() != '\n');

            if (input == 'y'){
                return;
            }
        }

    } while (true);


}

void new_game(Data *data) {
    int p_len;

    do {
        system("cls");
        printf("Nama (5-12 karakter): ");
        fgets(data->name, sizeof(data->name), stdin);
        data->name[strcspn(data->name, "\n")] = 0;
        p_len = strlen(data->name);
    } while (p_len < 5 || p_len > 12);
}

int main() {
    srand(time(0));
    int input;
    bool loadSuccess;
    

    while(1){
        Koordinat player = {10, 3};
        Koordinat pasien = {0, 0};
        Koordinat map = {20, 5};
        Data data = {"", 0};

        system("cls");
        printf("\"Game Dokter Penolong\"\n");
        printf("\n======= Menu ========\n");
        printf("1. New Game\n");
        printf("2. Load Game\n");
        printf("3. Exit\n");
        printf("\nPilih opsi: ");
        scanf("%d", &input);
        while (getchar() != '\n');

        switch (input) {
           case 1:
                system("cls");
                new_game(&data);
                loadSuccess = false;
                game(&data, &pasien, &player, &map, loadSuccess);
                break;

            case 2:
                system("cls");
                loadSuccess = load_data(&data, &pasien, &player);

                if (loadSuccess) {
                    game(&data, &pasien, &player, &map, loadSuccess);
                } else {
                    printf("Gagal memuat data. Tekan /enteer untuk kembali ke menu utama.\n");
                    getchar();
                }
                break;

            case 3:
                system("cls");
                exit(0);
                break;;

            default:
                system("cls");
                printf("Masukkan input yang valid !!\n");
                break;
        }
    }

    return 0;
}
