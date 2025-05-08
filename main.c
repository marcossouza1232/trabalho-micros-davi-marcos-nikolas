#include <avr/io.h>


// funcoes teclado
char getchar_alfanumerico(void);
void delay_ms(int);
char getchar_debounce(void);
char get_aperto(void);
void get_sequencia(char *, char);

// funcoes computador.


int main(void)
{
	// comecamos lendo o codigo do aluno.
	char codigo_aluno[6];
	get_sequencia(codigo_aluno, 6);

	// vamos configurar o usart para transmitir o codigo para o computador.
	
	return 0;
}

char getchar_alfanumerico()
{
	char caractere;
	// Devido ao arduino utilizado só ter os 8 pins da mesma porta na porta D, usaremos a porta D
	// Temos que usar PUD = 0, ou seja ativar o Pull-up, e também ativar o modo leitura.
	// Faremos assim L1 = PD0, L2 = PD1, L3 = PD2, L4 = PD3, C1 = PD4, C2 = PD5, C3 = PD6.
	// DDRD tem que estar no modo leitura para essas teclas, ou seja 0.
	// Leremos os bits pelo byte PIND.
	// Mas não faz sentido eu configurar o pino sem o PUD, pois é só uma vez.
	// PUD também é por default zero, não preciso alterar.

	char linha;
	char coluna;
	// vamos ler primeiro a linha, para isso eu farei o seguinte.
	// Vamos colocar a coluna como output, e as linhas para input.
	DDRD = 0x70; // DDRD = 0b0111.0000
	PORTD = 0x0f; // PORTD = 0b0000.1111, aplicando 0 nos outputs temos 0 nos outputs, e aplicando 1 nos inputs, temos o pull-up ativado.
	linha = (~PIND) & (0x0f)); // pegamos apenas os pinos a serem analisados, e invertmos pois lemos a negação.
	switch ( linha ) 
	{
		case 0:
			linha = 0; // nada foi apertado.
			break;
		case 1: 
			linha = 1;
			break;
		case 2:
			linha = 2;
			break;
		case 4:
			linha = 3;
			break;
		case 8:
			linhas = 4;
			break;
		default:
			linha = 0;

	}

	// vamos ler a coluna.
	DDRD = 0x0f; // DDRD = 0b0000.1111, input nas colunas, output nas linhas.
	PORTD = 0x70; // PORD = 0b0111.0000, pull-up nas colunas, 0 nas linhas.
	coluna = (~PIND) & (0x70);
	switch ( coluna )
	{
		case 0:
			coluna = 0; // nada foi apertado.
			break;
		case 1:
			coluna = 1;
			break;
		case 2:
			coluna = 2;
			break;
		case 4:
			coluna = 3;
			break;
		default:
			coluna = 0;
	}
	if ( linha == 0 || coluna == 0 )
	{
		caractere = 0;
	}
	else if ( coluna == 2 && linha == 4 )
	{
		caractere = '0';
	}
	else
	{
		caractere = '1' + (linha - 1) + 3*(coluna - 1); // formula para conseguir o numero.
	}
	return caractere;
}

void delay_ms(int ms)
{
	// supondo que o relogio seja de 16MHz, temos o seguinte:
	// np/(16e6) = 1/1e3
	// np = 16e6/1e3 = 16e3
	// n deve ser no máximo 255
	// logo p deve ser no mínimo 63, logo usaremos prescaler de 64.
	// n = 250.
	for ( char i = 0; i < 250*64; i++ ); // provisorio.
	return;
}

char getchar_debounce()
{
	char tecla_anterior = 0;
	char tecla_atual = 0;
	tecla_anterior = getchar_alfanumerico();
	char contagem = 0;
	while ( contagem != 7 )
	{
		tecla_atual = getchar_alfanumerico();
		if ( tecla_anterior == tecla_atual )
		{
			contagem++;
		}
		else
		{
			contagem = 0;
		}
		tecla_anterior = tecla_atual;
		delay_ms(1);
	}
	return tecla_anterior;
}

char get_aperto()
{
	// a funcao de aperto existe, pois nao interessa para nos que o usuario fique com um botao apertado, e nao solte mais e a gente fique lendo como se ele estivesse clicando na mesma tecla varias vezes, portanto é necessario que ele aperte é solte.
	char caractere;
	while ( ( caractere = getchar_debounce()) == 0 ); // pega o caractere ate ele ser valido.
	while ( getchar_debounce() )
	{
		// espero o getchar retorna 0, que quer dizer que nenhuma tecla está sendo apertada
	}
	return caractere;
}

void get_sequencia(char *string, char comprimento)
{
	for ( char contagem = 0; contagem < comprimento; contagem++ )
	{
		*(string++) = get_aperto();
	}
	return;
}
