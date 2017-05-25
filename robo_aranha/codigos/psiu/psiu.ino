/******************************************************************
                          PSIU PROTOCOL

 * Copyright (C) Ricardo Mariz and Robolivre.org 2012 <ricardo@robolivre.org>
 * This is a free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * PSIU PROTOOCOL is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *  
                       
 * data:  02/03/2012
 * Projeto realizado com fundos do Conselho Nacional de Desenvolvimento Científico e Tecnológico (CNPQ)   
                          
                          
Alguns exemplos de comandos que irao funcionar nessa versao:

MNERIM 034 parafrente 100 PC 02123
MNERIM 032 paratras 100 PC 01919
MNERIM 035 giradireita 100 PC 02217
MNERIM 036 giraesquerda 100 PC 02338
MNERIM 035 quantoscomandos PC 02514
MNERIM 034 exibecomando 1 PC 02225
?? 029 qualseunome PC 01755


******************************************************************/
// Codigo do Robo Aranha
#include <Servo.h> 
#define MXMT   12      /* indices 0-11, motores pino 2-13 */
#define ITERA  3      /* iteracoes para andar */

#define DELAY           5    /* tempo de delay da funcao goangle() */
#define DELAYTRIPOIDE 300    /* tempo de delay entre passos do tripoide */
#define DELAYONDA      40    /* tempo de delay entre passos do onda */

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


 


char nome[] = "Aranha";
char caractere, checkSum[6], tamanho[4], bufferComando[20], bufferParametro[20], remetente[20];
int contByte = 0, soma = 0;
long icheckSum;
  

//VARIAVEIS AUXILIARES
int validouComando = 0, comandoOk = 0 , criaRemetente = 0;
int qntInt, qntFloat, qntChar, numComando;
int perguntaNome = 0;
int posicao = 0;



//PONTEIROS PARA A CRIACAO DOS VETORES COM OS PARAMETROS DOS COMANDOS
int *parametroInt;
float *parametroFloat;
char *parametroChar;




//DEFINE QUANTIDADE DE COMANDOS DO MICROCONTROLADOR
#define qntComandos 7


//ESTRUTURA DE COMANDO
struct
{
  char nome[15];
  int parametro1, parametro2, parametro3;

} comando[qntComandos]; //Vetor com a quantidade de comandos.


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


void setup()
{
    int i;
  Serial3.begin(9600); 
  Serial3.println("Calibrating..."); 
  
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
  
  Serial3.println("Attaching..."); 
  for(i=0; i<MXMT; i++)
    s[i].attach(i+2);  // attaches servo i on pin i+1

  Serial3.println("Going HOME..."); 
  poshome();  
  
  //LISTA DE COMANDOS PREVIAMENTE DECLARADOS
  
  //Aqui é declarado os comandos que o microcontrolador aceitará.
  
  strcpy(comando[0].nome , "parafrente");
  comando[0].parametro1 = 1;
  comando[0].parametro2 = 0;
  comando[0].parametro3 = 0;
  
  strcpy(comando[1].nome , "paratras");
  comando[1].parametro1 = 1;
  comando[1].parametro2 = 0;
  comando[1].parametro3 = 0;  
   
  strcpy(comando[2].nome , "giradireita");
  comando[2].parametro1 = 1;
  comando[2].parametro2 = 0;
  comando[2].parametro3 = 0;
   
   
  strcpy(comando[3].nome , "giraesquerda");
  comando[3].parametro1 = 1;
  comando[3].parametro2 = 0;
  comando[3].parametro3 = 0;
  
  strcpy(comando[4].nome , "quantoscomandos");
  comando[4].parametro1 = 0;
  comando[4].parametro2 = 0;
  comando[4].parametro3 = 0;
  
  strcpy(comando[5].nome , "exibecomando");
  comando[5].parametro1 = 1;
  comando[5].parametro2 = 0;
  comando[5].parametro3 = 0;
  
  
  strcpy(comando[6].nome, "qualseunome");
  comando[6].parametro1 = 0;
  comando[6].parametro2 = 0;
  comando[6].parametro3 = 0;
  
  
  
  
}





void loop()
{
  if ( verificarComando() == 1 ) 
  {
    processaComando(bufferComando, parametroInt, parametroFloat, parametroChar); 
    zerarVariaveis();    
  }  
}






