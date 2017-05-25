// Programa XAPE - Robo hexapode
// Projeto de Hardware by Orivaldo
// Software Authors:
// "Dr. Beco" <rcb@beco.cc>
// "Oriva" <orivaldo@gmail.com>
// This example code is in the public domain.

#include <Servo.h> 
#define MXMT   12      /* indices 0-11, motores pino 2-13 */
#define ITERA  3      /* iteracoes para andar */

#define DELAY           5    /* tempo de delay da funcao goangle() */
#define DELAYTRIPOIDE 300    /* tempo de delay entre passos do tripoide */
#define DELAYONDA      50    /* tempo de delay entre passos do onda */

#define MSERIAL   1    /* modo iterativo - serial */
#define MTRIPOIDE 2    /* modo autonomo - andar tripoide */
#define MONDA     3    /* modo autonomo - ondas */
#define MODO MSERIAL   /* definicao do modo */

#define L1A  0     /* ombro frente esquerdo     PINO  2 */
#define L1B  1     /* cotovelo frente esquerdo  PINO  3 */
#define L2A  2     /* ombro meio esquerdo       PINO  4 */
#define L2B  3     /* cotovelo meio esquerdo    PINO  5 */
#define L3A  4     /* ombro tras esquerdo       PINO  6 */
#define L3B  5     /* cotovelo tras esquerdo    PINO  7 */
#define R1A  6     /* ombro frente direito      PINO  8 */
#define R1B  7     /* cotovelo frente direito   PINO  9 */
#define R2A  8     /* ombro meio direito        PINO 10 */
#define R2B  9     /* cotovelo meio direito     PINO 11 */
#define R3A 10     /* ombro tras direito        PINO 12 */
#define R3B 11     /* cotovelo tras direito     PINO 13 */

#define OFE L1A    /* ombro frente esquerdo     PINO  2 */
#define CFE L1B    /* cotovelo frente esquerdo  PINO  3 */
#define OME L2A    /* ombro meio esquerdo       PINO  4 */
#define CME L2B    /* cotovelo meio esquerdo    PINO  5 */
#define OTE L3A    /* ombro tras esquerdo       PINO  6 */
#define CTE L3B    /* cotovelo tras esquerdo    PINO  7 */
#define OFD R1A    /* ombro frente direito      PINO  8 */
#define CFD R1B    /* cotovelo frente direito   PINO  9 */
#define OMD R2A    /* ombro meio direito        PINO 10 */
#define CMD R2B    /* cotovelo meio direito     PINO 11 */
#define OTD R3A    /* ombro tras direito        PINO 12 */
#define CTD R3B    /* cotovelo tras direito     PINO 13 */

#define NER  17   /* numero de estados modo rapido */

int pulse, angle, motor, sign;
Servo s[MXMT];      // doze servomotores de 0 a 11 (pinos de 2 a 13)
int zerograu[MXMT]; // posicao em pulsos calibrada em zero grau para cada motor
int st = 0; //estado do parser
int c2=';', c1=';', c0=';'; // caracter lido: c0 atual, c1 anterior, c2 dois antes

int a2p(int mt, int ag); //converte angulo para pulso
void goangle(int mt, int ag); //gira o motor mt para o angulo ag
int executaletra(char lt); //executa acoes atribuidas a uma letra de atalho
int executamotor(int mt, int ag); //imprime mensagem e chama goangle (pode ser eliminada para usar direto goangle)
int parser_error(int st); //mensagem de erro do parser
void tripoide(void); //anda modo tripoide
int motorletra(int l2, int l1, int l0); //converte codigo de letras em numero do motor
int errecover = 0; //recover from syntax error

