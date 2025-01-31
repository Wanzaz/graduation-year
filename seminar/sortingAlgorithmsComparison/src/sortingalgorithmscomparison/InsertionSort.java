package sortingalgorithmscomparison;

/**
 *
 * @author ondrejpazourek
 */
public class InsertionSort implements SortingStrategy {
    @Override
    public void sort(int[] data) {
        int n = data.length;
        for (int i = 1; i < n; i++) {
            int key = data[i];
            int j = i - 1;
            
            // Posouvání větších prvků doprava
            while (j >= 0 && data[j] > key) {
                data[j + 1] = data[j];
                j--;
            }
            // Vložení klíčového prvku na správné místo
            data[j + 1] = key;
        }
    }
}

