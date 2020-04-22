/*
Naam: Annelot Janssen
Klas: 1D
Studentnummer: 0987583
Vak: programeren 3
 */
public class Kranen extends Thread { // extensie thread wordt gebruikt
    private Container container; // object wordt geintialiseerd
    private Containerschip kontainerschip; // fields
    private Kade kade; // fields
    private String naam;

    public Kranen(Containerschip kontainerschip, Kade kade, String naam) { // constructer met parameters
        this.kontainerschip = kontainerschip;
        this.kade = kade;
        this.naam = naam;
    }
    private void pause(int min, int max){
        try {
            sleep(((int)(Math.random()*((max-min)+1)) + min)*1000); // geeft een randowm waarde voor de delay
        } catch (InterruptedException e) { /* vangt een exceptie op */

        }
    }


    public void run() { // thread
        while (kontainerschip.getAantalContainer() >0 ) {
            container = kontainerschip.containerDoorgeven();
            System.out.println("De container " +container.getVolgnummer()+" wordt meegenomen door de kraan genaamd "+naam);
            pause(1, 6);
            kade.plaatsContainer(container);
            System.out.println("De container is op de kade geplaats door de kraan " +naam);
        }
    }
}
