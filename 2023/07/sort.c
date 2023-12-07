#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(int arr[], int i, int j) {
  int tmp = arr[i];
  arr[i] = arr[j];
  arr[j] = tmp;
}

int partition(int arr[], int low, int high) {
  int pivot = arr[high];
  
  int i = low - 1;
   
  for (int j = low; j <= high; j++) {
    if (arr[j] < pivot) {
      swap(arr, ++i, j);
    }
  }

  swap(arr, ++i, high);
  return i;
}
           
void quickSort(int arr[], int low, int high) {
  if (low < high) {  
    int pi = partition(arr, low, high);
    quickSort(arr, low, pi - 1);
    quickSort(arr, pi + 1, high);
  }
}
              
  
int main() {
  int arr[] = {10,7,8,9,1,5};
  int n = sizeof(arr) / sizeof(arr[0]);
  
  quickSort(arr, 0, n-1);
  
  printf("Sorted: ");
  for (int i = 0; i < n; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}