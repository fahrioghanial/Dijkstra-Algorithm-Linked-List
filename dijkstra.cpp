/*
Nama                : Mohamad Fahrio Ghanial Fatihah 
NPM                 : 140810190005
Kelas               : A
Deskripsi Program   : Program ini menghitung bobot terkecil yang dapat ditempuh antara dua node menggunakan algoritma dijkstra
*/

#include <iostream>
#include <limits>
using namespace std;
#define INF 999 // mendefinisikan 999 sebagai INF (infinity/tak hingga)
#define NA 888 // mendefinisikan 888 sebagai NA (informasi kosong/tidak diperlukan update)

void printEqualSign(int n){ // fungsi untuk print karakter "~" secara berulang ulang
    for (int i=0; i<n; i++){ // looping sesuai jumlah n
        cout<<"~"; // menampilkan karakter "~"
    }
    cout<<endl; // garis baru
}

void fail(){ // fungsi untuk error handling
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// multi linked list untuk menyimpan graph (node parent untuk menyimpan node awal dan node child untuk menyimpan node tujuan)

struct Tujuan{ // record untuk node child
    int bobot; // untuk menyimpan bobot edge antara dua node
    int jarak; // untuk menyimpan jarak antara dua node
    int macet; // untuk menyimpan tingkat kemacetan antara dua node
    int nodeTujuan; // node tujuan disimpan sebagai node child
    Tujuan* next; // linked list sebagai pengait node nujuan yang lain
};

struct Awal{ // record untuk node parent
    int node; // node awal disimpan sebagai node parent
    Tujuan* tujuan; // pointer of struct "Tujuan" untuk menghubungkan node parent dengan node child
    Awal* next; // linked list untuk pengait node awal yang lain
};

// singly linked list untuk menyimpan kelengkapan algoritma djikstra

struct utility{ // record untuk menyimpan kelengkapan yang dibutuhkan pada algoritma dijkstra
    int index; // untuk menyimpan indeks
    int Buffer; // untuk menyimpan jarak terdekat sementara dari node awal
    int prevNode; // untuk menyimpan node-node yang telah dilalui 
    int path; // untuk menyimpan node-node yang telah dilalui
    int terkunjungi; // untuk menandai node yang telah dikunjungi
    utility* next; // linked list sebagai pengait utility index berikutnya
};

typedef Awal* pointerAwal; // alias dari pointer of struct "Awal"
typedef Tujuan* pointerTujuan; // alias dari pointer of struct "Tujuan"
typedef pointerAwal List; // alias dari pointerAwal untuk multi linked list
typedef utility* pointerUtility; // alias dari pointer of struct "utility"
typedef pointerUtility Listutility; // alias dari pointerUtility untuk singly linked list

void createList(List& head, Listutility& hUty){ // fungsi untuk membuat multi linked list graph dan singly linked list kelengkapan
    head=NULL; // deklarasi head dari multi linked list = NULL
    hUty=NULL; // deklarasi head dari list utility = NULL
}

void searchNode(List head, int key, int& status, pointerAwal& pCari){ // fungsi untuk mencari node awal (node parent)
    status = 0; // untuk menandakan status
    pCari = head; // pCari menunjuk pada head
    while(status == 0 && pCari!=NULL){ // looping jika node masih belum ditemukan
        if(pCari->node == key){ // jika node ditemukan maka looping akan berhenti dan pCari akan menunjuk pada node yang dicari
            status = 1; // setelah node ditemukan maka status akan berubah menjadi 1
        }
        else{ // jika node belum ditemukan pCari akan bergeser ke node setelahnya
            pCari=pCari->next;
        }
    }
}

void searchTujuan(List head, int key, int& status, pointerAwal pCari1, pointerTujuan& pCari2){ // fungsi untuk mencari node tujuan (node child)
    status = 0; // untuk menandakan status
    pCari2 = pCari1->tujuan; // pCari2 menunjuk pada node child dari pCari1
    while(status == 0 && pCari2!=NULL){ // looping jika node masih belum ditemukan
        if(pCari2->nodeTujuan == key){ // jika node ditemukan maka looping akan berhenti dan pCari akan menunjuk pada node yang dicari
            status = 1; // setelah node ditemukan maka status akan berubah menjadi 1
        }
        else{ // jika node belum ditemukan, pCari2 akan bergeser ke node setelahnya
            pCari2=pCari2->next; 
        }
    }
}

void searchutility(Listutility hUty, int index, int& status, pointerUtility& pCari){ // fungsi untuk mencari node utility/kelengkapan
    status = 0; // untuk menandakan status
    pCari = hUty; // pCari menunjuk pada head dari list utility
    while(status == 0 && pCari!=NULL){ // looping jika node masih belum ditemukan
        if(pCari->index == index){ // jika node ditemukan maka looping akan berhenti dan pCari akan menunjuk pada node yang dicari
            status = 1; // setelah node ditemukan maka status akan berubah menjadi 1
        }
        else{ // jika node belum ditemukan, pCari akan bergeser ke node setelahnya
            pCari=pCari->next;
        }
    }
}

void createAndUpdateListUtility(Listutility& hUty, int index, int Buffer, int prevNode, int path, int terkunjungi){ // fungsi untuk membuat dan update list utility/kelengkapan
    pointerUtility nUty, tmp, pCari; // deklari pointerUtility
    nUty = new utility; // deklarasi nUty sebagai utility baru
    int status; // deklarasi status
    searchutility(hUty, index, status, pCari); // mencari node utility sesuai dengan indexnya
    if(status!=1){ // jika node tidak ditemukan maka node baru akan dibuat, diberi indeks sesuai dengan indeks yang diminta, dan dimasukkan ke dalam list
        nUty->index = index; // mengisi nilai index
        nUty->Buffer= Buffer; // mengisi nilai Buffer
        nUty->prevNode= prevNode; // mengisi nilai prevNode
        nUty->path= path; // mengiisi nilai path
        nUty->terkunjungi= terkunjungi; // mengisi nilai terkunjungi
        nUty->next = NULL; // menyatakan next dari node adalah NULL
        // insert last (node akan dimasukkan diurutan terakhir)
        if(hUty == NULL){ // Jika list kosong
            hUty = nUty;
        }
        else{ // Jika list berisi
            tmp = hUty;
            while (tmp->next!=NULL){ // pointer tmp akan mencari node yang terakhir
                tmp=tmp->next;
            }
            tmp->next=nUty; // node baru dimasukkan di urutan terakhir
        }
    }
    else { // jika node dengan indeks yang diminta ditemukan maka akan dilakukan update terhadap node tersebut
        if(Buffer!=NA){ // jika buffer diisi NA maka nilai buffer tidak akan diupdate
            pCari->Buffer = Buffer; // nilai buffer diupdate
        }
        if(prevNode!=NA){ // jika prevNode diisi NA maka nilai prevNode tidak akan diupdate
            pCari->prevNode = prevNode; // nilai prevNode diupdate
        }
        if(path!=NA){ // jika path diisi NA maka nilai path tidak akan diupdate
            pCari->path = path; // nilai path diupdate
        }
        if(terkunjungi!=NA){ // jika terkunjungi diisi NA maka nilai terkunjungi tidak akan diupdate
            pCari->terkunjungi = terkunjungi; // nilai terkunjungi diupdate
        }
    }
}

void createAndUpdateAwalAndEdge(List& head, int node, int nodeTujuan, int jarak, int macet){ // fungsi untuk membuat dan update multi linked list
    pointerAwal nodeBaru, tmp, pCari; // declare nodeBaru, tmp, pCari
    pointerTujuan TujuanBaru, pCari2; // declare TujuanBaru, pCari2
    nodeBaru = new Awal; // declare nodeBaru sebagai Awal baru
    TujuanBaru = new Tujuan; // declare Tujuanbaru sebagai Tujuan baru
    int status1, status2; // declare status1, status2
    searchNode(head, node, status1, pCari); // mencari node awal (node parent) 
    if(status1!=1){ // jika node awal tidak ditemukan maka akan dibuat node awal yang baru (node parent baru) dan dimasukkan ke dalam list parent
        nodeBaru->node = node; // mengisi nilai/nama dari node awal (node parent)
        nodeBaru->tujuan = NULL; // menyatakan node child (node tujuan) adalah NULL
        nodeBaru->next = NULL; // menyatakan next dari node adalah NULL
        // insert last untuk node parent
        if(head == NULL){ //Jika list kosong
            head = nodeBaru;
        }
        else{ // Jika list berisi
            tmp = head;
            while (tmp->next!=NULL){ // tmp akan mencari node parent terakhir
                tmp = tmp->next;
            }
            tmp->next = nodeBaru; // node parent baru dimasukkan di urutan terakhir
        }
        searchNode(head, node, status1, pCari); // setelah node baru dibuat maka pCari akan menunjuk ke node tersebut
    }
    // jika node awal ditemukan maka pCari akan langsung menunjuk node tersebut
    searchTujuan(head, nodeTujuan, status2, pCari, pCari2); // mencari node tujuan (node child)
    if (status2!=1){ // jika node tujuan tidak ditemukan maka akan dibuat node tujuan yang baru (node child baru) dan dimasukkan ke dalam list child
        TujuanBaru->jarak = INF; // mengisi nilai jarak menjadi tak hingga
        TujuanBaru->macet = INF; // mengisi nilai macet menjadi tak hingga
        TujuanBaru->bobot = INF; // mengisi nilai bobot menjadi tak hingga
        TujuanBaru->nodeTujuan = nodeTujuan; // mengisi nilai/nama dari node tujuan (node child)
        TujuanBaru->next = NULL; // menyatakan next dari node adalah NULL
        // insert last untuk node child
        if(pCari->tujuan==NULL){ //Jika tidak terdapat node child
            pCari->tujuan = TujuanBaru;
        }
        else{  //Jika terdapat node child
            pointerTujuan tmp2; // deklarasi pointer tmp2
            tmp2 = pCari->tujuan; // tmp2 menunjuk node child dari pCari
            while(tmp2->next!=NULL){ // tmp akan mencari node parent terakhir
                tmp2 = tmp2->next;
            }
            tmp2->next = TujuanBaru; // node child baru dimasukkan di urutan terakhir
        }
    }
    // jika node tujuan ditemukan maka pCari2 akan langsung menunjuk node tersebut
    else{ // jika node parent dan child ditemukan maka akan dilakukan update terhadap nilai jarak, tingkat kemacetan, dan bobot 
        pCari2->jarak = jarak; // update nilai jarak 
        pCari2->macet = macet; // update nilai tingkat kemacetan
        pCari2->bobot = jarak*macet; // update nilai bobot menjadi perkalian jarak dengan tingkat kemacetan
    }
}

void createGraph(List& head, Listutility& utility){ // fungsi untuk membuat graph
    // membuat list utility dengan setiap indeksnya disii :
    // buffer = INF, prevNode = -1, path = -1, terkunjungi = 0
    for(int i = 0; i < 12; i++) { // looping untuk membuat 12 node utility dan 12 node parent(awal)
        createAndUpdateListUtility(utility, i, INF, -1, -1, 0); // memanggil fungsi pembuat list utility
        for(int j=0; j<12; j++){ // looping untuk membuat 12 node child(tujuan) dengan nilai jarak dan macetnya adalah tak hingga (INF)
            createAndUpdateAwalAndEdge(head, i, j, INF, INF); // memanggil fungsi pembuat multi linked list graph
        }
    }
    // update nilai jarak dan tingkat kemacetan sesuai graph yang akan dibuat
    createAndUpdateAwalAndEdge(head, 0, 1, 6, 4);  // jalur dari node 0 ke node 1 memiliki jarak = 6 dan tingkat kemacetan = 4
    createAndUpdateAwalAndEdge(head, 0, 5, 4, 4);  // jalur dari node 0 ke node 5 memiliki jarak = 4 dan tingkat kemacetan = 4
    createAndUpdateAwalAndEdge(head, 1, 2, 8, 3);  // jalur dari node 1 ke node 2 memiliki jarak = 8 dan tingkat kemacetan = 3
    createAndUpdateAwalAndEdge(head, 1, 5, 9, 3);  // jalur dari node 1 ke node 5 memiliki jarak = 9 dan tingkat kemacetan = 3
    createAndUpdateAwalAndEdge(head, 2, 3, 7, 2);  // jalur dari node 2 ke node 3 memiliki jarak = 7 dan tingkat kemacetan = 2
    createAndUpdateAwalAndEdge(head, 3, 4, 5, 4);  // jalur dari node 3 ke node 4 memiliki jarak = 5 dan tingkat kemacetan = 4
    createAndUpdateAwalAndEdge(head, 4, 9, 10, 2); // jalur dari node 4 ke node 9 memiliki jarak = 10 dan tingkat kemacetan = 2
    createAndUpdateAwalAndEdge(head, 5, 8, 4, 1);  // jalur dari node 5 ke node 8 memiliki jarak = 4 dan tingkat kemacetan = 1
    createAndUpdateAwalAndEdge(head, 6, 2, 4, 4);  // jalur dari node 6 ke node 2 memiliki jarak = 4 dan tingkat kemacetan = 4
    createAndUpdateAwalAndEdge(head, 6, 8, 6, 2);  // jalur dari node 6 ke node 8 memiliki jarak = 6 dan tingkat kemacetan = 2
    createAndUpdateAwalAndEdge(head, 7, 0, 10, 1); // jalur dari node 7 ke node 0 memiliki jarak = 10 dan tingkat kemacetan = 1
    createAndUpdateAwalAndEdge(head, 7, 10, 5, 3); // jalur dari node 7 ke node 10 memiliki jarak = 5 dan tingkat kemacetan = 3
    createAndUpdateAwalAndEdge(head, 8, 2, 8, 2);  // jalur dari node 8 ke node 2 memiliki jarak = 8 dan tingkat kemacetan = 2
    createAndUpdateAwalAndEdge(head, 8, 7, 7, 2);  // jalur dari node 8 ke node 7 memiliki jarak = 7 dan tingkat kemacetan = 2
    createAndUpdateAwalAndEdge(head, 8, 9, 7, 3);  // jalur dari node 8 ke node 9 memiliki jarak = 7 dan tingkat kemacetan = 3
    createAndUpdateAwalAndEdge(head, 9, 6, 5, 3);  // jalur dari node 9 ke node 6 memiliki jarak = 5 dan tingkat kemacetan = 3
    createAndUpdateAwalAndEdge(head, 9, 11, 6, 1); // jalur dari node 9 ke node 11 memiliki jarak = 6 dan tingkat kemacetan = 1
    createAndUpdateAwalAndEdge(head, 10, 8, 6, 1); // jalur dari node 10 ke node 8 memiliki jarak = 6 dan tingkat kemacetan = 1
    createAndUpdateAwalAndEdge(head, 11, 8, 7, 2); // jalur dari node 11 ke node 8 memiliki jarak = 7 dan tingkat kemacetan = 2
}

// algoritma dijkstra untuk mencari jalur tercepat (jalur yang memiliki nilai bobot terkecil)
void dijkstra(int& start, List head, Listutility& utility, int& bobot_terkecil, int awal, int tujuan){ // fungsi untuk algoritma dijkstra
    int m, update, status, statusUty; // declare m, update, status, statusUty
    pointerAwal pCari; // declare pCari
    pointerTujuan Tujuan; // declare Tujuan
    pointerUtility pUty, pVisit; // declare pUty, pVisit
    searchNode(head, awal, status, pCari); // pCari akan menunjuk node awal
    start = awal; // start = awal
    createAndUpdateListUtility(utility, start, 0, NA, NA, 1); // update node utility yang berindeks start dengan rincian : buffer = 0, terkunjungi = 1
    searchutility(utility, tujuan, statusUty, pVisit); // pVisit akan menunjuk ke node utility yang berindeks tujuan
    while(pVisit->terkunjungi == 0) { // looping hingga node tujuan terkunjungi
        Tujuan = pCari->tujuan; // deklarasi Tujuan sebagai node child dari pCari
        bobot_terkecil = INF; // deklarasi bobot_terkecil (bobot terkecil dari node start) = INF
        m = 0; // deklarasi m = 0 (m adalah penanda node yang akan dikunjungi selanjutnya)
        for(int i=0;i<12;i++) { // looping untuk mencari bobot terkecil
            searchutility(utility, start, statusUty, pUty); // pUty akan menunjuk node utility yang berindeks start
            update = pUty->Buffer + Tujuan->bobot; // update = nilai buffer + bobot node start ke node i, nilai buffer adalah bobot terkecil dari node awal ke node start
            searchutility(utility, i, statusUty, pUty); // pUty akan menuntuk node utility yang berindeks i
            if(update < pUty->Buffer && pUty->terkunjungi==0) { // jika nilai update < buffer dan node i belum terkunjungi
                createAndUpdateListUtility(utility, i, update, start, NA, NA); // update nilai buffer = update karena ditemukan bobot yang lebih kecil, update pula prevNodenya = start
            }
            if(bobot_terkecil > pUty->Buffer && pUty->terkunjungi==0) { // jika nilai bobot_terkecil > buffer dan node i belum terkunjungi
                bobot_terkecil = pUty->Buffer; // bobot_terkecil = buffer
                m = i; // tandai node yang memiliki bobot terkecil
            }
            Tujuan=Tujuan->next; // pointer tujuan akan menunjuk ke node setelahnya
        } 
        start = m; // start akan berpindah ke node yang berbobot paling kecil darinya
        searchNode(head, start, status, pCari); // pCari akan menunjuk pada node start
        createAndUpdateListUtility(utility, start, NA, NA, NA, 1); // update nilai terkunjungi yang berindeks start menjadi 1 (menandakan bahwa node yang berindeks start telah dikunjungi)
    }
    start = tujuan; // nilai start = tujuan (untuk diisikan pada path)
}

void printPath(int start, Listutility& utility, int bobot_terkecil){ // fungsi untuk mencetak node yang dilalui
    pointerUtility pUty; // deklarasi pUty
    int status; // deklarasi status
    int j=0; // deklarasi j=0
    while(start != -1) { // looping hingga seluruh jalur yang telah dilalui dimasukkan ke dalam path
        createAndUpdateListUtility(utility, j, NA, NA, start, NA); // update node utility yang berindeks j dengan rincian : nilai path = nilai yang ada di start
        searchutility(utility, start, status, pUty); // pUty menunjuk ke node utility yang berindeks start
        start = pUty->prevNode; // start = nilai prevNode dari start
        j++; // increment j
    }
    cout << "\nBobot terkecil yang dilalui adalah : " << bobot_terkecil << "\n"; // menampilkan bobot terkecil
    cout << "Jalur yang ditempuh adalah         : "; // menampilkan jalur yang ditempuh
    for (int i = 11; i >= 0; i--) { // loop untuk menampilkan jalur (nilai yang ada di path)
        searchutility(utility, i, status, pUty); // puty menunjuk ke node utility yang berindeks i
        if (pUty->path != -1) { // tampilkan nilai path (jika nilai dalam path adalah -1 maka tidak akan ditampilkan)
            if(i==0){ // jika node yang ditampilkan merupakan node terakhir maka panah " -> " tidak akan ditampilkan
                cout<<pUty->path;
            }
            else cout << pUty->path << " -> "; // menampilkan node yang dilalui beserta tanda panah " -> "
        }
    } cout << "\n\n"; // garis baru
}

void traversal(List head){ // fungsi untuk menampilkan graph
    pointerAwal tmp; // deklarasi tmp
    pointerTujuan tmp2; // deklarasi tmp2
    system("cls"); // clear screen
    printEqualSign(40); // print karakter "~" sebanyak 40 kali
    cout << "              Daftar Tujuan" << endl; // menampilkan kalimat "Daftar Tujuan"
    printEqualSign(40); // print karakter "~" sebanyak 40 kali
    tmp=head; // pointer tmp menunjuk pada head dari node parent
    while (tmp!=NULL){ // looping hingga seluruh node parent ditampilkan
        tmp2=tmp->tujuan; // pointer tmp2 menunjuk pada node child dari tmp
        while (tmp2!=NULL){ // looping hingga seluruh node child ditampilkan
            if(tmp2->bobot!=INF){ // jika bobotnya tak hingga maka tak akan ditampilkan (bobot tak hingga = tidak ada jalur langsung antara kedua node tersebut)
                printEqualSign(40); // print karakter "~" sebanyak 40 kali
                cout << "Node Awal           : " << tmp->node << endl; // menampilkan node awal
                cout << "Node Tujuan         : " << tmp2->nodeTujuan << endl; // menampilkan node tujuan
                cout << "Jarak               : " << tmp2->jarak << endl; // menampilkan jarak
                cout << "Tingkat Kemacetan   : " << tmp2->macet << endl; // menampilkan tingkat kemacetan
                cout << "Bobot Jalur (Edge)  : " << tmp2->bobot << endl; // menampilkan bobot
            }
            tmp2=tmp2->next; // tmp2 menunjuk ke node setelahnya
        }
        tmp=tmp->next; // tmp menunjuk ke node setelahnya
    }
    printEqualSign(40); // print karakter "~" sebanyak 40 kali
}


enum enumMenu{ // enum untuk menu
    TAMPILKAN=1, // tampilkan = 1
    MULAI, // mulai = 2
    KELUAR // keluar = 3
};

void menu(List list, Listutility utility){ // fungsi untuk menampilkan menu
    int pilihan, awal, tujuan, start, bobot_terkecil; // deklarasi pilihan, awal, tujuan, start, bobot_terkecil
    while (true){ // looping akan berhenti jika user memilih keluar program
        system("cls"); // clear screen
        createList(list, utility); // memanggil fungsi untuk membuat list
        createGraph(list,  utility); // memanggil fungsi untuk membuat graph
        printEqualSign(50); // print karakter "~" sebanyak 50 kali
        cout << "               Program Shortest Path" << endl; // menampilkan kalimat "Program Shortest Path"
        printEqualSign(50); // print karakter "~" sebanyak 50 kali
        cout << "Menu\n" // menampilkan menu
                "1. Tampilkan Seluruh Jalur\n" // menu 1
                "2. Mulai Perjalanan\n" // menu 2
                "3. Keluar Program\n"; // menu 3
        printEqualSign(50); // print karakter "~" sebanyak 50 kali
        cout << "Pilih Menu : "; // menampilkan kalimat "Pilih Menu : "
        cin >> pilihan; // user memilih menu
        switch(pilihan){ // switch berdasarkan pilihan user
            case TAMPILKAN: // jika user memilih menu 1
                traversal(list); // memanggil fungsi traversal untuk menampilkan seluruh jalur yang ada di graph
                break;
            case MULAI: // jika user memilih menu 2
                system("cls"); // clear screen
                printEqualSign(40); // print karakter "~" sebanyak 40 kali
                cout << "            Mulai Perjalanan" << endl; // menampilkan kalimat "Mulai Perjalanan"
                printEqualSign(40); // print karakter "~" sebanyak 40 kali
                cout << "Pilih Node Awal (0-11)   : "; cin >> awal; // menampilkan kalimat "Pilih Node Awal" dan user memilih node awal
                cout << "Pilih Node Tujuan (0-11) : "; cin >> tujuan; // menampilkan kalimat "Pilih Node Tujuan" dan user memilih node tujuan
                if (cin.fail()){ // error handling jika user menginput selain integer
                    fail();
                    cout<<"\nInput Salah!\n\n"; // menampilkan kalimat "Input Salah!"
                }
                else if(awal>11 || tujuan>11){ // error handling jika user menginput nilai selain 0-11
                    cout<<"Node yang anda pilih tidak terdapat pada graph!\n\n"; // menampilkan kalimat "Node yang anda pilih tidak terdapat pada graph!"
                }
                else { // jika user menginput nilai antara 0-11
                    dijkstra(start, list,  utility, bobot_terkecil, awal, tujuan); // memanggil fungsi dijkstra untuk algoritma dijkstra
                    printPath(start, utility, bobot_terkecil); // memanggil fungsi printPath untuk menampilkan bobot terkecil dan jalur yang dilaluinya
                }
                break;
            case KELUAR: // jika user memilih menu 3
                cout << "~~~~~~~~~~~~~~~~ Terimakasih Telah Menggunakan Program Ini ~~~~~~~~~~~~~~~~\n\n"; // menampilkan kalimat "~~~~~~~~~~~~~~~~ Terimakasih Telah Menggunakan Program Ini ~~~~~~~~~~~~~~~~"
                exit(0); // exit program
            default: // jika user memasukkan nilai yang tidak ada di menu
                if (cin.fail()){ // error handling jika user menginput selain integer
                    fail();
                }
                cout << "Input salah!\n\n"; // menampilkan kalimat "Input Salah!"
        }
        system("pause"); // system pause
    }
}

int main() { // fungsi main
    Listutility utility; // deklarasi utility (list utility)
    List list; // deklarasi list (multi linked list graph)
    menu(list, utility); // memanggil fungsi menu untuk menjalankan program shortest path
}