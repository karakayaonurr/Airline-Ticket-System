// Onur Karakaya 140201051 3.sinif 1.ogretim
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <Windows.h>
#include <mysql.h>

using namespace std;

MYSQL* baglanti;
MYSQL_ROW satir;
MYSQL_RES* deger;

string sorgu;
int Anamenu = 0;

void vt_baglan(){
    /* Veritabani ile baglanti kurulur */
    baglanti = mysql_init(0);
    baglanti = mysql_real_connect(baglanti,"localhost","root","123456789","prolab2_4",0,NULL,0);
    /* Baglanti kontrolu*/
    if(!baglanti){
        cout<<"Veritabanina baglanma problemi: "<<mysql_error(baglanti)<<endl;
    }
}

int kapasite(string ukn, string tarih){
    vt_baglan();
    int yolcu_sayisi=0,bilet_sayisi=0;
    MYSQL_ROW satir2;
    MYSQL_RES* deger2;
    string sorgu2;
    sorgu2 = "SELECT * FROM yolcu_tablosu AS yt WHERE yt.tarih='"+tarih+"' AND yt.ukn='"+ukn+"'";
    mysql_query(baglanti, sorgu2.c_str());
    deger2 = mysql_store_result(baglanti);
    while((satir2=mysql_fetch_row(deger2))!=NULL){
        yolcu_sayisi++;
    }
    sorgu2 = "SELECT ut.kapasite FROM ucak_tablosu AS ut WHERE ut.ukn = '"+ukn+"'";
    mysql_query(baglanti, sorgu2.c_str());
    deger2 = mysql_store_result(baglanti);
    while((satir2=mysql_fetch_row(deger2))!=NULL){
        bilet_sayisi = atoi(satir2[0]) - yolcu_sayisi;
    }
    return bilet_sayisi;
}

void doluluk_orani(){
    const char* s2;
    string sorgu2,tarih,ukn;
    /* Tabloyu Temizle */
    cout <<"***DOLULUK ORANLARI***\n"<< endl;
    sorgu = "DELETE FROM doluluk_tablosu";
    mysql_query(baglanti, sorgu.c_str());
    deger = mysql_store_result(baglanti);
    /* Veritabani Islemleri */
    sorgu = "SELECT st.tarih, st.ukn, ut.kapasite FROM sefer_tablosu AS st JOIN ucak_tablosu AS ut ON ut.ukn=st.ukn";
    mysql_query(baglanti, sorgu.c_str());
    deger = mysql_store_result(baglanti);
    while((satir=mysql_fetch_row(deger))!=NULL){
        tarih=satir[0];
        ukn=satir[1];
        int bilet_sayisi=kapasite(satir[1],satir[0]);
        int doluluk=(atof(satir[2])-bilet_sayisi)*100/atof(satir[2]);
        stringstream ss;
        ss << doluluk <<endl;
        string doluluk2 = ss.str();
        sorgu2 = "INSERT INTO doluluk_tablosu(doluluk,ukn,tarih) values('"+doluluk2+"','"+ukn+"','"+tarih+"')";
        s2 = sorgu2.c_str();
        mysql_query(baglanti,s2);
    }
    sorgu = "SELECT  dt.doluluk, dt.ukn, dt.tarih, ut.kapasite FROM doluluk_tablosu AS dt JOIN ucak_tablosu AS ut ON ut.ukn=dt.ukn ORDER BY dt.doluluk DESC";
    mysql_query(baglanti, sorgu.c_str());
    deger = mysql_store_result(baglanti);
    while((satir=mysql_fetch_row(deger))!=NULL){
        cout <<"UKN: "<<satir[1]<<" Tarih: "<<satir[2]<<"   Doluluk Orani: "<<satir[0]<<"   Bos Koltuk: "<<kapasite(satir[1],satir[2])<<"/"<<satir[3]<<"\n";
    }
    cout <<"\n\n0)Ana Menu\n"; cin >> Anamenu;
    system("cls"); // Ekrani temizler
}

int ukn_kontrol(string ukn){
    int kontrol = 0;
    sorgu = "SELECT ut.ukn FROM ucak_tablosu AS ut";
    mysql_query(baglanti, sorgu.c_str());
    deger = mysql_store_result(baglanti);
    while((satir=mysql_fetch_row(deger))!=NULL){
        if(ukn == satir[0]){
            kontrol = 1;
        }
    }
    if(kontrol==0){
        cout<<"\nGecersiz bir kuyruk numarasi girdiniz!!\n\n0)Ana Menu\n"; cin >> Anamenu;
    }
    return kontrol;
}