int estado_onda_direito [49][MXMT] = {{ -30 , -25 , -6 , -25 , -18 , -25 , 18 , 25 , 5 , 25 , 30 , 25 }, 
{ -26 , -12 , -6 , -25 , -18 , -25 , 17 , 25 , 5 , 25 , 29 , 25 }, 
{ -22 , 0 , -7 , -25 , -19 , -25 , 16 , 25 , 4 , 25 , 28 , 25 }, 
{ -18 , 12 , -8 , -25 , -20 , -25 , 15 , 25 , 3 , 25 , 27 , 25 }, 
{ -15 , 25 , -9 , -25 , -21 , -25 , 15 , 25 , 2 , 25 , 27 , 25 }, 
{ -11 , 12 , -9 , -25 , -21 , -25 , 14 , 25 , 2 , 25 , 26 , 25 }, 
{ -7 , 0 , -10 , -25 , -22 , -25 , 13 , 25 , 1 , 25 , 25 , 25 }, 
{ -3 , -12 , -11 , -25 , -23 , -25 , 12 , 25 , 0 , 25 , 24 , 25 }, 
{ 0 , -25 , -12 , -25 , -24 , -25 , 12 , 25 , 0 , 25 , 24 , 25 }, 
{ 0 , -25 , -12 , -25 , -24 , -25 , 11 , 25 , 3 , 12 , 23 , 25 }, 
{ -1 , -25 , -13 , -25 , -25 , -25 , 10 , 25 , 7 , 0 , 22 , 25 }, 
{ -2 , -25 , -14 , -25 , -26 , -25 , 9 , 25 , 11 , -12 , 21 , 25 }, 
{ -3 , -25 , -15 , -25 , -27 , -25 , 9 , 25 , 15 , -25 , 21 , 25 }, 
{ -3 , -25 , -15 , -25 , -27 , -25 , 8 , 25 , 18 , -12 , 20 , 25 }, 
{ -4 , -25 , -16 , -25 , -28 , -25 , 7 , 25 , 22 , 0 , 19 , 25 }, 
{ -5 , -25 , -17 , -25 , -29 , -25 , 6 , 25 , 26 , 12 , 18 , 25 }, 
{ -6 , -25 , -18 , -25 , -30 , -25 , 5 , 25 , 30 , 25 , 18 , 25 }, 
{ -6 , -25 , -18 , -25 , -26 , -12 , 5 , 25 , 29 , 25 , 17 , 25 }, 
{ -7 , -25 , -19 , -25 , -22 , 0 , 4 , 25 , 28 , 25 , 16 , 25 }, 
{ -8 , -25 , -20 , -25 , -18 , 12 , 3 , 25 , 27 , 25 , 15 , 25 }, 
{ -9 , -25 , -21 , -25 , -15 , 25 , 2 , 25 , 27 , 25 , 15 , 25 }, 
{ -9 , -25 , -21 , -25 , -11 , 12 , 2 , 25 , 26 , 25 , 14 , 25 }, 
{ -10 , -25 , -22 , -25 , -7 , 0 , 1 , 25 , 25 , 25 , 13 , 25 }, 
{ -11 , -25 , -23 , -25 , -3 , -12 , 0 , 25 , 24 , 25 , 12 , 25 }, 
{ -12 , -25 , -24 , -25 , 0 , -25 , 0 , 25 , 24 , 25 , 12 , 25 }, 
{ -12 , -25 , -24 , -25 , 0 , -25 , 3 , 12 , 23 , 25 , 11 , 25 }, 
{ -13 , -25 , -25 , -25 , -1 , -25 , 7 , 0 , 22 , 25 , 10 , 25 }, 
{ -14 , -25 , -26 , -25 , -2 , -25 , 11 , -12 , 21 , 25 , 9 , 25 }, 
{ -15 , -25 , -27 , -25 , -3 , -25 , 15 , -25 , 21 , 25 , 9 , 25 }, 
{ -15 , -25 , -27 , -25 , -3 , -25 , 18 , -12 , 20 , 25 , 8 , 25 }, 
{ -16 , -25 , -28 , -25 , -4 , -25 , 22 , 0 , 19 , 25 , 7 , 25 }, 
{ -17 , -25 , -29 , -25 , -5 , -25 , 26 , 12 , 18 , 25 , 6 , 25 }, 
{ -18 , -25 , -30 , -25 , -6 , -25 , 30 , 25 , 18 , 25 , 5 , 25 }, 
{ -18 , -25 , -26 , -12 , -6 , -25 , 29 , 25 , 17 , 25 , 5 , 25 }, 
{ -19 , -25 , -22 , 0 , -7 , -25 , 28 , 25 , 16 , 25 , 4 , 25 }, 
{ -20 , -25 , -18 , 12 , -8 , -25 , 27 , 25 , 15 , 25 , 3 , 25 }, 
{ -21 , -25 , -15 , 25 , -9 , -25 , 27 , 25 , 15 , 25 , 2 , 25 }, 
{ -21 , -25 , -11 , 12 , -9 , -25 , 26 , 25 , 14 , 25 , 2 , 25 }, 
{ -22 , -25 , -7 , 0 , -10 , -25 , 25 , 25 , 13 , 25 , 1 , 25 }, 
{ -23 , -25 , -3 , -12 , -11 , -25 , 24 , 25 , 12 , 25 , 0 , 25 }, 
{ -24 , -25 , 0 , -25 , -12 , -25 , 24 , 25 , 12 , 25 , 0 , 25 }, 
{ -24 , -25 , 0 , -25 , -12 , -25 , 23 , 25 , 11 , 25 , 3 , 12 }, 
{ -25 , -25 , -1 , -25 , -13 , -25 , 22 , 25 , 10 , 25 , 7 , 0 }, 
{ -26 , -25 , -2 , -25 , -14 , -25 , 21 , 25 , 9 , 25 , 11 , -12 }, 
{ -27 , -25 , -3 , -25 , -15 , -25 , 21 , 25 , 9 , 25 , 15 , -25 }, 
{ -27 , -25 , -3 , -25 , -15 , -25 , 20 , 25 , 8 , 25 , 18 , -12 }, 
{ -28 , -25 , -4 , -25 , -16 , -25 , 19 , 25 , 7 , 25 , 22 , 0 }, 
{ -29 , -25 , -5 , -25 , -17 , -25 , 18 , 25 , 6 , 25 , 26 , 12 }, 
{ -30 , -25 , -6 , -25 , -18 , -25 , 18 , 25 , 5 , 25 , 30 , 25 }};

