%{
#include <stdlib.h>
#include <string.h>

#include "y.tab.h"
void yyerror(char *);
%}


%%
[a-z]  {
        yylval.ival = yytext[0] - 'a';
        return VAR;
       }


\" { return ASPA; }

[0-9]+(.)*[0-9]* {
		/* converte o valor de string para float */
		yylval.float_value = atof(yytext);

		return NUMERO;
	}

[a-zA-Z0-9\.]+  {
        for (int i = 0; i < yyleng; i++ )
        {
        	yylval.strval[i] = yytext[i];
        }

        yylval.strval[yyleng] = '\0';
        printf ("Achei nome: %s\n", yylval.strval);
        return STRING;
    }

[=] {
        return IGUAL;
    }

\n  {
    	return EOL;
    }
    
\*  {
		return VEZES;
	}

\/  {
		return DIVIDIDO;
	}

\[  {
		return ABRE_COLCHETE;
	}

\]  {
		return FECHA_COLCHETE;
	}

[ \t]+ ;

\*[t]  {
		return VEZES_T;
	}

\/[t]  {
		return DIVIDIDO_T;
	}

\*[p]  {
		return VEZES_P;
	}

\/[p]  {
		return DIVIDIDO_P;
	}

\*[l]  {
		return VEZES_L;
	}

\/[l]  {
		return DIVIDIDO_L;
	}

\*[c]  {
		return VEZES_C;
	}

\/[c]  {
		return DIVIDIDO_C;
	}

.     {printf("**%s**\n", yytext);
yyerror("Token nao reconhecido!\n");}

%%


int  yywrap()
{
  return 1;
}