int TCKN_kontrol(string TCKN){
    int kontrol = 0;
    sorgu = "SELECT kt.TCKN FROM kisi_tablosu AS kt";
    mysql_query(baglanti, sorgu.c_str());
    deger = mysql_store_result(baglanti);
    while((satir=mysql_fetch_row(deger))!=NULL){
        if(TCKN == satir[0]){
            kontrol = 1;
        }
    }
    if(kontrol==0){
        cout<<"\nGecersiz bir TCKN girdiniz!!\n\n0)Ana Menu\n"; cin >> Anamenu;
    }
    return kontrol;
}

int tarih_kontrol(string tarih){
    int kontrol = 0;
    sorgu = "SELECT st.tarih FROM sefer_tablosu AS st";
    mysql_query(baglanti, sorgu.c_str());
    deger = mysql_store_result(baglanti);
    while((satir=mysql_fetch_row(deger))!=NULL){
        if(tarih == satir[0]){
            kontrol = 1;
        }
    }
    if(kontrol==0){
        cout<<"\nGecersiz bir tarih girdiniz!!\n\n0)Ana Menu\n"; cin >> Anamenu;
    }
    return kontrol;
}

int sehir_kontrol(string sehir){
    int kontrol = 0;
    sorgu = "SELECT st.sehir_adi FROM sehir_tablosu AS st";
    mysql_query(baglanti, sorgu.c_str());
    deger = mysql_store_result(baglanti);
    while((satir=mysql_fetch_row(deger))!=NULL){
        if(sehir == satir[0]){
            kontrol = 1;
        }
    }
    if(kontrol==0){
        cout<<"\nGecersiz bir sehir girdiniz!!\n\n0)Ana Menu\n"; cin >> Anamenu;
    }
    return kontrol;
}

void yeni_sefer(){
    /* Girdiler istenir */
    string ukn,bn,bin,tarih,saat;
    int onay;
    const char* s;
    cout <<"***SEFER TANIMLAMA***\n"<< endl;
    cout <<"Ucak kuyruk numarasi giriniz: "; cin >> ukn; if(!ukn_kontrol(ukn)){goto yeni_sefer_bitir;}
    cout <<"\nBaslangic sehirini giriniz: "; cin >> bn; if(!sehir_kontrol(bn)){goto yeni_sefer_bitir;}
    cout <<"\nBitis sehirini giriniz: "; cin >> bin; if(!sehir_kontrol(bin)){goto yeni_sefer_bitir;}
    cout <<"\nUcus tarihini (GGAAYYYY) giriniz: "; cin >> tarih;
    cout <<"\nUcus saatini (SSDD) giriniz: "; cin >> saat;
    /* Veritabani islemleri */
    sorgu = "SELECT st.plaka FROM sehir_tablosu AS st WHERE st.sehir_adi='"+bn+"'";
    mysql_query(baglanti, sorgu.c_str());
    deger = mysql_store_result(baglanti);
    while((satir=mysql_fetch_row(deger))!=NULL){
        bn = satir[0];
    }
    sorgu = "SELECT st.plaka FROM sehir_tablosu AS st WHERE st.sehir_adi='"+bin+"'";
    mysql_query(baglanti, sorgu.c_str());
    deger = mysql_store_result(baglanti);
    while((satir=mysql_fetch_row(deger))!=NULL){
        bin = satir[0];
    }
    sorgu = "INSERT INTO sefer_tablosu(ukn,bn,bin,tarih,saat) values('"+ukn+"','"+bn+"','"+bin+"','"+tarih+"','"+saat+"')";
    s = sorgu.c_str();
    onay = mysql_query(baglanti,s);
    if(!onay){
        system("cls"); // Ekrani temizler
        cout <<"***SEFER TANIMLAMA***\n"<< endl;
        cout <<"Sefer tanimlama basarili bir sekilde tamamlandi\n\n0)Ana Menu\n"; cin >> Anamenu;
        system("cls"); // Ekrani temizler
    }else{
        system("cls"); // Ekrani temizler
        cout <<"***SEFER TANIMLAMA***\n"<< endl;
        cout <<"Sefer tanimlama basarisiz oldu... Girislerinizi kontrol ediniz!\n\n0)Ana Menu\n"; cin >> Anamenu;
        system("cls"); // Ekrani temizler
    }
    yeni_sefer_bitir:
    system("cls"); // Ekrani temizler
}

