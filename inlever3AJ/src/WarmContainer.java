/*
Naam: Annelot Janssen
Klas: 1D
Studentnummer: 0987583
Vak: programeren 3
 */
public class WarmContainer implements Container{ // extend is voor het maken van een subclasse
    private int volgNummer;

    public WarmContainer(int volgNummer) {
        this.volgNummer = volgNummer;
    }

    @Override
    public  void uitladen (){
        System.out.println("De container wordt losgekoppeld van het verwarmingelement");
    }
    public  void inladen(){
        System.out.println(" De container wordt vasgekoppeld aan het verwarmingselement");
    }
    public int getVolgnummer(){
        return volgNummer;
    }

}