int estado_onda_esquerdo [49][MXMT] = { { 0 , -25 , -24 , -25 , -12 , -25 , 12 , 25 , 24 , 25 , 0 , 25 }, 
{ -3 , -12 , -23 , -25 , -11 , -25 , 12 , 25 , 24 , 25 , 0 , 25 }, 
{ -7 , 0 , -22 , -25 , -10 , -25 , 13 , 25 , 25 , 25 , 1 , 25 }, 
{ -11 , 12 , -21 , -25 , -9 , -25 , 14 , 25 , 26 , 25 , 2 , 25 }, 
{ -15 , 25 , -21 , -25 , -9 , -25 , 15 , 25 , 27 , 25 , 3 , 25 }, 
{ -18 , 12 , -20 , -25 , -8 , -25 , 15 , 25 , 27 , 25 , 3 , 25 }, 
{ -22 , 0 , -19 , -25 , -7 , -25 , 16 , 25 , 28 , 25 , 4 , 25 }, 
{ -26 , -12 , -18 , -25 , -6 , -25 , 17 , 25 , 29 , 25 , 5 , 25 }, 
{ -30 , -25 , -18 , -25 , -5 , -25 , 18 , 25 , 30 , 25 , 6 , 25 }, 
{ -29 , -25 , -17 , -25 , -5 , -25 , 18 , 25 , 26 , 12 , 6 , 25 }, 
{ -28 , -25 , -16 , -25 , -4 , -25 , 19 , 25 , 22 , 0 , 7 , 25 }, 
{ -27 , -25 , -15 , -25 , -3 , -25 , 20 , 25 , 18 , -12 , 8 , 25 }, 
{ -27 , -25 , -15 , -25 , -2 , -25 , 21 , 25 , 15 , -25 , 9 , 25 }, 
{ -26 , -25 , -14 , -25 , -2 , -25 , 21 , 25 , 11 , -12 , 9 , 25 }, 
{ -25 , -25 , -13 , -25 , -1 , -25 , 22 , 25 , 7 , 0 , 10 , 25 }, 
{ -24 , -25 , -12 , -25 , 0 , -25 , 23 , 25 , 3 , 12 , 11 , 25 }, 
{ -24 , -25 , -12 , -25 , 0 , -25 , 24 , 25 , 0 , 25 , 12 , 25 }, 
{ -23 , -25 , -11 , -25 , -3 , -12 , 24 , 25 , 0 , 25 , 12 , 25 }, 
{ -22 , -25 , -10 , -25 , -7 , 0 , 25 , 25 , 1 , 25 , 13 , 25 }, 
{ -21 , -25 , -9 , -25 , -11 , 12 , 26 , 25 , 2 , 25 , 14 , 25 }, 
{ -21 , -25 , -9 , -25 , -15 , 25 , 27 , 25 , 3 , 25 , 15 , 25 }, 
{ -20 , -25 , -8 , -25 , -18 , 12 , 27 , 25 , 3 , 25 , 15 , 25 }, 
{ -19 , -25 , -7 , -25 , -22 , 0 , 28 , 25 , 4 , 25 , 16 , 25 }, 
{ -18 , -25 , -6 , -25 , -26 , -12 , 29 , 25 , 5 , 25 , 17 , 25 }, 
{ -18 , -25 , -5 , -25 , -30 , -25 , 30 , 25 , 6 , 25 , 18 , 25 }, 
{ -17 , -25 , -5 , -25 , -29 , -25 , 26 , 12 , 6 , 25 , 18 , 25 }, 
{ -16 , -25 , -4 , -25 , -28 , -25 , 22 , 0 , 7 , 25 , 19 , 25 }, 
{ -15 , -25 , -3 , -25 , -27 , -25 , 18 , -12 , 8 , 25 , 20 , 25 }, 
{ -15 , -25 , -2 , -25 , -27 , -25 , 15 , -25 , 9 , 25 , 21 , 25 }, 
{ -14 , -25 , -2 , -25 , -26 , -25 , 11 , -12 , 9 , 25 , 21 , 25 }, 
{ -13 , -25 , -1 , -25 , -25 , -25 , 7 , 0 , 10 , 25 , 22 , 25 }, 
{ -12 , -25 , 0 , -25 , -24 , -25 , 3 , 12 , 11 , 25 , 23 , 25 }, 
{ -12 , -25 , 0 , -25 , -24 , -25 , 0 , 25 , 12 , 25 , 24 , 25 }, 
{ -11 , -25 , -3 , -12 , -23 , -25 , 0 , 25 , 12 , 25 , 24 , 25 }, 
{ -10 , -25 , -7 , 0 , -22 , -25 , 1 , 25 , 13 , 25 , 25 , 25 }, 
{ -9 , -25 , -11 , 12 , -21 , -25 , 2 , 25 , 14 , 25 , 26 , 25 }, 
{ -9 , -25 , -15 , 25 , -21 , -25 , 3 , 25 , 15 , 25 , 27 , 25 }, 
{ -8 , -25 , -18 , 12 , -20 , -25 , 3 , 25 , 15 , 25 , 27 , 25 }, 
{ -7 , -25 , -22 , 0 , -19 , -25 , 4 , 25 , 16 , 25 , 28 , 25 }, 
{ -6 , -25 , -26 , -12 , -18 , -25 , 5 , 25 , 17 , 25 , 29 , 25 }, 
{ -5 , -25 , -30 , -25 , -18 , -25 , 6 , 25 , 18 , 25 , 30 , 25 }, 
{ -5 , -25 , -29 , -25 , -17 , -25 , 6 , 25 , 18 , 25 , 26 , 12 }, 
{ -4 , -25 , -28 , -25 , -16 , -25 , 7 , 25 , 19 , 25 , 22 , 0 }, 
{ -3 , -25 , -27 , -25 , -15 , -25 , 8 , 25 , 20 , 25 , 18 , -12 }, 
{ -2 , -25 , -27 , -25 , -15 , -25 , 9 , 25 , 21 , 25 , 15 , -25 }, 
{ -2 , -25 , -26 , -25 , -14 , -25 , 9 , 25 , 21 , 25 , 11 , -12 }, 
{ -1 , -25 , -25 , -25 , -13 , -25 , 10 , 25 , 22 , 25 , 7 , 0 }, 
{ 0 , -25 , -24 , -25 , -12 , -25 , 11 , 25 , 23 , 25 , 3 , 12 }, 
{ 0 , -25 , -24 , -25 , -12 , -25 , 12 , 25 , 24 , 25 , 0 , 25 } };

