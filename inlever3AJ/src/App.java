/*
Naam: Annelot Janssen
Klas: 1D
Studentnummer: 0987583
Vak: programeren
 */
public class App {
    public static void main(String[] args) {
        Containerschip titanic = new Containerschip();
        Kade kade = new Kade();

        Kranen desperado = new Kranen(titanic, kade, "desperado");
        Kranen corona = new Kranen(titanic, kade, "Corona");
        desperado.start();
        corona.start();

        Vrachtwagens rusland = new Vrachtwagens("Rusland", kade, titanic);
        Vrachtwagens china = new Vrachtwagens("China", kade, titanic);
        Vrachtwagens amerika = new Vrachtwagens("Amerika", kade, titanic);
        rusland.start();
        china.start();
        amerika.start();

    }
}