int verificarComando()
{

 while (Serial3.available() > 0)
 {

   caractere = Serial3.read();
 
  //NOME, OS PRIMEIROS BYTES
  
   if(posicao == 0)
   {
      
     if ( ((nome[contByte] == caractere) || (caractere == '?')) && (caractere != 32))
     {
       contByte++; // Se o caractere vindo da Serial for igual ao caractere do nome o contador (contByte) eh  incrementado em 1.
     }
     
     else  if (caractere == 32) //se encontrar um espaço, ele incremente a posição em 1 e zera o contByte para contar os bytes do próximo campo.
     {
       posicao = 1; //Posição 1 = Campo de Tamanho.
       contByte = 0;
   
     }
     soma = soma + caractere;    
  
   }
   
     
  // BYTES DE TAMANHO
  
   else if (posicao == 1) 
   {
     if(caractere != 32)
     {
      tamanho[contByte] = caractere;
      contByte++;
     }
     else
     {
      posicao = 2; //Posição 1 = Campo de Comando
      contByte = 0;
     }
     soma = soma + caractere;
   }
   
   
   
   
   
   
   //BYTES DE COMANDO 
   
   else if(posicao == 2) //VERIFICA SE O BYTE EH DE COMANDO
   {
   
     if (validouComando == 0) //Verifica se ja validou o comando
     {
      
       if (caractere != 32) //32 = Espaco no ASCII
       {
           bufferComando[contByte] = caractere;
           contByte++;
  
       }
      
       else //Se o caractere for espaco, o nome do comando ja esta no buffer.
       {
         int i;
         
         for(i = 0; i < qntComandos; i++) //Aqui usamos a variavel qntComandos declarada no comeco do programa para checar os comandos existentes.
         {
           if(!strcmp(bufferComando, comando[i].nome))   //Compara a string no buffer com a lista dos comandos definidos, se bater ele aloca dinamicamente variaveis para os parametros.
           {
             //Aloca dinamicamente espaco para colocar os parametros dos comandos
             // e valida o comando.
             parametroInt = (int*)malloc(comando[i].parametro1 * sizeof(int)); 
             qntInt = comando[i].parametro1; //Pega a quantidade parametros inteiro da funcao
             
             parametroFloat = (float*)malloc(comando[i].parametro2 * sizeof(float));
             qntFloat = comando[i].parametro2;//Pega a quantidade de parametros float da funcao.
             
             parametroChar = (char*)malloc(comando[i].parametro3 * sizeof(char));
             qntChar = comando[i].parametro3; //Idem aos de cima.
             
             comandoOk = 1; //O comando eh valido
             numComando = i; //Guarda o numero do comando definido na lista de comandos.
             
    
           }
           
         }
         
         if(comandoOk) //Se o comando for valido continua com a verificacao.
         {
           validouComando = 1; // Comando valido.
           contByte = 0;
           
           if ( ( qntInt == 0) && ( qntFloat == 0) && (qntChar == 0) ) // SE EXISTIR PARAMETROS PARA SEREM LIDOS, O COMANDO NAO TA OK!
           {
             posicao = 3; //POSICAO DE REMETENTE
           }
           
          
         }
         
         else //Se o comando nao for OK zeraremos as variaveis de soma e contagem de byte para iniciar de novo o processo.
         {
           soma = 0;
           contByte = 0; 
           limpabuffer(bufferComando); 
           
         }
       }    
    
    }
    
    else if (validouComando == 1) //Se o comando ja foi validado, pegaremos os parametros
    {

      if (caractere != 32) //32 = CARACTERE "ESPACO" no ASCII
       {
           bufferParametro[contByte] = caractere;
           contByte++;
       }
       
       else //Se o caractere eh 32 (espaco), ja esta armazenado no bufferParametro o valor do parametro. Armazenaremos nos vetores correspondente.
       {
         if (qntInt > 0) //Checa a quantidade de inteiros que a funcao ainda tem para receber.
         {
           parametroInt[comando[numComando].parametro1 - qntInt] = atoi(bufferParametro);
           qntInt--; //Subtrai quando o parametro for adicionado ao vetor.

         }
         
         else if (qntFloat > 0) //Checa a quantidade de float que a funcao ainda tem para receber.
         {
           parametroFloat[comando[numComando].parametro2 - qntFloat] = atof(bufferParametro);
           qntFloat--;
         }
         
         else if (qntChar > 0) //Checa a quantidade de float que a funcao ainda tem para receber.
         {
           parametroChar[comando[numComando].parametro3 - qntChar] = bufferParametro[comando[numComando].parametro3 - qntChar];
           qntChar--;
         }
         
        
        //Se acabou os parametros, prossegue na mensagem.
        
         if( (qntInt == 0) && (qntFloat == 0) && (qntChar == 0))   //Checa se nao falta mais parametros a ser guardados.       
         { 
            validouComando = 0; //Espera o proximo comando no proximo frame.
            comandoOk = 1;
            posicao = 3; //POSICAO DE REMETENTE
  
          }

           contByte = 0;
           //limpabuffer(bufferParametro);    //**NAO SEI A UTILIDADE DISSO AINDA.
           
         
       }
      
      
    }
    soma = soma + caractere;
    
    
    
   }
  
  
  //REMETENTE
  
  
  
  
  else if (posicao == 3)
  {
    if(caractere != 32){
      remetente[contByte] = caractere;
      contByte++;
    }
    
    else{
      posicao = 4; // POSICAO DE CHECKSUM
      contByte = 0;
    }
    soma = soma + caractere;
      
  }  
  
  
  
 
  
   //5 BYTES DE CHECKSUM
   
   else if (posicao == 4)
   { 
     checkSum[contByte] = caractere;     
     contByte++;  
 
          // CHECAGEM FINAL COM O CHECKSUM
   
    if (contByte == 5) 
     {
       icheckSum = atol(checkSum);
         
       if (icheckSum == soma)  //SE O CHECKSUM BATER MANDAMOS O NOME DA FUNCAO E SEUS PARAMETROS
      {  
          return 1;
      }
      else
      {
         Serial3.println("CHECKSUM NAO BATEU!");
         zerarVariaveis();
         return 0;
      }
      
       
              
         
     }
    
      
   }
      

 }    
      
      
}  
  

