#include <stdio.h>
#include <stdlib.h>
int binarySearch(int arr[], int n, int key) {
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid] == key)
            return mid;
        else if (arr[mid] < key)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}
int main(int argc, char *argv[]) {
    int n = argc - 1;
    int arr[n];
    for (int i = 1; i < argc; i++)
        arr[i-1] = atoi(argv[i]);
    printf("Child process received sorted array: ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
    int key;
    printf("Enter element to search: ");
    scanf("%d", &key);
    int result = binarySearch(arr, n, key);
    if (result != -1)
        printf("Element found at index %d\n", result);
    else
        printf("Element not found\n");
    return 0;
}