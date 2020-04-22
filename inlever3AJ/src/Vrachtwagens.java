/*
Naam: Annelot Janssen
Klas: 1D
Studentnummer: 0987583
Vak: programeren 3
 */
public class Vrachtwagens extends Thread {

    private Kade kade;
    private Container container = null;
    private String naam;
    private Containerschip kontainerschip; // fields


    public Vrachtwagens(String naam, Kade kade, Containerschip kontainerschip) {
        this.naam = naam;
        this.kade = kade;
        this.kontainerschip = kontainerschip;
    }

    private void pause(int min, int max) {
        try {
            sleep(((int) (Math.random() * ((max - min) + 1)) + min)*1000); // geeft een randowm waarde voor de delay
        } catch (InterruptedException e) { /* vangt een exceptie op */

        }
    }


    public void run() {
        while(kontainerschip.getAantalContainer() != 0 || kade.getAantalContainer() !=0){
            container = kade.containerDoorgeven();
            System.out.println("De vrachtwagen " +naam+"brengt container "+container.getVolgnummer());
            pause(1, 25);
            System.out.println("De vrachtwagen "+naam+ " heeft de container "+container.getVolgnummer()+" weggebracht");
            container=null;
        }
    }
}