int estadonda [49][MXMT] = {{ -45 , -30 , -9 , -30 , -27 , -30 , 18 , 30 , 36 , 30 , 0 , 30 }, 
{ -39 , -17 , -10 , -30 , -28 , -30 , 19 , 30 , 37 , 30 , 1 , 30 }, 
{ -33 , -5 , -11 , -30 , -29 , -30 , 20 , 30 , 38 , 30 , 2 , 30 }, 
{ -28 , 7 , -12 , -30 , -30 , -30 , 21 , 30 , 39 , 30 , 3 , 30 }, 
{ -22 , 20 , -13 , -30 , -31 , -30 , 22 , 30 , 40 , 30 , 4 , 30 }, 
{ -16 , 7 , -14 , -30 , -32 , -30 , 23 , 30 , 41 , 30 , 5 , 30 }, 
{ -11 , -5 , -15 , -30 , -33 , -30 , 24 , 30 , 42 , 30 , 6 , 30 }, 
{ -5 , -17 , -16 , -30 , -34 , -30 , 25 , 30 , 43 , 30 , 7 , 30 }, 
{ 0 , -30 , -18 , -30 , -36 , -30 , 27 , 30 , 45 , 30 , 9 , 30 }, 
{ -1 , -30 , -19 , -30 , -37 , -30 , 28 , 30 , 39 , 17 , 10 , 30 }, 
{ -2 , -30 , -20 , -30 , -38 , -30 , 29 , 30 , 33 , 5 , 11 , 30 }, 
{ -3 , -30 , -21 , -30 , -39 , -30 , 30 , 30 , 28 , -7 , 12 , 30 }, 
{ -4 , -30 , -22 , -30 , -40 , -30 , 31 , 30 , 22 , -20 , 13 , 30 }, 
{ -5 , -30 , -23 , -30 , -41 , -30 , 32 , 30 , 16 , -7 , 14 , 30 }, 
{ -6 , -30 , -24 , -30 , -42 , -30 , 33 , 30 , 11 , 5 , 15 , 30 }, 
{ -7 , -30 , -25 , -30 , -43 , -30 , 34 , 30 , 5 , 17 , 16 , 30 }, 
{ -9 , -30 , -27 , -30 , -45 , -30 , 36 , 30 , 0 , 30 , 18 , 30 }, 
{ -10 , -30 , -28 , -30 , -39 , -17 , 37 , 30 , 1 , 30 , 19 , 30 }, 
{ -11 , -30 , -29 , -30 , -33 , -5 , 38 , 30 , 2 , 30 , 20 , 30 }, 
{ -12 , -30 , -30 , -30 , -28 , 7 , 39 , 30 , 3 , 30 , 21 , 30 }, 
{ -13 , -30 , -31 , -30 , -22 , 20 , 40 , 30 , 4 , 30 , 22 , 30 }, 
{ -14 , -30 , -32 , -30 , -16 , 7 , 41 , 30 , 5 , 30 , 23 , 30 }, 
{ -15 , -30 , -33 , -30 , -11 , -5 , 42 , 30 , 6 , 30 , 24 , 30 }, 
{ -16 , -30 , -34 , -30 , -5 , -17 , 43 , 30 , 7 , 30 , 25 , 30 }, 
{ -18 , -30 , -36 , -30 , 0 , -30 , 45 , 30 , 9 , 30 , 27 , 30 }, 
{ -19 , -30 , -37 , -30 , -1 , -30 , 39 , 17 , 10 , 30 , 28 , 30 }, 
{ -20 , -30 , -38 , -30 , -2 , -30 , 33 , 5 , 11 , 30 , 29 , 30 }, 
{ -21 , -30 , -39 , -30 , -3 , -30 , 28 , -7 , 12 , 30 , 30 , 30 }, 
{ -22 , -30 , -40 , -30 , -4 , -30 , 22 , -20 , 13 , 30 , 31 , 30 }, 
{ -23 , -30 , -41 , -30 , -5 , -30 , 16 , -7 , 14 , 30 , 32 , 30 }, 
{ -24 , -30 , -42 , -30 , -6 , -30 , 11 , 5 , 15 , 30 , 33 , 30 }, 
{ -25 , -30 , -43 , -30 , -7 , -30 , 5 , 17 , 16 , 30 , 34 , 30 }, 
{ -27 , -30 , -45 , -30 , -9 , -30 , 0 , 30 , 18 , 30 , 36 , 30 }, 
{ -28 , -30 , -39 , -17 , -10 , -30 , 1 , 30 , 19 , 30 , 37 , 30 }, 
{ -29 , -30 , -33 , -5 , -11 , -30 , 2 , 30 , 20 , 30 , 38 , 30 }, 
{ -30 , -30 , -28 , 7 , -12 , -30 , 3 , 30 , 21 , 30 , 39 , 30 }, 
{ -31 , -30 , -22 , 20 , -13 , -30 , 4 , 30 , 22 , 30 , 40 , 30 }, 
{ -32 , -30 , -16 , 7 , -14 , -30 , 5 , 30 , 23 , 30 , 41 , 30 }, 
{ -33 , -30 , -11 , -5 , -15 , -30 , 6 , 30 , 24 , 30 , 42 , 30 }, 
{ -34 , -30 , -5 , -17 , -16 , -30 , 7 , 30 , 25 , 30 , 43 , 30 }, 
{ -36 , -30 , 0 , -30 , -18 , -30 , 9 , 30 , 27 , 30 , 45 , 30 }, 
{ -37 , -30 , -1 , -30 , -19 , -30 , 10 , 30 , 28 , 30 , 39 , 17 }, 
{ -38 , -30 , -2 , -30 , -20 , -30 , 11 , 30 , 29 , 30 , 33 , 5 }, 
{ -39 , -30 , -3 , -30 , -21 , -30 , 12 , 30 , 30 , 30 , 28 , -7 }, 
{ -40 , -30 , -4 , -30 , -22 , -30 , 13 , 30 , 31 , 30 , 22 , -20 }, 
{ -41 , -30 , -5 , -30 , -23 , -30 , 14 , 30 , 32 , 30 , 16 , -7 }, 
{ -42 , -30 , -6 , -30 , -24 , -30 , 15 , 30 , 33 , 30 , 11 , 5 }, 
{ -43 , -30 , -7 , -30 , -25 , -30 , 16 , 30 , 34 , 30 , 5 , 17 }, 
{ -45 , -30 , -9 , -30 , -27 , -30 , 18 , 30 , 36 , 30 , 0 , 30 }};


