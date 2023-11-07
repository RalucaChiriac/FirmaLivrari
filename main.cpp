#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <array>
#include <random>
#include <chrono>
#include <thread>


class Produs
{
    int nr_bucati;
    float pret;
    std::string nume;

public:
    ///constructori initializare
    Produs(int, float, const std::string&);
    Produs();
    ///destructor
    ~Produs()
    {}
    ///constructor copiere
    Produs(const Produs&);
    ///get si set

    float getter_pret_per_total()
    {
        return pret*nr_bucati;
    }

    std::string get_nume()
    {
        return nume;
    }
    ///operator de copiere
    Produs& operator=(const Produs&);

    ///operator de afisare
    friend std::ostream &operator<<(std::ostream&, const Produs&);
    ///operator de citire
    friend std::istream &operator>>(std::istream &, Produs &);
};
Produs::Produs():nr_bucati(0), pret(0), nume("")
{}
Produs::Produs(int nr, float p, const std::string& prod):nr_bucati(nr), pret(p), nume(prod)
{}
Produs::Produs(const Produs& p)  /// constructor de copiere
{
    this->nume = p.nume;
    this->pret=p.pret;
    this->nr_bucati=p.nr_bucati;
}

Produs& Produs::operator=(const Produs& ob)
{
    if(this!=&ob)
    {
        nume = ob.nume;
        this->pret=ob.pret;
        this->nr_bucati=ob.nr_bucati;
    }
    return *this;
}

std::ostream &operator<<(std::ostream& out, const Produs &p)
{
    std::cout<<"Nume: "<<p.nume<<std::endl;
    std::cout<<"Nr. bucati: "<<p.nr_bucati<<" Pret: "<<p.pret<<std::endl;
    std::cout<<std::endl;
    return out;
}
std::istream &operator>>(std::istream &cit_fisier, Produs &p)
{
    cit_fisier>>p.nume;
    cit_fisier>>p.nr_bucati>>p.pret;
    return cit_fisier;
}

class Comanda
{
    int nr_prod;
    std::string magazin, status;
    std::array<Produs, 20> produse;
public:
    ///constructori initializare
    Comanda();
    Comanda(int, const std::string&);
    ///destructor
    ~Comanda()
    {}
    ///get si set
    std::string get_status()
    {
        return status;
    }
    void set_status(const std::string& s)
    {
        status = s;
    }

    void avanseaza_status();
    void sterge_produs(int i);
    void adauga_produs(Produs prod);
    void afis_produse();
    float calcul_suma_plata();

    Produs operator[](int i) /// supraincarcare []
    {
        return produse[i];
    }

    ///operator afisare
    friend std::ostream &operator<<(std::ostream&, const Comanda&);
};

std::ostream &operator<<(std::ostream& out, const Comanda& c)
{
    std::cout<<"Comanda:"<<std::endl<<" "<<c.magazin<<" "<<c.status<<std::endl;
    return out;
}

Comanda::Comanda():nr_prod(0), magazin(""), status("")
{}

Comanda::Comanda(int nr, const std::string& m):nr_prod(nr), magazin(m), status("Adauga produse")
{}

void Comanda::adauga_produs(Produs prod)
{
    nr_prod++;
    produse[nr_prod]=prod;
    //suma_plata+=prod.get_pret()*prod.get_nr_bucati();
}

void Comanda::sterge_produs(int i)
{
    if (i==0 || i>nr_prod)
        std::cout<<"Produsul selectat nu exista in cos\n";
    else
    {
        Produs prod=produse[i];
        //suma_plata-=prod.get_pret()*prod.get_nr_bucati();

        std::cout<<"Produsul "<<prod.get_nume()<<" a fost sters din comanda\n";
        for(int j=i; j<nr_prod; j++)
            produse[j]=produse[j+1];
        nr_prod--;
    }
}

void Comanda::afis_produse()
{
    std::cout<<"Produsele din comanda:\n";
    for(int i=1; i<=nr_prod; i++)
        std::cout<<i<<". "<<" "<<produse[i]<<" ";
    std::cout<<'\n';
}

float Comanda::calcul_suma_plata()
{
    float suma_finala = 0.0;
    int maxim = produse.size();
    for (int i=0;i<maxim;i++)
    {
        suma_finala+=produse[i].getter_pret_per_total();
    }

    return suma_finala;
}

void Comanda::avanseaza_status()
{
    if (status=="Comanda plasata")
        set_status("depozit");
    else
        if (status=="depozit")
            set_status("plecat din depozit");
    else
        if( status=="plecat din depozit")
            set_status("oras destinatie");
    else
        if (status=="oras destinatie")
            set_status("in livrare");
    else
        if (status=="in livrare")
        set_status("Livrat!");
}


class Client
{
    std::string nume_prenume, adresa, tel;
    Comanda com;
public:
    ///constructori initializare
    Client();
    Client(const std::string&, const std::string&, const std::string&, const Comanda&);
    ///destructor
    ~Client() {}

    void deschide_comanda();
    void comanda_plasata();
    void editeaza_cont(const std::string&, const std::string&);

