package sortingalgorithmscomparison;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;



public class Meter {

    public static void runTests() {
        // Seznam velikostí testovaných polí
        int[] sizes = {100, 1000, 10000};  // Malé, středně velké a velké pole

        // Seznam řadících algoritmů
        List<SortingStrategy> algorithms = new ArrayList<>();
        algorithms.add(new BubbleSort());
        algorithms.add(new InsertionSort());
        algorithms.add(new SelectionSort());
        algorithms.add(new ShakerSort());

        // Seznam podmínek pro testování dat
        List<String> conditions = Arrays.asList("Seřazená data", "Schodová data", "Opačně seřazená data");

        // Pro každou velikost pole a každý algoritmus provedeme testy pro všechny podmínky
        for (int size : sizes) {
            System.out.println("Testování velikosti pole: " + size);
            for (SortingStrategy algorithm : algorithms) {
                for (String condition : conditions) {
                    testSortingAlgorithm(size, algorithm, condition);
                }
            }
            System.out.println("--------------------------------------");
        }
    }

    // Metoda pro testování jednoho algoritmu a jedné podmínky
    private static void testSortingAlgorithm(int size, SortingStrategy algorithm, String condition) {
        // Počet opakování pro průměrné měření
        int repetitions = 10;

        // Uložení času pro každé opakování
        double totalDuration = 0;

        // Provádíme měření několikrát a spočítáme průměr
        for (int i = 0; i < repetitions; i++) {
            // Vytvoření nového objektu Sorter s daným algoritmem a velikostí
            Sorter sorter = new Sorter(algorithm, size);

            // Nastavení dat podle podmínky
            setDataCondition(sorter, condition);

            // Měření času provedení řazení
            long startTime = System.nanoTime();
            sorter.sortData();
            long endTime = System.nanoTime();

            // Výpočet doby trvání v sekundách pro toto opakování
            double durationInSeconds = (endTime - startTime) / 1_000_000_000.0;
            totalDuration += durationInSeconds;
        }

        // Výpočet průměrného času
        double averageDuration = totalDuration / repetitions;

        // Výpis průměrného výsledku
        System.out.printf("%-20s (%-20s): %.9f s (průměr z %d testů)%n", algorithm.getClass().getSimpleName(), condition, averageDuration, repetitions);
    }

    // Nastavení dat podle podmínky
    private static void setDataCondition(Sorter sorter, String condition) {
        switch (condition) {
            case "Seřazená data":
                sorter.resetData();
                break;
            case "Schodová data":
                sorter.resetData();
                sorter.shuffleData();  // Zamícháme data a uděláme je schodová
                break;
            case "Opačně seřazená data":
                sorter.resetData();
                reverseArray(sorter.getData());  // Obrátíme pořadí dat
                break;
            default:
                sorter.resetData();
                break;
        }
    }

    // Obrácení pole pro opačně seřazená data
    private static void reverseArray(int[] data) {
        int start = 0;
        int end = data.length - 1;
        while (start < end) {
            int temp = data[start];
            data[start] = data[end];
            data[end] = temp;
            start++;
            end--;
        }
    }
}
