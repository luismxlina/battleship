#include <stdio.h>

int main() {
  int n;
  printf("Introduce un número n: ");
  scanf("%d", &n);

  // Imprimir las columnas
  for (int i = 0; i < n; i++) {
    printf(" %c", 'a' + i);
  }
  printf("\n");

  // Imprimir las filas
  for (int i = 0; i < n; i++) {
    printf("%d ", i);
    for (int j = 0; j < n; j++) {
      if ((i + j) % 2 == 0) {
        printf("\033[47m  \033[0m"); // Fondo blanco
      } else {
        printf("\033[40m  \033[0m"); // Fondo negro
      }
    }
    printf("\n");
  }

  return 0;
}
