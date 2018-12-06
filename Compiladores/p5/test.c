#include <stdio.h>
#include <stdlib.h>
int KMPSearch(char* txt, char* pat);
void computeLPSArray(char* pat, int M, int lps[]);
int cadlen(char* cad);

int main(int argc, char const *argv[]){
	char* cad = "hola";
	char* resta = "ol";
	int match = KMPSearch(cad, resta);

	printf("match %d\n", match);

	char* res;
	if(match != -1){
		int len = cadlen(cad) - cadlen(resta);
		int i = 0, j = 0;
		res = malloc(len);
		while(i < len){
			if(i != match){
				res[i] = cad[j];
			}
			else{
				j += cadlen(resta);
				res[i] = cad[j];
			}
			++i;
			++j;
		}

		printf("Res: %s\n", res);
	}else{
		printf("No se puede eliminar\n");
	}
	return 0;
}

int KMPSearch(char* txt, char* pat) { 
    int M = cadlen(pat);
    int N = cadlen(txt);

    int lps[M];
    int j = 0;

    calcPrefSuf(pat, M, lps); 

    int i = 0;
    while (i < N) { 
        if (pat[j] == txt[i]) { 
            j++; 
            i++; 
        } 
        if (j == M) { 
        	return (i - j);
        }
        else if (i < N && pat[j] != txt[i]) { 
            if (j != 0) 
                j = lps[j - 1]; 
            else
                i = i + 1; 
        }
    }
    return -1;
}
void calcPrefSuf(char* pat, int M, int lps[]) { 
    int len = 0; 
    int i = 1; 
    lps[0] = 0;

    while (i < M) { 
        if (pat[i] == pat[len]) { 
            len++; 
            lps[i] = len; 
            i++; 
        } 
        else{ 
            if (len != 0) { 
                len = lps[len - 1]; 
            } 
            else{ 
                lps[i] = len; 
                i++; 
            } 
        } 
    } 
}

int cadlen(char* cad){
    int i = 0;
    int len = 0;
    while(cad[i]){
        len++;
        i++;
    }

    return len;
}