void sefer_iptal(){
    /* Girdiler istenir */
    string tarih,ukn;
    cout <<"***SEFER IPTAL***\n"<< endl;
    cout <<"Ucak kuyruk numarasini: "; cin >> ukn; if(!ukn_kontrol(ukn)){goto sefer_iptal_bitir;}
    cout <<"\nUcus tarihini giriniz: "; cin >> tarih; if(!tarih_kontrol(tarih)){goto sefer_iptal_bitir;}
    /* Veritabani islemleri */
    sorgu = "DELETE FROM sefer_tablosu WHERE ukn = '"+ukn+"' AND tarih='"+tarih+"'";
    mysql_query(baglanti, sorgu.c_str());
    deger = mysql_store_result(baglanti);
    system("cls"); // Ekrani temizler
    cout <<"***SEFER IPTAL***\n"<< endl;
    cout <<"Ucagin girilen tarihteki ucusuyla ilgili bilgileri silindi\n\n0)Ana Menu\n"; cin >> Anamenu;
    sefer_iptal_bitir:
    system("cls"); // Ekrani temizler
}

void sefer_sorgu(){
    /* Girdiler istenir */
    string tarih,ukn;
    int kontrol=0;
    cout <<"***SEFER SORGULAMA***\n"<< endl;
    cout <<"Tarih giriniz: "; cin >> tarih; if(!tarih_kontrol(tarih)){goto sefer_sorgu_bitir;}
    cout <<"Ucak kuyruk numarasini giriniz: "; cin >> ukn; if(!ukn_kontrol(ukn)){goto sefer_sorgu_bitir;}
    /* Veritabani islemleri */
    sorgu = "SELECT kt.TCKN, kt.isim, kt.eposta, st.sehir_adi, kt.adres FROM yolcu_tablosu AS yt JOIN kisi_tablosu AS kt ON kt.TCKN=yt.TCKN JOIN sehir_tablosu AS st ON st.plaka=kt.sehir WHERE yt.tarih='"+tarih+"' AND yt.ukn='"+ukn+"'";
    mysql_query(baglanti, sorgu.c_str());
    deger = mysql_store_result(baglanti);
    while((satir=mysql_fetch_row(deger))!=NULL){
        cout <<"\nTCKN: "<<satir[0]<<"   Isim: "<<satir[1]<<"   e-Posta: "<<satir[2]<<"    Sehir: "<< satir[3]<<"   Adres: "<<satir[4];
        kontrol = 1;
    }
    if(kontrol == 0){
        cout<<"\nGirilen sefer bilgileri icin herhangi bir yolcu bilgisi bulunamamistir...";
    }
    cout <<"\n\n0)Ana Menu\n"; cin >> Anamenu;
    sefer_sorgu_bitir:
    system("cls"); // Ekrani temizler
}

void yolcu_sorgu(){
    /* Girdiler istenir */
    string TCKN;
    int kontrol=0;
    cout <<"***YOLCU SORGULAMA***\n"<< endl;
    cout <<"TCKN giriniz: "; cin >> TCKN; if(!TCKN_kontrol(TCKN)){goto yolcu_sorgu_bitir;}
    /* Veritabani islemleri */
    sorgu = "SELECT yt.tarih, yt.ukn, st.sehir_adi, st2.sehir_adi FROM yolcu_tablosu AS yt JOIN sehir_tablosu AS st ON yt.bn = st.plaka JOIN sehir_tablosu AS st2 ON yt.bin = st2.plaka WHERE yt.TCKN='"+TCKN+"'";
    mysql_query(baglanti, sorgu.c_str());
    deger = mysql_store_result(baglanti);
    while((satir=mysql_fetch_row(deger))!=NULL){
        cout <<"\nTarih: "<<satir[0]<<"   UKN: "<<satir[1]<<"   Baslangic: "<<satir[2]<<"    Bitis: "<< satir[3];
        kontrol = 1;
    }
    if(kontrol == 0){
        cout<<"\nGirilen TCKN'a ait ucus bilgisi bulunamamistir...";
    }
    cout <<"\n\n0)Ana Menu\n"; cin >> Anamenu;
    yolcu_sorgu_bitir:
    system("cls"); // Ekrani temizler
}