void processaComando ( char* comandoRecebido, int* parametroInt, float* parametroFloat, char* parametroChar)
{
  char resposta[20];
  
  if (!(strcmp(comandoRecebido,"parafrente")))
   {
     strcpy(resposta, "sucesso");
     enviarMensagem(remetente, comandoRecebido, resposta);
     
      int sto; //estado da onda
      int mt; //motor
    
      // numero de passos 
      for (int passo = 0; passo < parametroInt[0]; passo++)
        for (sto=0; sto<NER-1; sto++) 
        { 
          for (mt=0; mt<MXMT; mt++)
            s[mt].write(zerograu[mt] + estado_rapido[sto][mt]);  
            //goangle(mt, estado_onda_esquerdo[sto][mt]); //go to position 'pos'
          delay(70);  //waits DELAY ms for the servo to reach the position
        }
     
   }
  
   else if (!(strcmp(comandoRecebido,"paratras")))
   {
     strcpy(resposta, "sucesso");
     enviarMensagem(remetente, comandoRecebido, resposta);
     // Anda pra frente   

    int sto; //estado da onda
    int mt; //motor
  
    // numero de passos 
    for (int passo = 0; passo < parametroInt[0]; passo++)
      for (sto=48; sto>=0; sto--) 
      { 
        for (mt=0; mt<MXMT; mt++)
          s[mt].write(zerograu[mt] + estadonda[sto][mt]); 
          //goangle(mt, estadonda[sto][mt]); //go to position 'pos'
        delay(DELAYONDA);  //waits DELAY ms for the servo to reach the position
      }
     
   }
   
   else if (!(strcmp(comandoRecebido,"giradireita")))
   {
     strcpy(resposta, "sucesso");
     enviarMensagem(remetente, comandoRecebido, resposta);
     // Anda pra direita      
     
     int sto; //estado da onda
     int mt; //motor
      // numero de passos 
      for (int passo = 0; passo < parametroInt[0]; passo++)  
        for (sto=0; sto<48; sto++) 
        { 
          for (mt=0; mt<MXMT; mt++)
            s[mt].write(zerograu[mt] + estado_onda_direito[sto][mt]);  
            //goangle(mt, estado_onda_esquerdo[sto][mt]); //go to position 'pos'
          delay(DELAYONDA);  //waits DELAY ms for the servo to reach the position
        }
     
   }

   else if (!(strcmp(comandoRecebido,"giraesquerda")))
   {
     strcpy(resposta, "sucesso");
     enviarMensagem(remetente, comandoRecebido, resposta);
     
      int sto; //estado da onda
      int mt; //motor

      // numero de passos 
      for (int passo = 0; passo < parametroInt[0]; passo++)    
        for (sto=0; sto<48; sto++) 
        { 
          for (mt=0; mt<MXMT; mt++)
            s[mt].write(zerograu[mt] + estado_onda_esquerdo[sto][mt]);  
            //goangle(mt, estado_onda_esquerdo[sto][mt]); //go to position 'pos'
          delay(DELAYONDA);  //waits DELAY ms for the servo to reach the position
        }
     
   } 
  
   else if (!(strcmp(comandoRecebido,"quantoscomandos")))
   {
     char* resposta = "";
     itoa(qntComandos, resposta, 10);
 
     enviarMensagem(remetente, comandoRecebido, resposta);
     
   } 
   
   else if (!(strcmp(comandoRecebido,"exibecomando")))
   {
     char resposta[30];
     char aux[2];
     int qntInt, qntFloat, qntChar;
     

   
     strcpy(resposta, comando[parametroInt[0] - 1].nome);
    
     qntInt = comando[parametroInt[0] -1].parametro1;
     qntFloat = comando[parametroInt[0] -1].parametro2;
     qntChar = comando[parametroInt[0] -1].parametro3;  

     itoa(qntInt, aux, 10);
     strcat(resposta, " ");
     strcat(resposta, aux);

     itoa(qntFloat, aux, 10);
     strcat(resposta, " ");
     strcat(resposta, aux);
             
     itoa(qntChar, aux, 10);
     strcat(resposta, " ");
     strcat(resposta, aux);
     
     enviarMensagem(remetente, comandoRecebido, resposta); 
   } 
   
   else if (!(strcmp(comandoRecebido,"qualseunome")))
   {
     enviarMensagem(remetente, comandoRecebido, NULL);
   } 
}


