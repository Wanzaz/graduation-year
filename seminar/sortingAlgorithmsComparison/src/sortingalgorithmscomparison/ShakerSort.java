/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package sortingalgorithmscomparison;

/**
 *
 * @author ondrejpazourek
 */
public class ShakerSort implements SortingStrategy {
    @Override
    public void sort(int[] array) {
        boolean swapped = true;
        int start = 0;
        int end = array.length - 1;

        while (swapped) {
            swapped = false;

            // Procházení zleva doprava
            for (int i = start; i < end; i++) {
                if (array[i] > array[i + 1]) {
                    swap(array, i, i + 1);
                    swapped = true;
                }
            }

            if (!swapped) break;  // Pokud nebyly provedeny žádné výměny, pole je seřazeno

            swapped = false;
            end--;

            // Procházení zprava doleva
            for (int j = end; j >= start; j--) {
                if (array[j] > array[j + 1]) {
                    swap(array, j, j + 1);
                    swapped = true;
                }
            }
            start++;
        }
    }

    // Metoda pro výměnu hodnot v poli
    private void swap(int[] array, int i, int j) {
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}


