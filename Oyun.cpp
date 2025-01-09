#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define MAX_BOYUT 15
#define MAX_MAYIN 60

int BOYUT = 10; // Başlangıç boyutu
int MAYIN_SAYISI = 40; // Başlangıç mayın sayısı
int skor = 0; // Skor
int zorluk = 2; // Varsayılan zorluk "Orta"
clock_t start_time, end_time; // Başlangıç ve bitiş zamanı
int remaining_time = 600; // 10 dakika, normal ve zor modda

void mayin_tarlasi_yazdir(char mayin_tarlasi[15][15], char gosterim[15][15], int cursor_x, int cursor_y) {
    for (int i = 0; i < BOYUT; i++) {
        for (int j = 0; j < BOYUT; j++) {
            if (i == cursor_x && j == cursor_y) {
                printf("["); // İmleç başlangıcı
            } else {
                printf(" ");
            }

            if (gosterim[i][j] == 'B') {
                printf("F"); // Bayrak simgesi "F"
            } else if (gosterim[i][j] == 'O') {
                printf("%c", mayin_tarlasi[i][j]); // Açılmış hücre
            } else {
                printf("X"); // Kapalı hücre
            }

            if (i == cursor_x && j == cursor_y) {
                printf("]"); // İmleç bitişi
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

int komsu_mayin_sayisi(char mayin_tarlasi[15][15], int x, int y) {
    int sayi = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int nx = x + i;
            int ny = y + j;
            if (nx >= 0 && ny >= 0 && nx < BOYUT && ny < BOYUT && mayin_tarlasi[nx][ny] == '*') {
                sayi++;
            }
        }
    }
    return sayi;
}

void mayinlari_yerlestir(char mayin_tarlasi[15][15], int ilk_x, int ilk_y) {
    int x, y, i;

    srand(time(NULL));
    for (i = 0; i < MAYIN_SAYISI; i++) {
        do {
            x = rand() % BOYUT;
            y = rand() % BOYUT;
        } while (mayin_tarlasi[x][y] == '*' || (abs(x - ilk_x) <= 1 && abs(y - ilk_y) <= 1));
        mayin_tarlasi[x][y] = '*';
    }

    for (i = 0; i < BOYUT; i++) {
        for (int j = 0; j < BOYUT; j++) {
            if (mayin_tarlasi[i][j] != '*') {
                int sayi = komsu_mayin_sayisi(mayin_tarlasi, i, j);
                if (sayi > 0) {
                    mayin_tarlasi[i][j] = '0' + sayi;
                } else {
                    mayin_tarlasi[i][j] = '0';
                }
            }
        }
    }
}

void kutu_ac(char mayin_tarlasi[15][15], char gosterim[15][15], int x, int y) {
    if (gosterim[x][y] == 'O' || gosterim[x][y] == 'B') return;

    gosterim[x][y] = 'O';

    if (mayin_tarlasi[x][y] == '*') {
        printf("\nMayina bastiniz! Oyun bitti.\n");
        if (zorluk != 1) { // Kolay modda skor gösterilmez
            printf("Skorunuz: 0\n"); // Mayına basıldığında skor 0 olur
        }
        exit(0); // Mayına basıldığında oyunu sonlandır
    }

    if (mayin_tarlasi[x][y] == '0') {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                int nx = x + i;
                int ny = y + j;
                if (nx >= 0 && ny >= 0 && nx < BOYUT && ny < BOYUT && gosterim[nx][ny] != 'O') {
                    kutu_ac(mayin_tarlasi, gosterim, nx, ny);
                }
            }
        }
    }
}

void bayrak_ekle_kaldir(char gosterim[15][15], int x, int y) {
    if (gosterim[x][y] == 'X') {
        gosterim[x][y] = 'B'; // Bayrak ekle
    } else if (gosterim[x][y] == 'B') {
        gosterim[x][y] = 'X'; // Bayrağı kaldır
    }
}

void zorluk_sec() {
    printf("\nOyun Zorlugu: \t \n 1 - Kolay,(pratik modda denebilir sure ve skor yoktur) \t \n 2 - Orta,(default) \t \n 3 - Zor\n");
    printf("oyun zorlugunu sayi belirterek secin: ");
    scanf("%d", &zorluk);

    // Zorluk seviyesine göre mayin sayisi ve alan boyutu belirle
    if (zorluk == 1) {
        BOYUT = 8;
        MAYIN_SAYISI = 10;
    } else if (zorluk == 2) {
        BOYUT = 10;
        MAYIN_SAYISI = 40;
    } else if (zorluk == 3) {
        BOYUT = 13;
        MAYIN_SAYISI = 60;
    }
}