void enviarMensagem(char* remetente, char* comando, char* resposta)
{
  int soma = 0, i;
  int itamanho = 3 + 5 + 1; // 3 BYTES DO TAMANHO, 5 BYTES DO CHECKSUM, 1 BYTE DE ESPAÇO
  char tamanho[3], nomeAux[20];
  strcpy(nomeAux, nome);
  if (resposta != NULL) strcat(resposta," ");
  strcat(comando," ");
  strcat(remetente," ");
  strcat(nomeAux," ");
  
  itamanho = itamanho + strlen(remetente) + strlen(comando) + strlen(resposta) + strlen(nomeAux);
  itoa(itamanho, tamanho, 10);
  
  
  for (i = 0; i < strlen(remetente); i++)
  {
    soma = soma +  remetente[i];
  }
  
  for (i = 0; i < strlen(comando); i++)
  {
    soma = soma + comando[i];
  }
    
  for (i = 0; i < strlen(resposta); i++)
  {
    soma = soma + resposta[i];
  }
  
  
  for (i = 0; i < strlen(nomeAux); i++)
  {
    soma = soma + nomeAux[i];
  }
  
    for (i = 0; i < strlen(tamanho); i++)
  {
    soma = soma + tamanho[i];
  }
 
  

  
  Serial3.print(remetente);
  if ( strlen(tamanho) < 3)
  {
    Serial3.print("0");
    soma += 48;
  }
  Serial3.print(tamanho);
  Serial3.print(" ");
  soma += 32;
  Serial3.print(comando);
  if (resposta != NULL) Serial3.print(resposta);
  Serial3.print(nomeAux);
  if(soma < 10000) Serial3.print("0");
  Serial3.println(soma);
  
}



//Funcao para limpar os buffers. (Avaliarei depois a eficiencia disso)  
void limpabuffer(char buffer[20])
{
 int i;
  for (i = 0; i < 20; i++)
 {
  buffer[i] = NULL;
 }
}




void  zerarVariaveis()
{
         //Apos executar o comando zera todos os valores e comeca tudo de novo.
       soma = 0;
       contByte = 0; 
       validouComando = 0;
       limpabuffer(bufferComando);
       limpabuffer(bufferParametro);
       limpabuffer(remetente);
       comandoOk = 0;
       criaRemetente = 0;
       perguntaNome = 0;
       posicao = 0;
      
      
}

