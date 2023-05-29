#include <stdio.h>
#include <stdlib.h>

int main() {
   //sum of the first five natural numbers
    int sum = 0, input;
    for (int i = 1; i <= 5; i++) {
        scanf("%d", &input);
        sum += input;
    }
    printf("The sum is: %d", sum);
}
