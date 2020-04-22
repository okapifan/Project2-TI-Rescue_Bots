/*
Naam: Annelot Janssen
Klas: 1D
Studentnummer: 0987583
Vak: programeren
 */
public class Containerschip {
    private Container[] container = new Container[100]; // je intialiseerd de array + maakt het object aan
    private int aantalContainer = 100; // field

    public int getAantalContainer() {
        return aantalContainer;
    }

    public void vulConatinerschip() { // je hebt een fucntiea
        for (int i = 0; i < 100; i++) {
            int j = (int) (Math.random() * ((3 - 1) + 1)) + 1; // geeft een random waarde tussen de 1 en de 3 voor soort conatiainer
            if (j == 1) { // voorwaarde aan verbonden
                container[i] = new WarmContainer(i); // maakt het object aan
            } else if (j == 2) {
                container[i] = new KoudeContainer(i);
            } else if (j == 3) {
                container[i] = new NormaleContainer(i);
            }
        }

    }


    public synchronized Container containerDoorgeven() {
        while (aantalContainer <= 0) {
            try {
                wait();
            } catch (InterruptedException e) {

            }
        }
        notifyAll();
        Container kontainer = null; // maakt object aan
        for (int j = 0; j < 100; j++) {
            if (container[j] != null) {
                kontainer = container[j];
                container[j] = null; // verwijderd het
                break; // stopt de for loop
            }
        }
        kontainer.uitladen();
        System.out.println("Het volgnummer van de container is " + kontainer.getVolgnummer() + " Het wordt verplaats door de kraan");
        aantalContainer--;
        return kontainer;
    }


    public Containerschip() { // constructor
        vulConatinerschip(); // roept functie op
    }
}