int estado_rapido [NER][MXMT] = {{ -45 , -30 , 0 , -30 , -45 , -30 , 0 , 30 , 45 , 30 , 0 , 30 }, 
{ -39 , -17 , -5 , -30 , -39 , -17 , 5 , 30 , 39 , 17 , 5 , 30 }, 
{ -33 , -5 , -11 , -30 , -33 , -5 , 11 , 30 , 33 , 5 , 11 , 30 }, 
{ -28 , 7 , -16 , -30 , -28 , 7 , 16 , 30 , 28 , -7 , 16 , 30 }, 
{ -22 , 20 , -22 , -30 , -22 , 20 , 22 , 30 , 22 , -20 , 22 , 30 }, 
{ -16 , 7 , -28 , -30 , -16 , 7 , 28 , 30 , 16 , -7 , 28 , 30 }, 
{ -11 , -5 , -33 , -30 , -11 , -5 , 33 , 30 , 11 , 5 , 33 , 30 }, 
{ -5 , -17 , -39 , -30 , -5 , -17 , 39 , 30 , 5 , 17 , 39 , 30 }, 
{ 0 , -30 , -45 , -30 , 0 , -30 , 45 , 30 , 0 , 30 , 45 , 30 }, 
{ -5 , -30 , -39 , -17 , -5 , -30 , 39 , 17 , 5 , 30 , 39 , 17 }, 
{ -11 , -30 , -33 , -5 , -11 , -30 , 33 , 5 , 11 , 30 , 33 , 5 }, 
{ -16 , -30 , -28 , 7 , -16 , -30 , 28 , -7 , 16 , 30 , 28 , -7 }, 
{ -22 , -30 , -22 , 20 , -22 , -30 , 22 , -20 , 22 , 30 , 22 , -20 }, 
{ -28 , -30 , -16 , 7 , -28 , -30 , 16 , -7 , 28 , 30 , 16 , -7 }, 
{ -33 , -30 , -11 , -5 , -33 , -30 , 11 , 5 , 33 , 30 , 11 , 5 }, 
{ -39 , -30 , -5 , -17 , -39 , -30 , 5 , 17 , 39 , 30 , 5 , 17 }, 
{ -45 , -30 , 0 , -30 , -45 , -30 , 0 , 30 , 45 , 30 , 0 , 30 }};

void setup() 
{ 
  int i;
  Serial.begin(9600); 
  Serial.println("Calibrating..."); 
  
  // configura motores para a posicao HOME
  zerograu[L1A] =  95;     //ombro frente esquerdo      OFE
  zerograu[L1B] = 115;     //cotovelo frente esquerdo   CFE
  zerograu[L2A] =  90;     //ombro meio esquerdo        OME
  zerograu[L2B] = 100;     //cotovelo meio esquerdo     CME
  zerograu[L3A] =  70;     //ombro tras esquerdo        OTE
  zerograu[L3B] =  90;     //cotovelo tras esquerdo     CTE
  zerograu[R1A] =  75;     //ombro frente direito       OFD
  zerograu[R1B] =  90;     //cotovelo frente direito    CFD
  zerograu[R2A] =  90;     //ombro meio direito         OMD
  zerograu[R2B] = 115;     //cotovelo meio direito      CMD
  zerograu[R3A] = 100;     //ombro tras direito         OTD
  zerograu[R3B] =  95;     //cotovelo tras direito      CTD
  
  Serial.println("Attaching..."); 
  for(i=0; i<MXMT; i++)
    s[i].attach(i+2);  // attaches servo i on pin i+1

  Serial.println("Going HOME..."); 
  poshome();  

  Serial.println("Looping..."); 
  Serial.println("Type MOTOR +/- ANGLE;"); 
  Serial.println("MOTOR: number from 0 to 11 or letter code [LF][123][AB] or portuguese code [OC][FMT][ED]"); 
  Serial.println("ANGLE: number from -80 to +80 "); 
} 

int parser_error(int state)
{
  if(!errecover)
  {
    Serial.print("Error! Usage: ([A-G];) | ([0-13](+|-)[0-80];) State: ");
    Serial.println(state);
    errecover=1;
  }
  return -1;
}

