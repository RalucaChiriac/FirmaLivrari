#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;
ifstream in("tastatura.txt");

class Produs
{
    char *nume;
    float pret;
    int nr_bucati;
public:
    ///constructori initializare
    Produs(char*, float, int);
    Produs();
    ///destructor
    ~Produs()
    {
        delete[] nume;
    }
    ///constructor copiere
    Produs(const Produs&);
    ///get si set
    float get_pret()
    {
        return pret;
    }
    int get_nr_bucati()
    {
        return nr_bucati;
    }
    void set_nr_bucati(int x)
    {
        nr_bucati=x;
    }
    char* get_nume()
    {
        return nume;
    }
    ///operator de copiere
    Produs& operator=(Produs&);

    ///operator de afisare
    friend ostream &operator<<(ostream&, const Produs&);
    ///operator de citire
    friend istream &operator>>(istream &, Produs &);
};
Produs::Produs()
{
    nr_bucati=0;
    pret=0;
    nume=new char(50);
}
Produs::Produs(char* prod, float p, int nr)
{
    nume=new char(50);
    strcpy(nume, prod);
    pret=p;
    nr_bucati=nr;
}
Produs::Produs(const Produs& p)  /// constructor de copiere
{
    nume=new char(50);
    strcpy(this->nume, p.nume);
    this->pret=p.pret;
    this->nr_bucati=p.nr_bucati;
}

Produs& Produs::operator=(Produs& ob)
{
    if(this!=&ob)
    {
        this->nume=new char[strlen(ob.nume)+1];
        strcpy(this->nume, ob.nume);
        this->pret=ob.pret;
        this->nr_bucati=ob.nr_bucati;
    }
    return *this;
}

ostream &operator<<(ostream& out, const Produs &p)
{
    cout<<"Nume: "<<p.nume<<endl;
    cout<<"Nr. bucati: "<<p.nr_bucati<<" Pret: "<<p.pret<<endl;
    cout<<endl;
    return out;
}
istream &operator>>(istream &in, Produs &p)
{
    in.getline(p.nume, 50);
    in>>p.nr_bucati>>p.pret;
    return in;
}

class Comanda
{
    int nr_prod;
    string magazin, status;
    float suma_plata;
    Produs p[20];
public:
    ///constructori initializare
    Comanda();
    Comanda(int, string);
    ///destructor
    ~Comanda()
    {}
    ///get si set
    string get_status()
    {
        return status;
    }
    void set_status(string s)
    {
        status = s;
    }
    void set_suma_plata()
    {
        suma_plata=0;
    }
    int get_nr_prod()
    {
        return nr_prod;
    }
    Produs get_prod(int i)
    {
        return p[i];
    }

    void sterge_produs(int i);
    void adauga_produs(Produs prod);
    void afis_suma_plata();
    void afis_produse();

    Produs operator[](int i) /// supraincarcare []
    {
        return p[i];
    }

    ///operator afisare
    friend ostream &operator<<(ostream&, const Comanda&);
};

ostream &operator<<(ostream& out, const Comanda& c)
{
    cout<<"Comanda:"<<endl<<" "<<c.magazin<<" "<<c.status<<" "<<c.suma_plata<<endl;
    return out;
}

Comanda::Comanda()
{
    suma_plata=0;
    nr_prod=0;
    magazin="";
    status="";
}

Comanda::Comanda(int nr, string m):suma_plata(0), nr_prod(0)
{
    magazin="";
    status="";
    magazin = m;
    status = "Adauga produse";
}

void Comanda::adauga_produs(Produs prod)
{
    nr_prod++;
    p[nr_prod]=prod;
    suma_plata+=prod.get_pret()*prod.get_nr_bucati();
}

void Comanda::sterge_produs(int i)
{
    if (i==0 || i>nr_prod)
        cout<<"Produsul selectat nu exista in cos\n";
    else
    {
        Produs prod=p[i];
        suma_plata-=prod.get_pret()*prod.get_nr_bucati();

        cout<<"Produsul "<<prod.get_nume()<<" a fost sters din comanda\n";
        for(int j=i; j<nr_prod; j++)
            p[j]=p[j+1];
        nr_prod--;
    }
}

void Comanda::afis_produse()
{
    cout<<"Produsele din comanda:\n";
    for(int i=1; i<=nr_prod; i++)
        cout<<i<<". "<<" "<<p[i]<<" ";
    cout<<'\n';
}

void Comanda::afis_suma_plata()
{
    status = "Comanda procesata";
    cout<<status<<". De plata: "<<suma_plata<<"lei\n";
}


