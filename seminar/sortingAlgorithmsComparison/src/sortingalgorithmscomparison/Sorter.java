package sortingalgorithmscomparison;

/**
 *
 * @author ondrejpazourek
 */
import java.util.Arrays;
import java.util.Random;

public class Sorter {
    private int[] data;
    private int[] originalData;
    private SortingStrategy strategy;

    // Konstruktor, přijímá řadící strategii
    public Sorter(SortingStrategy strategy, int size) {
        this.strategy = strategy;
        data = new int[size];
        originalData = new int[size];
        generateData();  // Generujeme náhodná data
        System.arraycopy(data, 0, originalData, 0, data.length);  // Uložíme originální data
    }

    // Generování náhodných dat
    private void generateData() {
        Random rand = new Random();
        for (int i = 0; i < data.length; i++) {
            data[i] = rand.nextInt(100000) + 1; // Generujeme čísla mezi 1 a 100000
        }
    }

    // Zamíchání dat
    public void shuffleData() {
        Random rand = new Random();
        for (int i = 0; i < data.length; i++) {
            int j = rand.nextInt(data.length);
            // Swap
            int temp = data[i];
            data[i] = data[j];
            data[j] = temp;
        }
    }

    // Vrácení dat do původního stavu
    public void resetData() {
        System.arraycopy(originalData, 0, data, 0, data.length);
    }

    // Řazení podle aktuální strategie
    public void sortData() {
        strategy.sort(data);
    }

    // Získání seřazených dat
    public int[] getData() {
        return data;
    }

    // Získání aktuální délky dat
    public int getDataLength() {
        return data.length;
    }

    // Nastavení nové strategie řazení
    public void setSortingStrategy(SortingStrategy strategy) {
        this.strategy = strategy;
    }
}

