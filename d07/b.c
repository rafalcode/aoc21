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
    size_t vbf, vsz;
    int *v;
} av_c;

void norm_avc(av_c *avc) // Warning: only use this at very end!
{
    /* somewhat trivial, but idea is that, as avc is a container, it can be re-alloced inside a function */
    avc->v=realloc(avc->v, avc->vsz*sizeof(size_t));
    return;
}

void free_avc(av_c *avc)
{
    free(avc->v);
    free(avc);
    return;
}

void dayvec(av_c *avc)
{
    size_t i;
    size_t curtot=avc->vsz;
    for(i=0;i<curtot;++i) {
        if(avc->v[i] == 0) {
            CONDREALLOC(avc->vsz, avc->vbf, GBUF, avc->v, size_t);
            avc->v[i]=6;
            avc->v[avc->vsz++]=8;
        } else
            avc->v[i]--; 
    }
    return;
}

void prtavec(av_c *avc)
{
    size_t i;
    // printf("vsz=%i/vbf=%i\n", avc->vsz, avc->vbf); 
    for(i=0;i<avc->vsz;++i)
        printf("%i ", avc->v[i]); 
    printf("\n"); 
    return;
}

void prtavec2(av_c *avc, size_t j)
{
    size_t i;
    // printf("vsz=%i/vbf=%i\n", avc->vsz, avc->vbf); 
    printf("After day %zu (tot %zu): ", j, avc->vsz);
    for(i=0;i<avc->vsz;++i)
        printf("%i ", (int)avc->v[i]); 
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

    size_t i, j;
	char t[]=",";
    av_c *avc=crea_avc(GBUF);
	char *tk=strtok(argv[1], t);
	if(tk ==NULL) {
		printf("First strtok returned NULL\n"); 
	    exit(EXIT_FAILURE);
    }
    avc->v[avc->vsz++]=atol(tk);
	while( (tk=strtok(NULL, t)) != NULL) {
        CONDREALLOC(avc->vsz, avc->vbf, GBUF, avc->v, size_t);
        avc->v[avc->vsz++]=atol(tk);
	}
    int mx=0;
    for(i=0;i<avc->vsz;++i)
        if(avc->v[i]>mx)
            mx=avc->v[i];
    int m0, tm /* total moves */, tmx=0x7FFFFFFF, imx;
    int m;
    printf("tmx=%i\n", tmx);
    for(i=0;i<mx;++i) {
        tm=0;
        for(j=0;j<avc->vsz;++j) {
            m0=i-avc->v[j];
            if(m0<0)
                m0=-m0;
            m=(m0*m0+m0)/2; // avoid premature division by 2, would break integer.
            tm+=m;
        }
        if(tm<tmx) {
            tmx=tm;
            imx=i;
        }
    }
    printf("imx=%i/tmx=%i\n", imx, tmx);

    free_avc(avc);
    return 0;
}