void oyun_tanitimi() {
    printf("\nMayin Tarlasi Oyunu\n");
    printf("Amac: Ekrandaki mayinlara basmadan tum kutulari acmak.\n");
    printf("Oyun boyunca, her adimda kutulari acarak ve sayilari kullanarak(sayilar etrafindaki mayin sayisini temsil eder) etraftaki mayinlari bulmaya calisin.\n");
    printf("Bayraklar ekleyerek mayinlari belirtin, kutulari acarak mayinlari cikarmaktan kacinin.\n");
    printf("mayinlar haric tum kutuları actiginizda kazanirsiniz ve oyununuz sona erer.\n");
    printf(" A,S,W,D tuşlarini kullanarak hareket edin,Enter tusuna basarak kutu secin,F tusuna basarak bayrak dikebilirsiniz.\n");
    printf("Baslamak icin herhangi bir tusa basin...\n");
    getch(); // Kullanıcıdan bir tuş girişi bekliyoruz
}

int main() {
    char mayin_tarlasi[15][15];
    char gosterim[15][15]; // Burada gosterim tanımlanmalıydı
    int cursor_x = 0, cursor_y = 0;
    char tus;
    int ilk_hamle = 1;

    // Oyun başlamadan önce zorluk seviyesi seç
    zorluk_sec();

    // Oyun tanıtımı
    oyun_tanitimi();

    // Mayın tarlasını ve gösterim alanını sıfırlama
    for (int i = 0; i < BOYUT; i++) {
        for (int j = 0; j < BOYUT; j++) {
            gosterim[i][j] = 'X'; // X, kapalı kutu
            mayin_tarlasi[i][j] = 'X'; // Mayın tarlasını başlat
        }
    }

    printf("\nMayin Tarlasi Oyunu!\n");
    printf("Boyut: %dx%d, Mayin Sayisi: %d\n", BOYUT, BOYUT, MAYIN_SAYISI);

    // Zaman başlatma
    if (zorluk != 1) {
        start_time = clock();
    }

    while (1) {
        system("cls"); //  ekran temizleme
        printf("\nMayin Tarlasi:\n");
        mayin_tarlasi_yazdir(mayin_tarlasi, gosterim, cursor_x, cursor_y);

        tus = getch();
        if (tus == 'q') {
            printf("\nOyunu kapattiniz. Gule Gule!\n");
            break;
        }

        switch (tus) {
            case 'w': if (cursor_x > 0) cursor_x--; break;
            case 's': if (cursor_x < BOYUT - 1) cursor_x++; break;
            case 'a': if (cursor_y > 0) cursor_y--; break;
            case 'd': if (cursor_y < BOYUT - 1) cursor_y++; break;
            case '\r': // Enter tuşu
                if (ilk_hamle) {
                    mayinlari_yerlestir(mayin_tarlasi, cursor_x, cursor_y);
                    ilk_hamle = 0;
                }
                kutu_ac(mayin_tarlasi, gosterim, cursor_x, cursor_y);
                break;
            case 'f': // Bayrak ekle/kaldır
                bayrak_ekle_kaldir(gosterim, cursor_x, cursor_y);
                break;
        }

        // Süreyi güncelle (sadece normal ve zor modda)
        if (zorluk != 1) {
            end_time = clock();
            double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
            remaining_time = 600 - (int)elapsed_time;

            if (remaining_time <= 0) {
                printf("\nZamaniniz bitti! Oyun sona erdi.\n");
                printf("Skorunuz: 0\n"); // Zaman bitince skor 0 yapılır
                break;
            }
        }

        int tum_acik = 1;
        for (int i = 0; i < BOYUT; i++) {
            for (int j = 0; j < BOYUT; j++) {
                if (mayin_tarlasi[i][j] != '*' && gosterim[i][j] != 'O') {
                    tum_acik = 0;
                    break;
                }
            }
        }

        if (tum_acik) {
            // Zamanı hesapla
            if (zorluk != 1) {
                end_time = clock();
                double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
                skor = remaining_time * 1; // Kalan zaman kadar puan kazanılır
                printf("\nTebrikler, oyunu kazandiniz!\n");
                printf("Skorunuz: %d\n", skor);
            } else {
                printf("\nTebrikler, oyunu kazandiniz!\n");
            }
            break;
        }
    }

    return 0;
}
