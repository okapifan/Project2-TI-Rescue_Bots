/*
Naam: Annelot Janssen
Klas: 1D
Studentnummer: 0987583
Vak: programeren 3
 */
 public class NormaleContainer implements Container{ // extend gebruik je voor het maken van een subklasse
     private int volgNummer;
     public NormaleContainer(int volgNummer) {
         this.volgNummer = volgNummer;
     }
     @Override
     public void uitladen() {
    }
    public void inladen (){
        System.out.println("Normale container wordt ingeladen");
    }
     public int getVolgnummer(){
         return volgNummer;
     }
}
