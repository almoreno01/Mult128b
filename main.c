#include <stdio.h>
#include <immintrin.h>

typedef unsigned long long u64;

 /* Cada numero de 128 bits se divide en dos palabras de 64 bits*/

typedef struct
{
    u64 lo;
    u64 hi;

} uint128;


/*Cada numero de 256 bits se divide en cuatro palabras de 64 bits*/
typedef struct
{
    u64 limb[4];

} uint256;


/*
  Funcion para sumar un numero de 64 bits
  a una posicion determinada del resultado.
 
  Si ocurre overflow, se genera un acarreo
  hacia la siguiente palabra de 64 bits.
 */

void sumar_limb(u64 r[4], int posicion, u64 valor)
{
    while (valor != 0 && posicion < 4)
    {
        u64 anterior = r[posicion];

        /*
         * Realizamos la suma.
         */
        r[posicion] += valor;


        if (r[posicion] < anterior)
        {
            valor = 1;
        }
        else
        {
            valor = 0;
        }
        posicion++;
    }
}


/*Multiplicacion*/
uint256 multiplicar128(uint128 A, uint128 B)
{
    uint256 R = {{0, 0, 0, 0}};

    u64 hi00;
    u64 hi01;
    u64 hi10;
    u64 hi11;

    u64 lo00;
    u64 lo01;
    u64 lo10;
    u64 lo11;

    lo00 = _mulx_u64(A.lo, B.lo, &hi00);
    lo01 = _mulx_u64(A.lo, B.hi, &hi01);
    lo10 = _mulx_u64(A.hi, B.lo, &hi10);
    lo11 = _mulx_u64(A.hi, B.hi, &hi11);

    R.limb[0] = lo00;
    R.limb[1] = hi00;

    sumar_limb(R.limb, 1, lo01);
    sumar_limb(R.limb, 2, hi01);
    sumar_limb(R.limb, 1, lo10);
    sumar_limb(R.limb, 2, hi10);
    sumar_limb(R.limb, 2, lo11);
    sumar_limb(R.limb, 3, hi11);

    return R;
}


/*Imprime una variable byte por byte en memoria.*/
void imprimir_memoria(
    const char *nombre,
    const void *direccion,
    size_t cantidad)
{
    /*
     Convertimos la direccion a unsigned char *
    
     unsigned char ocupa exactamente 1 byte,
     por eso nos permite recorrer la memoria
     byte por byte.
    */
    const unsigned char *p =
        (const unsigned char *)direccion;


    printf("\n");
    printf("=================================================\n");
    printf("MEMORIA DE %s\n", nombre);
    printf("=================================================\n");

    printf("Direccion inicial : %p\n", direccion);
    printf("Cantidad de bytes : %zu\n", cantidad);

    printf("-------------------------------------------------\n");
    printf("Byte        Direccion              Valor\n");
    printf("-------------------------------------------------\n");


    /*
     * Recorremos todos los bytes.
     */
    for (size_t i = 0; i < cantidad; i++)
    {
        printf(
            "[%2zu]        %p        0x%02X\n",
            i,
            (void *)(p + i),
            (unsigned int)p[i]
        );
    }
}


/*
 * Funcion principal.
 */
int main(void)
{
    uint128 A;
    uint128 B;

    uint256 resultado;


    printf("\n");
    printf("============================================\n");
    printf("      MULTIPLICADOR DE 128 BITS\n");
    printf("============================================\n");


    printf("\nNumero A\n");
    printf("--------------------------------------------\n");


    printf("A_hi = 0x");

    if (scanf("%llx", &A.hi) != 1)
    {
        printf("\nError: valor incorrecto para A_hi.\n");
        return 1;
    }


    printf("A_lo = 0x");

    if (scanf("%llx", &A.lo) != 1)
    {
        printf("\nError: valor incorrecto para A_lo.\n");
        return 1;
    }


    printf("\nNumero B\n");
    printf("--------------------------------------------\n");


    printf("B_hi = 0x");

    if (scanf("%llx", &B.hi) != 1)
    {
        printf("\nError: valor incorrecto para B_hi.\n");
        return 1;
    }


    printf("B_lo = 0x");

    if (scanf("%llx", &B.lo) != 1)
    {
        printf("\nError: valor incorrecto para B_lo.\n");
        return 1;
    }


    printf("\n");
    printf("============================================\n");
    printf("NUMEROS INTRODUCIDOS\n");
    printf("============================================\n");


    printf(
        "A = 0x%016llX%016llX\n",
        A.hi,
        A.lo
    );


    printf(
        "B = 0x%016llX%016llX\n",
        B.hi,
        B.lo
    );


    resultado = multiplicar128(A, B);


    printf("\n");
    printf("============================================\n");
    printf("RESULTADO DE LA MULTIPLICACION\n");
    printf("============================================\n");


    printf("A * B = 0x");

    printf("%016llX", resultado.limb[3]);
    printf("%016llX", resultado.limb[2]);
    printf("%016llX", resultado.limb[1]);
    printf("%016llX", resultado.limb[0]);

    printf("\n");


    printf("\nResultado dividido en palabras de 64 bits:\n");

    printf(
        "R3 [255:192] = 0x%016llX\n",
        resultado.limb[3]
    );

    printf(
        "R2 [191:128] = 0x%016llX\n",
        resultado.limb[2]
    );

    printf(
        "R1 [127:64 ] = 0x%016llX\n",
        resultado.limb[1]
    );

    printf(
        "R0 [63:0   ] = 0x%016llX\n",
        resultado.limb[0]
    );


    imprimir_memoria(
        "RESULTADO",
        &resultado,
        sizeof(resultado)
    );


    return 0;
}