int executaletra(char l)
{
  int i;
  Serial.print("Command ");
  Serial.println(l);
  switch(l)
  {
    case 'I': //modo tripoide A: levanta patas 2e1d
      goangle(L1B,30);
      goangle(L3B,30);
      goangle(R2B,-30);
      break;
    case 'O': //modo tripoide B: para tras patas 1e2d
      goangle(L2A,-30);
      goangle(R1A,30);
      goangle(R3A,30);
      break;
    case 'P': //modo tripoide C: para frente patas 2e1d
      goangle(L1A,30);
      goangle(L3A,30);
      goangle(R2A,-30);
      break;
    case 'J': //modo tripoide D: baixa patas 2e1d
      goangle(L1B,-30);
      goangle(L3B,-30);
      goangle(R2B,30);
      break;
    case 'K': //modo tripoide E: levanta patas 1e2d
      goangle(L2B,30);
      goangle(R1B,-30);
      goangle(R3B,-30);
      break;
    case 'L': //modo tripoide F: para tras patas 2e1d
      goangle(L1A,-30);
      goangle(L3A,-30);
      goangle(R2A,30);
      break;
    case 'B': //modo tripoide G: para frente patas 1e2d
      goangle(L2A,30);
      goangle(R1A,-30);
      goangle(R3A,-30);
      break;
    case 'N': //modo tripoide  H: baixa patas 1e2d
      goangle(L2B,-30);
      goangle(R1B,30);
      goangle(R3B,30);
      break;
    case 'M': //modo tripoide: executa tripoid por UMA vez (IOP,JKL,BNM)
        tripoide();
      break;
    case 'H': // coloca na posicao HOME (todos angulos ZERO)
      poshome();
      break;
    case 'G': // roda modo TRIPOIDE por ITERA vezes
      for(i=0; i<ITERA; i++)
        tripoide();
       break;
    case 'F': // roda modo ONDA para frente por ITERA vezes
      for(i=0; i<ITERA; i++)
        onda();
      break;
    case 'E': // roda modo onda para esquerda por ITERA vezes
      for(i=0; i<ITERA; i++)
        onda_esquerda();
      break;
    case 'D': // roda modo onda para direita por ITERA vezes
      for(i=0; i<ITERA; i++)
        onda_direita();
      break;      
    case 'T': // roda modo onda para tras por ITERA vezes
      for(i=0; i<ITERA; i++)
        onda_re();
      break;   
   case 'R': // roda modo locomocao rapido por ITERA vezes    
      for(i=0; i<ITERA; i++)
        modo_rapido();
      break;  

    // livres para uso futuro      
    case 'A':
    case 'C':
    case 'Q':
    case 'S':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
    default:    
      Serial.println("Wrong command letter");
  }
  return 0;
}

int executamotor(int mt, int ag)
{
  Serial.println("Execute motor: ");
  //Serial.print(mt);
  //Serial.println(ag);
  goangle(mt, ag);
  return 0;
}

int motorletra(int l2, int l1, int l0)
{
  int mt; //default error!
  switch(l2)
  {
    case 'L':
    case 'R': //modo ingles
      switch(l0)
      {
        case 'A':
                  switch(l1)
                  {
                    case '1':
                              switch(l2)
                              {
                                case 'L':
                                          mt=L1A;
                                          break;
                                case 'R':
                                          mt=R1A;
                                          break;
                                default:    
                                  Serial.println("Wrong code [LR].");
                              }
                              break;
                    case '2':
                              switch(l2)
                              {
                                case 'L':
                                          mt=L2A;
                                          break;
                                case 'R':
                                          mt=R2A;
                                          break;
                                default:    
                                  Serial.println("Wrong code [LR].");
                              }
                              break;
                    case '3':
                              switch(l2)
                              {
                                case 'L':
                                          mt=L3A;
                                          break;
                                case 'R':
                                          mt=R3A;
                                          break;
                                default:    
                                  Serial.println("Wrong code [LR].");
                              }
                              break;
                    default:    
                      Serial.println("Wrong code [123].");
                  }
                  break;
        case 'B':
                  switch(l1)
                  {
                    case '1':
                              switch(l2)
                              {
                                case 'L':
                                          mt=L1B;
                                          break;
                                case 'R':
                                          mt=R1B;
                                          break;
                                default:    
                                  Serial.println("Wrong code [LR].");
                              }
                              break;
                    case '2':
                              switch(l2)
                              {
                                case 'L':
                                          mt=L2B;
                                          break;
                                case 'R':
                                          mt=R2B;
                                          break;
                                default:    
                                  Serial.println("Wrong code [LR].");
                              }
                              break;
                    case '3':
                              switch(l2)
                              {
                                case 'L':
                                          mt=L3B;
                                          break;
                                case 'R':
                                          mt=R3B;
                                          break;
                                default:    
                                  Serial.println("Wrong code [LR].");
                              }
                              break;
                    default:    
                      Serial.println("Wrong code [123].");
                  }
                  break;
        default:    
          Serial.println("Wrong code [AB].");
      }
      break;
    case 'O':
    case 'C': //modo portugues
      switch(l0)
      {
        case 'E':
                  switch(l1)
                  {
                    case 'F':
                              switch(l2)
                              {
                                case 'O':
                                          mt=OFE;
                                          break;
                                case 'C':
                                          mt=CFE;
                                          break;
                                default:    
                                  Serial.println("Wrong code [OC].");
                              }
                              break;
                    case 'M':
                              switch(l2)
                              {
                                case 'O':
                                          mt=OME;
                                          break;
                                case 'C':
                                          mt=CME;
                                          break;
                                default:    
                                  Serial.println("Wrong code [OC].");
                              }
                              break;
                    case 'T':
                              switch(l2)
                              {
                                case 'O':
                                          mt=OTE;
                                          break;
                                case 'C':
                                          mt=CTE;
                                          break;
                                default:    
                                  Serial.println("Wrong code [OC].");
                              }
                              break;
                    default:    
                      Serial.println("Wrong code [FMT].");
                  }
                  break;
        case 'D':
                  switch(l1)
                  {
                    case 'F':
                              switch(l2)
                              {
                                case 'O':
                                          mt=OFD;
                                          break;
                                case 'C':
                                          mt=CFD;
                                          break;
                                default:    
                                  Serial.println("Wrong code [OC].");
                              }
                              break;
                    case 'M':
                              switch(l2)
                              {
                                case 'O':
                                          mt=OMD;
                                          break;
                                case 'C':
                                          mt=CMD;
                                          break;
                                default:    
                                  Serial.println("Wrong code [OC].");
                              }
                              break;
                    case 'T':
                              switch(l2)
                              {
                                case 'O':
                                          mt=OTD;
                                          break;
                                case 'C':
                                          mt=CTD;
                                          break;
                                default:    
                                  Serial.println("Wrong code [OC].");
                              }
                              break;
                    default:    
                      Serial.println("Wrong code [FMT].");
                  }
                  break;
        default:    
          Serial.println("Wrong code [ED].");
      }
      break;
    default:
        Serial.println("Wrong code mode (english/portuguese)");
  }
  return mt;
}