void bilet_al(){
    /* Girdiler istenir */
    string bn,bin,tarih,ukn,TCKN,isim,eposta,sehir,adres;
    int kalan_bilet,bilet_sayisi,kontrol=0,kontrol2=0;
    const char* s;
    cout <<"***BILET ALMA***\n"<< endl;
    cout <<"Baslangic sehirini giriniz: "; cin >> bn;
    cout <<"\nBitis sehirini giriniz: "; cin >> bin;
    cout <<"\nUcus icin tarih giriniz: "; cin >> tarih; if(!tarih_kontrol(tarih)){goto bilet_al_bitir;}
    cout <<"\nBilet sayisini giriniz: "; cin >> bilet_sayisi;
    /* Veritabani islemleri */
    sorgu = "SELECT st.ukn, st.saat , st.tarih FROM sefer_tablosu AS st JOIN sehir_tablosu AS st2 ON st2.plaka=st.bn JOIN sehir_tablosu AS st3 ON st3.plaka=st.bin WHERE st.tarih='"+tarih+"' AND st2.sehir_adi='"+bn+"' AND st3.sehir_adi='"+bin+"'";
    mysql_query(baglanti, sorgu.c_str());
    deger = mysql_store_result(baglanti);
    while((satir=mysql_fetch_row(deger))!=NULL){
        kalan_bilet = kapasite(satir[0],satir[2]);
        cout <<"\nUcak Kuyruk No: "<<satir[0]<<"   Saat: "<<satir[1]<<"   Kalan Bilet: "<<kalan_bilet;
        kontrol++;
    }
    if(kontrol==0){
        cout <<"\nGirilen tarihte istenilen sehire sefer bulunmamaktadir.\n\n0)Ana Menu"; cin>>Anamenu;
        goto bilet_al_bitir;
    }
    kontrol = 0;
    cout <<"\n\nUcak Kuruk Numarasi giriniz: "; cin >> ukn;
    mysql_query(baglanti, sorgu.c_str());
    deger = mysql_store_result(baglanti);
    while((satir=mysql_fetch_row(deger))!=NULL){
        if(ukn == satir[0]){
            kontrol++;
            if(bilet_sayisi > kalan_bilet){
                kontrol2++;
            }
        }
    }
    if(kontrol==0){
        cout <<"\nGirilen kuyruk numarasi listelenen seferler listesinde bulunmamaktadir.\n\n0)Ana Menu"; cin>>Anamenu;
        goto bilet_al_bitir;
    }else if(kontrol2!=0){
        cout <<"\nIstenen ucus icin yeterli bos koltuk bulunmamaktadir.\n\n0)Ana Menu"; cin>>Anamenu;
        goto bilet_al_bitir;
    }
    while(bilet_sayisi != 0){
        kontrol = 0;
        cout <<"\nTCKN giriniz: "; cin >> TCKN;
        sorgu = "SELECT kt.TCKN FROM kisi_tablosu AS kt";
        mysql_query(baglanti, sorgu.c_str());
        deger = mysql_store_result(baglanti);
        while((satir=mysql_fetch_row(deger))!=NULL){
            if(TCKN == satir[0]){
                kontrol = 1;
            }
        }
        if(kontrol==0){
            cout<<"\nSoyisim,isim giriniz: "; cin >> isim;
            cout<<"\ne-Posta giriniz: "; cin >> eposta;
            cout<<"\nSehir plaka kodu giriniz: "; cin >> sehir;
            cout<<"\nAdres giriniz: "; cin >> adres;
            sorgu = "SELECT st.plaka FROM sehir_tablosu AS st WHERE st.sehir_adi='"+sehir+"'";
            mysql_query(baglanti, sorgu.c_str());
            deger = mysql_store_result(baglanti);
            while((satir=mysql_fetch_row(deger))!=NULL){
                sehir = satir[0];
            }
            sorgu = "INSERT INTO kisi_tablosu(TCKN,isim,eposta,sehir,adres) values('"+TCKN+"','"+isim+"','"+eposta+"','"+sehir+"','"+adres+"')";
            s = sorgu.c_str();
            mysql_query(baglanti,s);
        }
        sorgu = "SELECT st.bn, st.bin FROM sefer_tablosu AS st WHERE st.ukn='"+ukn+"'";
        mysql_query(baglanti, sorgu.c_str());
        deger = mysql_store_result(baglanti);
        while((satir=mysql_fetch_row(deger))!=NULL){
            bn=satir[0];
            bin=satir[1];
        }
        sorgu = "INSERT INTO yolcu_tablosu(tarih,ukn,TCKN,bn,bin) values('"+tarih+"','"+ukn+"','"+TCKN+"','"+bn+"','"+bin+"')";
        s = sorgu.c_str();
        mysql_query(baglanti,s);
        bilet_sayisi--;
    }
    system("cls"); // Ekrani temizler
    cout <<"***BILET ALMA***\n"<< endl;
    cout <<"Bilet alma islemi basarili bir sekilde tamamlandi\n\n0)Ana Menu\n"; cin >> Anamenu;
    bilet_al_bitir:
    system("cls"); // Ekrani temizler
}

