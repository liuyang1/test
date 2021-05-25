/** Elvis operator
 * GNU extension to C, allows oimmiting the 2nd operand,
 * and using implicitly the 1st operand as the 2nd also
 *
 * A ? B : C
 * A ? : C => A ? A : C
 */

#include <stdio.h>
#include <stdlib.h>

int main() {
	int a = 0;
	int b = a ? : 42;
	printf("a=%d b=%d\n", a, b);
	a = 1;
	b = a ? : 42;
	printf("a=%d b=%d\n", a, b);
	return 0;
}