void loop(void) 
{  

 if(MODO==MTRIPOIDE)
   tripoide();
 else
   if(MODO==MONDA)
     onda();
   else   
      if(Serial.available() >= 1)
      {
        c2 = c1;
        c1 = c0;
        c0 = Serial.read();
        switch(st)
        {
          case  -1: //error
                  if( c0!=';' )
                  {
                    if(!errecover)
                    {
                      Serial.println("Error state! Type ';'");
                      errecover=1;
                    }
                    st=parser_error(-1);
                  }
                  else
                    st=0;
                  break;
          case 0: //estado inicial
                  errecover = 0;
                  Serial.println("Type command...");
                  if( (c0<'0') || (c0>'9' && c0<'A') || (c0>'Z') )
                    st=parser_error(0);
                  else
                    if(c0>='A' && c0!='O' && c0!='C' && c0!='L' && c0!='R')
                      st=1;
                    else
                      if(c0=='O' || c0=='C')
                        st=9;
                      else
                        if(c0=='L' || c0=='R')
                          st=11;
                        else
                          if(c0=='0')
                            st=2;
                          else
                            if(c0=='1')
                              st=3;
                            else //c0==2 || c0==9
                            {
                              motor=c0-'0'; //numero motor
                              st=4;
                            }
                  break;
          case  1:
                  if(c0!=';')
                    st=parser_error(1);
                  else
                    st=executaletra(c1);
                  break;
          case  2:
                  if( (c0<'0' || c0>'9') && c0!='+' && c0!='-' )
                    st=parser_error(2);
                  else
                    if( c0=='+' || c0=='-' )
                    {
                      motor=0;
                      sign=(c0=='-'?-1:1);
                      st=5;
                    }
                    else //c0 == 0 ... 9
                    {
                      motor=c0-'0'; // numero do motor
                      st=4;
                    }
                  break;
          case  3:
                  if( c0!='0' && c0!='1' && c0!='+' && c0!='-' )
                    st=parser_error(3);
                  else
                    if( c0=='+' || c0=='-' )
                    {
                      sign=(c0=='-'?-1:1);
                      motor=1;
                      st=5;
                    }
                    else //c0 == 0 ou 1
                    {
                      motor=c0-'0'+10; // numero do motor
                      st=4;
                    }
                  break;
          case  4:
                  if( c0!='+' && c0!='-')
                    st=parser_error(4);
                  else
                  {
                    sign=(c0=='-'?-1:1);
                    st=5;
                  }
                  break;
          case  5:
                  if(c0=='9')
                  {
                    angle=sign*9; //numero do angulo
                    st=8;
                  }
                  else
                    if(c0=='8')
                      st=7;
                    else
                      if(c0>='0' && c0<='7')
                        st=6;
                      else
                        st=parser_error(5);
                  break;
          case  6:
                  if( c0==';')
                  {
                    angle=sign*(c1-'0'); //numero do angulo
                    st=executamotor(motor,angle);
                  } 
                  else
                    if( c0<'0' || c0>'9' )
                      st=parser_error(6);
                    else
                    {
                      angle=sign*((c1-'0')*10 + c0-'0'); // numero do angulo
                      st=8;
                    }
                  break;
          case  7:
                  if( c0==';' )
                  {
                    angle=sign*8; //numero do angulo
                    st=executamotor(motor,angle);
                  } 
                  else
                    if( c0=='0' )
                    {
                      angle=sign*80; // numero do angulo
                      st=8;
                    }
                    else
                      st=parser_error(7);
                  break;
          case  8:
                  if( c0!=';' )
                    st=parser_error(8);
                  else
                    st=executamotor(motor,angle);
                  break;
          case 9:
                  if( c0!='F' && c0!='M' && c0!='T' && c0!=';')
                    st=parser_error(9);
                  else
                    if( c0==';')
                      st=executaletra(c1); //O; ou C;
                    else //F, M ou T
                      st=10;
                  break;
          case 10:
                  if( c0!='E' && c0!='D')
                    st=parser_error(10);
                  else
                  {
                    motor=motorletra(c2, c1, c0);
                    st=4;
                  }
                  break;
          case 11:
                  if( c0!='1' && c0!='2' && c0!='3' && c0!=';')
                    st=parser_error(11);
                  else
                    if( c0==';')
                      st=executaletra(c1); //L; ou R;
                    else //1, 2 ou 3
                      st=12;
                  break;
          case 12:
                  if( c0!='A' && c0!='B')
                    st=parser_error(12);
                  else
                  {
                    motor=motorletra(c2, c1, c0);
                    st=4;
                  }
                  break;
          default:
            Serial.println("Error! Bug in the parser: wrong state!");
        }
      } //fim if(.available()>=1)
}

