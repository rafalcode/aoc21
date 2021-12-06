#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GBUF 4
#define CONDREALLOC(x, b, c, a, t); \
    if((x)>=((b)-1)) { \
        (b) += (c); \
        (a)=realloc((a), (b)*sizeof(t)); \
    }

typedef struct /* av_c */
{
    int vbf, vsz, *v;
} av_c;

void norm_avc(av_c *avc)
{
    /* somewhat trivial, but idea is that, as avc is a container, it can be re-alloced inside a function */
    avc->v=realloc(avc->v, avc->vsz*sizeof(int));
    return;
}

void free_avc(av_c *avc)
{
    free(avc->v);
    free(avc);
    return;
}

void prtavec(av_c *avc)
{
    int i;
    printf("Vector of size %i\n", avc->vsz);
    for(i=0;i<avc->vsz;++i)
        printf("%i ", avc->v[i]); 
    printf("\n"); 
    return;
}

av_c *crea_avc(int vbf)
{
    av_c *avc=malloc(sizeof(av_c));
    avc->vbf=vbf;
    avc->v=malloc(avc->vbf*sizeof(int));
    avc->vsz=0;
    return avc;
}

void prtusage(void)
{
	printf("Comma separated integers all in one string please\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	if(argc != 2)
		prtusage();

	char t[]=",";
    av_c *avc=crea_avc(GBUF);
	char *tk=strtok(argv[1], t);
	if(tk ==NULL) {
		printf("First strtok returned NULL\n"); 
	    exit(EXIT_FAILURE);
    }
    avc->v[avc->vsz++]=atoi(tk);
    int cou=avc->vsz;
	while( (tk=strtok(NULL, t)) != NULL) {
        cou++;
        CONDREALLOC(cou, avc->vbf, GBUF, avc->v, int);
        avc->v[avc->vsz++]=atoi(tk);
	}
    norm_avc(avc);
    prtavec(avc);
    free_avc(avc);

   return 0;
}
