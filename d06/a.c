/* how to build up an integer vector of arbitrary size 
Note how we use a container for the vector. Sure, it involves some extra functions */
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

int main(int argc, char *argv[])
{
    av_c *avc=crea_avc(GBUF);

    int i;
    int *s= {3,4,3,1,2};
    printf("s=%zu\n", sizeof(s)); 
    // for(i=0;i<24;++i) {
    //     condrea_avc(avc);
    //     avc->v[avc->vsz++]=i;
    // }
    // norm_avc(avc);

    // printf("Avec printed out:\n"); 
    // for(i=0;i<24;++i)
    //     printf("%i ", avc->v[i]); 
    // printf("\n"); 

    // free_avc(avc);

    return 0;
}
