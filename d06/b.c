/* how to build up an integer vector of arbitrary size 
these programs are often copied pasted from, but this one is unusual,
and actually leads to errors.
Caveat Emptor! */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GBUF 5
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

av_c *crea_avc(int vbf)
{
    av_c *avc=malloc(sizeof(av_c));
    avc->vbf=vbf;
    avc->v=malloc(avc->vbf*sizeof(int));
    avc->vsz=0;
    return avc;
}

void condrea_avc(av_c *avc)
{
    /* somewhat trivial, but idea is that, as avc is a container, it can be re-alloced inside a function */
    CONDREALLOC(avc->vsz, avc->vbf, GBUF, avc->v, int);
    return;
}

void condrea2_avc(av_c *avc)
{
    int i,j;
   int csz=avc->vsz;
   for(i=0;i<3;++i) 
       for(j=0;j<csz;++j) 
           avc->v[j]--;
    /* somewhat trivial, but idea is that, as avc is a container, it can be re-alloced inside a function */
    // CONDREALLOC(avc->vsz, avc->vbf, GBUF, avc->v, int);
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

void prtavecn(char *c, av_c *avc)
{
    int i, j;
    // before the first token
    for(i=0;i<avc->v[0];++i)
        putchar(c[i]);
    putchar('\n');
    for(i=1;i<avc->vsz;++i) {
        for(j=avc->v[i-1]+1;j<avc->v[i];++j) 
            putchar(c[j]);
        putchar('\n');
    }
    char *tc=c + avc->v[avc->vsz-1]+1;
    while(*tc) {
        putchar(*tc);
        tc++;
    }
    putchar('\n');
    return;
}

void prtavecnd(char *ca, av_c *avc) /* a debug version */
{
    int i, j;
    char *c=ca;
    printf("chars %i to %i: ", 0, avc->v[0]-1);
    for(i=0;i<avc->v[0];++i)
        putchar(c[i]);
    putchar('\n');

    for(i=1;i<avc->vsz;++i) {
        printf("chars %i to %i: ", avc->v[i-1]+1, avc->v[i]-1);
        for(j=avc->v[i-1]+1;j<avc->v[i];++j) 
            putchar(c[j]);
        putchar('\n');
    }
    return;
}

av_c *avget(char *c)
{
    av_c *avc=crea_avc(GBUF);
    int cou=0;
    while(c[cou]) {
        if(c[cou]==',') {
            condrea_avc(avc);
            avc->v[avc->vsz++]=cou;
        }
        cou++;
    }
    // somewhat stealthily, we also add the position of the final \0.
    avc->v[avc->vsz++]=cou;
    norm_avc(avc);

    return avc;
}

int main(int argc, char *argv[])
{
    if(argc<2) {
        printf("One arg: a string with underscores.\n");
        exit(EXIT_FAILURE);
    }
    char *c=argv[1];

    av_c *avc=avget(c);
    prtavec(avc);
    condrea2_avc(avc);
    prtavec(avc);

    free_avc(avc);

    return 0;
}
