/*
Naam: Annelot Janssen
Klas: 1D
Studentnummer: 0987583
Vak: programeren 3
 */
public class KoudeContainer implements Container { // extend is voor het maken van een subclass
     private int volgNummer;

     public KoudeContainer(int volgNummer) {
         this.volgNummer = volgNummer;
     }
    @Override
     public  void uitladen (){
        System.out.println("De container wordt losgekoppeld van het cool element");
    }
    public  void inladen (){
        System.out.println("De container wordt vastgekoppeld aan het cool element");
    }
    public int getVolgnummer(){
        return volgNummer;
    }

}
