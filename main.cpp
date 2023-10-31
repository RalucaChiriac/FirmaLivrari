#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;
ifstream cit_fisier("tastatura.txt");

class Produs
{
    string nume;
    float pret;
    int nr_bucati;
public:
    ///constructori initializare
    Produs(string, float, int);
    Produs();
    ///destructor
    ~Produs()
    {}
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
    string get_nume()
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
    nume="";
}
Produs::Produs(string prod, float p, int nr)
{
    nume="";
    nume = prod;
    pret=p;
    nr_bucati=nr;
}
Produs::Produs(const Produs& p)  /// constructor de copiere
{
    nume="";
    this->nume = p.nume;
    this->pret=p.pret;
    this->nr_bucati=p.nr_bucati;
}

Produs& Produs::operator=(Produs& ob)
{
    if(this!=&ob)
    {
        nume = ob.nume;
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
istream &operator>>(istream &cit_fisier, Produs &p)
{
    getline(cit_fisier,p.nume);
    cit_fisier>>p.nr_bucati>>p.pret;
    return cit_fisier;
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

Comanda::Comanda(int nr, string m)
{
    suma_plata = 0;
    nr_prod = nr;
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
    string nume_prenume, adresa, tel;
    Comanda* com;
public:
    ///constructori initializare
    Client();
    Client(string, string, string, Comanda*);
    ///destructor
    ~Client() {
        delete com;
    }

    void deschide_comanda();
    void finalizeaza_comanda();
    void editeaza_cont(string, string);

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
    nume_prenume="";
    adresa="";
    tel="";
    com = new Comanda();

}
Client::Client(string np, string adr, string t, Comanda* cc)
{
    nume_prenume="";
    adresa="";
    tel="";
    com = new Comanda();
    nume_prenume=np;
    adresa=adr;
    tel=t;
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
void Client::editeaza_cont(string adresa, string tel)
{
    this->tel = tel;
    this->adresa = adresa;
}

ostream &operator<<(ostream& out, const Client& cl)
{
    cout<<"Client: "<<endl<<cl.nume_prenume<<" "<<cl.adresa<<" "<<cl.tel<<" "<<endl;
    return out;
}

istream &operator>>(istream &cit_fisier, Client &cl)
{
    getline(cit_fisier,cl.nume_prenume);
    getline(cit_fisier,cl.adresa);
    getline(cit_fisier, cl.tel);
    return cit_fisier;
}

int main()
{

    Client cnt;
    //cout<<"Introdu datele de client (Nume, Adresa, Telefon, fiecare pe cate un rand):\n";
    cit_fisier>>cnt; //supraincarcare operator citire

    //cout<<"1.Schimba date cont client\n2.Incepe cumparaturile\n";
    int x;
    cit_fisier>>x;
    cit_fisier.get();
    if(x==1)
    {
        char tel[50], adr[50];
        cout<<"Noua adresa:\n";
        cit_fisier.getline(adr, 50);
        cout<<"Noul telefon:\n";
        cit_fisier.getline(tel, 50);
        cnt.editeaza_cont(adr, tel);
    }

    cout<<cnt<<"\nSe deschide comanda:"; //supraincarcare operator afisare
    cnt.deschide_comanda();
    int nr;
    //cout<<"Introdu numarul de produse";
    cit_fisier>>nr;
    cit_fisier.get();
    for(int i=1; i<=nr; i++)
    {
        //cout<<"Adauga produsul "<<i<<" (Nume, Numar bucati, Pretul, fiecare pe cate o linie):\n";
        char n[50];
        int bucati;
        float pret;
        cit_fisier.getline(n, 50);
        cit_fisier>>bucati>>pret;
        cit_fisier.get();
        Produs p(n, bucati, pret); //constr. init. cu param.
        cnt.getter_comanda()->adauga_produs(p);
    }

    cnt.getter_comanda()->afis_produse();
    //cout<<"Alege urmatoarul pas:\n"<<"1.Elimina un produs.\n"<<"2.Adauga inca o data un produs existent din comanda.\n";
    //cout<<"3.Finalizeaza comanda\n";
    cit_fisier>>x;
    switch(x)
    {
    case 1:
        //cout<<"Alege nr produsului de eliminat:\n";
        int nr;
        cit_fisier>>nr;
        cnt.getter_comanda()->sterge_produs(nr);
        //cout<<endl;
        //cout<<endl;
        //cout<<endl;
        //for(int i=1; i<=c.get_nr_prod(); i++)
            //cout<<cnt.getter_comanda()[i]<<" "; //operator de indexare
         break;

    case 2:
        cout<<"Alege nr produsului de adaugat:\n";
        cit_fisier>>nr;
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
