#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_POINTS 100
#define UNDEFINED -1
#define UNVISITED -2

// Nokta yap�s� x ve y koordinatlar�n� i�erir.
struct Nokta {
    double x, y;
};

// Nokta dizisi ve nokta say�s�n� tutan de�i�kenler tan�mlan�r.
struct Nokta noktalar[MAX_POINTS];
int noktaSayisi;

// Kullan�c�dan nokta bilgilerini alan fonksiyon
struct Nokta kullanicidanNoktaAl() {
    struct Nokta n;
    printf("x koordinatini girin: ");
    scanf("%lf", &n.x);
    printf("y koordinatini girin: ");
    scanf("%lf", &n.y);
    return n;
}

// �ki nokta aras�ndaki Euclidean uzakl���n� hesaplayan fonksiyon
double uzaklikHesapla(struct Nokta n1, struct Nokta n2) {
    return sqrt(pow(n1.x - n2.x, 2) + pow(n1.y - n2.y, 2));
}

// Geni�leme s�ras�n� uygulayan yard�mc� fonksiyon
void genislemeSirasi(int index, int kumeID, double epsilon, int minPts, int kumeIDs[], double ulasilabilirlikUzakliklari[]) {
    int i;
    for (i = 0; i < noktaSayisi; i++) {
        if (ulasilabilirlikUzakliklari[i] <= epsilon && kumeIDs[i] == UNDEFINED) {
            kumeIDs[i] = kumeID;
            if (i != index)
                genislemeSirasi(i, kumeID, epsilon, minPts, kumeIDs, ulasilabilirlikUzakliklari);
        }
    }
}

// OPTICS algoritmas�n� uygulayan ana fonksiyon
void optics(double epsilon, int minPts, int kumeIDs[], double ulasilabilirlikUzakliklari[]) {
    int i, j;
    int kumeID = 0;

    for (i = 0; i < noktaSayisi; i++) {
        if (kumeIDs[i] == UNDEFINED) {
            kumeIDs[i] = kumeID;

            // Her noktan�n kom�u uzakl�klar�n� hesaplamak i�in bellek ayr�l�r
            double* komsuUzakliklar = malloc(noktaSayisi * sizeof(double));
            for (j = 0; j < noktaSayisi; j++) {
                if (i != j)
                    komsuUzakliklar[j] = uzaklikHesapla(noktalar[i], noktalar[j]);
                else
                    komsuUzakliklar[j] = INFINITY;
            }

            int cekirdekUzaklikIndeksi = -1;
            double cekirdekUzaklik = INFINITY;

            // �ekirdek uzakl��� ve indeksi belirleme
            for (j = 0; j < noktaSayisi; j++) {
                if (komsuUzakliklar[j] <= epsilon) {
                    int komsuSayisi = 0;
                    int k;

                    // Belirlenen epsilon i�indeki kom�u noktalar�n say�s�n� hesapla
                    for (k = 0; k < noktaSayisi; k++) {
                        if (komsuUzakliklar[k] <= epsilon)
                            komsuSayisi++;
                    }

                    // MinPts ko�ulunu sa�layan ve en yak�n noktay� belirleme
                    if (komsuSayisi >= minPts && komsuUzakliklar[j] < cekirdekUzaklik) {
                        cekirdekUzaklik = komsuUzakliklar[j];
                        cekirdekUzaklikIndeksi = j;
                    }
                }
            }

            // �ekirdek nokta varsa geni�leme s�ras�n� uygulama
            if (cekirdekUzaklikIndeksi != -1) {
                genislemeSirasi(i, kumeID, epsilon, minPts, kumeIDs, ulasilabilirlikUzakliklari);
                for (j = 0; j < noktaSayisi; j++) {
                    if (kumeIDs[j] == kumeID && komsuUzakliklar[j] > cekirdekUzaklik)
                        ulasilabilirlikUzakliklari[j] = fmax(cekirdekUzaklik, komsuUzakliklar[j]);
                }
                kumeID++;
            }

            // Bellek temizleme
            free(komsuUzakliklar);
        }
    }
}

int main() {
    int i;

    printf("Nokta sayisini girin: ");
    scanf("%d", &noktaSayisi);

    printf("Noktalarin koordinatlarini girin:\n");
    for (i = 0; i < noktaSayisi; i++) {
        printf("Nokta %d:\n", i + 1);
        noktalar[i] = kullanicidanNoktaAl();
    }

    double epsilon;
    int minPts;
    int kumeIDs[MAX_POINTS];
    double ulasilabilirlikUzakliklari[MAX_POINTS];

    printf("Epsilon degerini girin: ");
    scanf("%lf", &epsilon);
    printf("minPts degerini girin: ");
    scanf("%d", &minPts);

    // K�me ID'leri ve ula��labilirlik uzakl�klar�n� ba�lang�� de�erleriyle ayarla
    for (i = 0; i < noktaSayisi; i++) {
        kumeIDs[i] = UNDEFINED;
        ulasilabilirlikUzakliklari[i] = INFINITY;
    }

    // OPTICS algoritmas�n� uygula
    optics(epsilon, minPts, kumeIDs, ulasilabilirlikUzakliklari);

    // K�me atamalar�n� ekrana yazd�r
    printf("Kume Atamalari:\n");
    for (i = 0; i < noktaSayisi; i++) {
        printf("Nokta %d -> Kume %d\n", i + 1, kumeIDs[i]);
    }

    return 0;
}
