/*
 * ============================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/31/2016 12:41:04 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * ============================================================================
 */
#include <stdlib.h>
#include "abp.h"

int main() {
    ABP *abp = abp_create(5, 10);
    abp_labelDestroy(abp);
    return 0;
}

