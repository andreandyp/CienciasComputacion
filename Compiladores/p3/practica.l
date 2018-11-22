unidades [0-9]
decenas [1-9]{unidades}
centenasA 1{unidades}{unidades}
centenasB 2[0-4]{unidades}
centenasC 25[0-5]
octeto {unidades}|{decenas}|{centenasA}|{centenasB}|{centenasC}

digitos [0-9]
decimal {digitos}*\.{digitos}+
entero {digitos}+
operador \+|\-|\*|\/

beta [0-9a-zA-Z_]*
%%
{octeto}\.{octeto}\.{octeto}\.{octeto} {
	printf("IP válida");
}

({entero}|{decimal}){operador}({entero}|{decimal}) {
	printf("Operación válida");
}

{beta}[A-Z]{beta}[0-9]{beta}|{beta}[0-9]{beta}[A-Z]{beta} {
	int tam = (int) strlen(yytext);
	if(tam < 8 || tam > 16){
		printf("Contraseña inválida");
	}
	else{
		printf("Contraseña válida");
	}
}

. {
	printf("_");
}
%%