void bilet_iptal(){
    /* Girdiler istenir */
    string TCKN,tarih,ukn;
    cout <<"***BILET IPTAL***\n"<< endl;
    cout <<"Yolcu TCKN giriniz: "; cin >> TCKN;
    cout <<"\nUcus tarihini giriniz: "; cin >> tarih; if(!tarih_kontrol(tarih)){goto bilet_iptal_bitir;}
    cout <<"\nUcak kuyruk numarasini: "; cin >> ukn; if(!ukn_kontrol(ukn)){goto bilet_iptal_bitir;}
    /* Veritabani islemleri */
    sorgu = "DELETE FROM yolcu_tablosu WHERE tarih='"+tarih+"' AND ukn = '"+ukn+"' AND TCKN='"+TCKN+"'";
    mysql_query(baglanti, sorgu.c_str());
    deger = mysql_store_result(baglanti);
    system("cls"); // Ekrani temizler
    cout <<"***BILET IPTAL***\n"<< endl;
    cout <<"Kisinin girilen ucusla ilgili bilgileri silindi\n\n0)Ana Menu\n"; cin >> Anamenu;
    bilet_iptal_bitir:
    system("cls"); // Ekrani temizler
}

int main(){
    int kullanici,islem;
    string a="5.5",b="1.1";
    kullanici_tercihi:
    cout <<"***ULUSAL HAVAYOLU ACENTESI***\n"<< endl;
    cout <<"1)Yetkili Islemleri\n2)Musteri Islemleri\n\nSecim yapiniz: ";
    cin >> kullanici;
    system("cls"); // Ekrani temizler
    if(kullanici == 1){/* Yetkili islemleri */
        cout <<"***YETKILI ISLEMLERI***\n"<< endl;
        islem_tercihi1:
        cout <<"1)Yeni Sefer Tanimlama\n2)Sefer Iptali\n3)Sefer Sorgulama\n4)Doluluk Oranlari\n\n0)Geri Don\n\nSecim yapiniz: ";
        cin >> islem;
        system("cls"); // Ekrani temizler
        switch(islem){
            case 1 :
                vt_baglan();
                yeni_sefer();
                goto kullanici_tercihi;
            case 2 :
                vt_baglan();
                sefer_iptal();
                goto kullanici_tercihi;
            case 3 :
                vt_baglan();
                sefer_sorgu();
                goto kullanici_tercihi;
            case 4 :
                vt_baglan();
                doluluk_orani();
                goto kullanici_tercihi;
            case 0 :
                goto kullanici_tercihi;
            default :// Yanlis giris kontrolu
                cout <<"!! UYARI !!\nGecersiz bir tercih yaptiniz\n!! UYARI !!\n"<< endl;
                goto islem_tercihi1;
        }
    }else if(kullanici == 2){/* Musteri islemleri */
        cout <<"***MUSTERI ISLEMLERI***\n"<< endl;
        islem_tercihi2:
        cout <<"1)Bilet Satin Alma\n2)Bilet Iptali\n3)Yolcu Sogulama\n\n0)Geri Don\n\nSecim yapiniz: ";
        cin >> islem;
        system("cls"); // Ekrani temizler
        switch(islem){
            case 1 :
                vt_baglan();
                bilet_al();
                goto kullanici_tercihi;
            case 2 :
                vt_baglan();
                bilet_iptal();
                goto kullanici_tercihi;
            case 3 :
                vt_baglan();
                yolcu_sorgu();
                goto kullanici_tercihi;
            case 0 :
                goto kullanici_tercihi;
            default :// Yanlis giris kontrolu
                cout <<"!! UYARI !!\nGecersiz bir tercih yaptiniz\n!! UYARI !!\n"<< endl;
                goto islem_tercihi2;
        }
    }else{// Yanlis giris kontrolu
        cout <<"!! UYARI !!\nGecersiz bir tercih yaptiniz\n!! UYARI !!\n"<< endl;
        goto  kullanici_tercihi;
    }
    mysql_close(baglanti);
    return 0;
}
