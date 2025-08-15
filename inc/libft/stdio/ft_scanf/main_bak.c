/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 01:02:37 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/15 16:31:08 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"
#include "ft_scanf.h"
#include <unistd.h>
#include "libft.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    char    input[256];
    int     i;

    ft_printf("write the input:");
    if (ft_scanf("%s %d", input, &i) != 2)
        return (write(1, "error", 5), 1);
    ft_printf("%s\n", input);
    ft_printf("\ninteger: %i\n", i);
    return (0);
}


//int main(void)
//{
//    int     i;
//    float   fp;
//    char    c;
//    char    s[81];
//
//    printf("Enter an integer, a real number, and a string: \n");
//    if (scanf("%d %f %c %s", &i, &fp, &c, s)  != 4)
//        printf("not all field were assigned");
//    else
//    {
//        printf("integer = %d\n", i);
//        printf("real number = %f\n", fp);
//        printf("character = %c\n", c);
//        printf("srtring = %s\n", s);
//    }
//}


//int main(void)
//{
//    int number;
//
//    printf("enter a hexadecimal number or anything else to quit:\n");
//    while (scanf("%x", &number))
//    {
//        printf("Hexadecimal Number = %x\n", number);
//        printf("Decimal number = %d\n", number);
//    }
//}