    Comanda& getter_comanda()
    {
        return com;
    }

    ///operator afisare
    friend std::ostream &operator<<(std::ostream&, const Client&);
    ///operator citire
    friend std::istream &operator>>(std::istream &, Client &);

};

Client::Client():nume_prenume(""), adresa(""), tel("")
{}

Client::Client(const std::string& np, const std::string& adr, const std::string& t, const Comanda& cc):nume_prenume(np), adresa(adr), tel(t), com(cc)
{}

void Client::deschide_comanda()
{
    com.set_status("Comanda noua");
    std::cout<<" "<<com.get_status()<<std::endl;
}
void Client::comanda_plasata()
{
    getter_comanda().set_status("Comanda plasata");
    std::cout<<"Detalii livrare:\n"<<"Client\n"<<nume_prenume<<", "<<adresa<<", "<<tel<<std::endl;
}
void Client::editeaza_cont(const std::string& adresa_temp, const std::string& tel_temp)
{
    this->tel = tel_temp;
    this->adresa = adresa_temp;
}

std::ostream &operator<<(std::ostream& out, const Client& cl)
{
    std::cout<<"Client: "<<std::endl<<cl.nume_prenume<<" "<<cl.adresa<<" "<<cl.tel<<" "<<std::endl;
    return out;
}

std::istream &operator>>(std::istream &cit_fisier, Client &cl)
{
    cit_fisier>>cl.nume_prenume;
    cit_fisier>>cl.adresa;
    cit_fisier>>cl.tel;
    return cit_fisier;
}

int main()
{
    std::ifstream cit_fisier("tastatura.txt");

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
        std::cout<<"Noua adresa:\n";
        cit_fisier.getline(adr, 50);
        std::cout<<"Noul telefon:\n";
        cit_fisier.getline(tel, 50);
        cnt.editeaza_cont(adr, tel);
    }

    std::cout<<cnt<<"\nSe deschide comanda:"; //supraincarcare operator afisare
    cnt.deschide_comanda();
    int nr;
    //cout<<"Introdu numarul de produse";
    cit_fisier>>nr;
    cit_fisier.get();
    for(int i=1; i<=nr; i++)
    {
        //cout<<"Adauga produsul "<<i<<" (Nume, Numar bucati, Pretul, fiecare pe cate o linie):\n";
        std::string nt;
        int bucati;
        float pret;
        cit_fisier>>nt;
        cit_fisier>>bucati>>pret;
        cit_fisier.get();
        Produs p(bucati, pret, nt); //constr. init. cu param.
        cnt.getter_comanda().adauga_produs(p);
    }
    float suma_plata = cnt.getter_comanda().calcul_suma_plata();

    cnt.getter_comanda().afis_produse();
    //cout<<"Alege urmatoarul pas:\n"<<"1.Elimina un produs.\n"<<"2.Adauga inca o data un produs existent din comanda.\n";
    //cout<<"3.Finalizeaza comanda\n";
    cit_fisier>>x;
    switch(x)
    {
    case 1:
        //cout<<"Alege nr produsului de eliminat:\n";
        int nr_temp;
        cit_fisier>>nr_temp;
        cnt.getter_comanda().sterge_produs(nr_temp);
        suma_plata-=cnt.getter_comanda()[nr_temp].getter_pret_per_total();
        //cout<<endl;
        //cout<<endl;
        //cout<<endl;
        //for(int i=1; i<=c.get_nr_prod(); i++)
            //cout<<cnt.getter_comanda()[i]<<" "; //operator de indexare
         break;

    case 2:
        std::cout<<"Alege nr produsului de adaugat:\n";
        cit_fisier>>nr;
        Produs p2=cnt.getter_comanda()[nr];//operator de atribuire
        Produs p3(p2); //constructor de copiere
        std::cout<<p2<<'\n'<<p3<<'\n';
        cnt.getter_comanda().adauga_produs(p3);
        suma_plata+=cnt.getter_comanda()[nr+1].getter_pret_per_total();

        break;
    }
    std::cout<<std::endl;
    cnt.comanda_plasata();
    std::cout<<std::endl;
    std::cout<<"Pret total al comenzii: "<<suma_plata<<" de lei"<<std::endl;

    std::cout<<std::endl<<std::endl<<"Statusul actual al comenzii: Comanda plasata    (Va rugam nu inchideti programul fortat!)"<<std::endl;

    for (int i=0;i<5;i++)
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        std::uniform_real_distribution<double> distribution(1.0, 1.5);

        double randomTime = distribution(generator);
        int randomMilliseconds = static_cast<int>(randomTime * 1000);

        std::cout <<std::endl<<"Timp asteptare pentru noul status: " << randomTime << " secunde" << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(randomMilliseconds));
        cnt.getter_comanda().avanseaza_status();
        std::cout<<"Status actual: "<<cnt.getter_comanda().get_status();
        std::cout<<std::endl<<std::endl<<std::endl;
    }


    std::cout<<std::endl<<"Va multumim pentru utilizarea programului nostru!"<< std::endl;


    return 0;
}
