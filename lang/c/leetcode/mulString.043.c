char *multiply(char *num1, char *num2) {
    int n1 = strlen(num1), n2 = strlen(num2);
    char *s = malloc(sizeof(char) * (n1 + n2 + 2));
    char *p1, *p2;
    for (p1 = num1 + n1 - 1, p2 = num1 + n2 - 1;
         p1 >= num1 || p2 >= 

}