class Client
{
    char* nume_prenume, *adresa, *tel;
    Comanda* com;
public:
    ///constructori initializare
    Client();
    Client(char*, char*, char*, Comanda*);
    ///destructor
    ~Client() {
        delete com;
    }

    void deschide_comanda();
    void finalizeaza_comanda();
    void editeaza_cont(char*, char*);

    Comanda* getter_comanda()
    {
        return com;
    }

    ///operator afisare
    friend ostream &operator<<(ostream&, const Client&);
    ///operator citire
    friend istream &operator>>(istream &, Client &);

};

Client::Client()
{
    nume_prenume=new char[50];
    adresa=new char[50];
    tel=new char[50];
    com = new Comanda();

}
Client::Client(char* np, char* adr, char* t, Comanda* cc)
{
    nume_prenume=new char[50];
    adresa=new char[50];
    tel=new char[50];
    com = new Comanda();
    strcpy(nume_prenume, np);
    strcpy(adresa, adr);
    strcpy(tel, t);
    com = cc;
}

void Client::deschide_comanda()
{
    com->set_status("Comanda deschisa");
    com->set_suma_plata();
    cout<<" "<<com->get_status()<<endl;
}
void Client::finalizeaza_comanda()
{
    cout<<"Detalii livrare:\n"<<"Client\n"<<nume_prenume<<", "<<adresa<<", "<<tel<<endl;
    com->afis_suma_plata();

}
void Client::editeaza_cont(char *adresa, char *tel)
{
    strcpy(this->tel, tel);
    strcpy(this->adresa, adresa);
}

ostream &operator<<(ostream& out, const Client& cl)
{
    cout<<"Client: "<<endl<<cl.nume_prenume<<" "<<cl.adresa<<" "<<cl.tel<<" "<<endl;
    return out;
}

istream &operator>>(istream &in, Client &cl)
{
    in.getline(cl.nume_prenume, 50);
    in.getline(cl.adresa, 50);
    in.getline(cl.tel, 50);
    return in;
}

int main()
{

    Client cnt;
    //cout<<"Introdu datele de client (Nume, Adresa, Telefon, fiecare pe cate un rand):\n";
    in>>cnt; //supraincarcare operator citire
    Comanda c; //constructor initializare fara param.

    //cout<<"1.Schimba date cont client\n2.Incepe cumparaturile\n";
    int x;
    in>>x;
    in.get();
    if(x==1)
    {
        char tel[50], adr[50];
        cout<<"Noua adresa:\n";
        in.getline(adr, 50);
        cout<<"Noul telefon:\n";
        in.getline(tel, 50);
        cnt.editeaza_cont(adr, tel);
    }

    cout<<cnt<<"\nSe deschide comanda:"; //supraincarcare operator afisare
    cnt.deschide_comanda();
    int nr;
    //cout<<"Introdu numarul de produse";
    in>>nr;
    in.get();
    for(int i=1; i<=nr; i++)
    {
        //cout<<"Adauga produsul "<<i<<" (Nume, Numar bucati, Pretul, fiecare pe cate o linie):\n";
        char n[50];
        int bucati;
        float pret;
        in.getline(n, 50);
        in>>bucati>>pret;
        in.get();
        Produs p(n, bucati, pret); //constr. init. cu param.
        cnt.getter_comanda()->adauga_produs(p);
    }

    cnt.getter_comanda()->afis_produse();
    //cout<<"Alege urmatoarul pas:\n"<<"1.Elimina un produs.\n"<<"2.Adauga inca o data un produs existent din comanda.\n";
    //cout<<"3.Finalizeaza comanda\n";
    in>>x;
    switch(x)
    {
    case 1:
        //cout<<"Alege nr produsului de eliminat:\n";
        int nr;
        in>>nr;
        cnt.getter_comanda()->sterge_produs(nr);
        //cout<<endl;
        //cout<<endl;
        //cout<<endl;
        //for(int i=1; i<=c.get_nr_prod(); i++)
            //cout<<cnt.getter_comanda()[i]<<" "; //operator de indexare
         break;

    case 2:
        cout<<"Alege nr produsului de adaugat:\n";
        in>>nr;
        Produs p2=cnt.getter_comanda()->get_prod(nr);//operator de atribuire
        Produs p3(p2); //constructor de copiere
        cout<<p2<<'\n'<<p3<<'\n';
        cnt.getter_comanda()->adauga_produs(p3);

        break;
    }
    cout<<"Finalizare comanda\n";
    cnt.finalizeaza_comanda();

    return 0;
}