//Funcao goangle
//Move o motor m [0,11] para o angulo a [-80,+80]
void goangle(int mt, int ag)
{
  int pul;
  pul = a2p(mt, ag); //converte angulo em pulso, para o motor i
  Serial.print("Motor "); 
  Serial.print(mt); 
  Serial.print(", Angle "); 
  Serial.print(ag); 
  Serial.print(", pulse "); 
  Serial.println(pul); 
  s[mt].write(pul);
  delay(DELAY);
}  

//Funcao poshome
//Coloca os motores na posicao HOME
void poshome(void)
{
  int i;
  for (i=0; i < MXMT; i++)
  {
    s[i].write(zerograu[i]);              // tell servo to go to zerograuition in variable 'zerograu' 
    delay(DELAY);
  }
}  

//Funcao a2p
//Converte o angulo a (de -80 a +80) em um pulso (de 000 a 200)
//respeitando a calibragem dada em zerograu[i], do motor i
int a2p(int mt, int ag)
{
  int pul;
  if(mt>5) //motores da direita, inverte sinal
    ag *= -1;
  pul = zerograu[mt]+ag;
  if(pul<0)
  {
    Serial.println("Warning! Pulse < 0! Rounded to 0."); 
    return 0;
  }
  if(pul>200)
  {
    Serial.println("Warning! Pulse > 200! Rounded to 200."); 
    return 200;
  }
  return pul;
}

void tripoide(void)
{
      //A: levanta patas 2e1d
      goangle(L1B,30);
      goangle(L3B,30);
      goangle(R2B,-30);
      delay(DELAYTRIPOIDE);
      //B: para tras patas 1e2d
      goangle(L2A,-30);
      goangle(R1A,30);
      goangle(R3A,30);
      //C para frente patas 2e1d
      goangle(L1A,30);
      goangle(L3A,30);
      goangle(R2A,-30);
      delay(DELAYTRIPOIDE);
      //D: baixa patas 2e1d
      goangle(L1B,-30);
      goangle(L3B,-30);
      goangle(R2B,30);
      //E: levanta patas 1e2d
      goangle(L2B,30);
      goangle(R1B,-30);
      goangle(R3B,-30);
      delay(DELAYTRIPOIDE);
      //F: para tras patas 2e1d
      goangle(L1A,-30);
      goangle(L3A,-30);
      goangle(R2A,30);
      //G: para frente patas 1e2d
      goangle(L2A,30);
      goangle(R1A,-30);
      goangle(R3A,-30);
      delay(DELAYTRIPOIDE);
      //H: baixa patas 1e2d
      goangle(L2B,-30);
      goangle(R1B,30);
      goangle(R3B,30);
}  

void onda(void)
{
  int sto; //estado da onda
  int mt; //motor

  for (sto=0; sto<48; sto++) 
  { 
    for (mt=0; mt<MXMT; mt++)
      s[mt].write(zerograu[mt] + estadonda[sto][mt]); 
      //goangle(mt, estadonda[sto][mt]); //go to position 'pos'
    delay(DELAYONDA);  //waits DELAY ms for the servo to reach the position
  }
}

void onda_re(void)
{
  int sto; //estado da onda
  int mt; //motor

  for (sto=48; sto>=0; sto--) 
  { 
    for (mt=0; mt<MXMT; mt++)
      s[mt].write(zerograu[mt] + estadonda[sto][mt]); 
      //goangle(mt, estadonda[sto][mt]); //go to position 'pos'
    delay(DELAYONDA);  //waits DELAY ms for the servo to reach the position
  }
}

void onda_esquerda(void)
{
  int sto; //estado da onda
  int mt; //motor

  for (sto=0; sto<48; sto++) 
  { 
    for (mt=0; mt<MXMT; mt++)
      s[mt].write(zerograu[mt] + estado_onda_esquerdo[sto][mt]);  
      //goangle(mt, estado_onda_esquerdo[sto][mt]); //go to position 'pos'
    delay(DELAYONDA);  //waits DELAY ms for the servo to reach the position
  }
}

void onda_direita(void)
{
  int sto; //estado da onda
  int mt; //motor

  for (sto=0; sto<48; sto++) 
  { 
    for (mt=0; mt<MXMT; mt++)
      s[mt].write(zerograu[mt] + estado_onda_direito[sto][mt]);  
      //goangle(mt, estado_onda_esquerdo[sto][mt]); //go to position 'pos'
    delay(DELAYONDA);  //waits DELAY ms for the servo to reach the position
  }
}

void modo_rapido(void)
{
  int sto; //estado da onda
  int mt; //motor

  for (sto=0; sto<NER; sto++) 
  { 
    for (mt=0; mt<MXMT; mt++)
      s[mt].write(zerograu[mt] + estado_rapido[sto][mt]);  
      //goangle(mt, estado_onda_esquerdo[sto][mt]); //go to position 'pos'
    delay(120);  //waits DELAY ms for the servo to reach the position
  }
}

