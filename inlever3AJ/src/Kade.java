
/*
Naam: Annelot Janssen
Klas: 1D
Studentnummer: 0987583
Vak: programeren 3
 */
public class Kade {
    private Container[] berging = new Container[5]; // je intialiseerd de array/
    private int aantalContainer=0;

    public int getAantalContainer() {
        return aantalContainer;
    }

    public synchronized void plaatsContainer(Container container) { // method met parameter
        while (aantalContainer == 5) {
            try {
                wait();
            } catch (InterruptedException e) {

            }
        }
        notifyAll();
        for (int i = 0; i < 5; i++) {
            if (berging[i] == null) { // kijken op plek vrij in de array
                berging[i] = container;
                aantalContainer++;
                break; // stopt de for loop
            }
        }
        System.out.println("Er zijn zoveel plekken bezet op de kade" +aantalContainer);
    }

    public synchronized Container containerDoorgeven() {
        Container kontainer = null; // maakt object aan
        while (aantalContainer <= 0) {
            try {
                wait();
            }
            catch (InterruptedException e) {

            }
        }
        notifyAll();
        for (int j = 0; j < 5; j++) {
            if (berging[j] != null) {
                if (berging[j] instanceof NormaleContainer) {
                    if (kontainer == null){
                        kontainer = berging[j];
                        berging[j] = null;
                        aantalContainer--;
                    }
                } else if (berging[j] instanceof WarmContainer || berging[j] instanceof KoudeContainer) {
                    if (kontainer != null){
                        plaatsContainer(kontainer);
                    }
                    kontainer = berging[j];
                    berging[j] = null;
                    aantalContainer--;
                    break;
                }
            }
        }
        kontainer.uitladen();
        System.out.println("Het volgnummer van de container is " + kontainer.getVolgnummer() + " Het wordt meegenoemen door de vrachtwagen.");
        System.out.println("Er zijn zoveel plekken bezet op de kade " +aantalContainer);

        return kontainer;
    }


    public Kade() {

    }

}
