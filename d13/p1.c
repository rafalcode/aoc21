/* an exercise in getline: this is the original example from the man pages
 * note that getline() does look quite convenient, because the line's memory need not
 * be explicitly allocated
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GBUF 4
#define CONDREALLOC(x, b, c, a, t, a2, t2); \
    if((x)>=((b)-1)) { \
        (b) += (c); \
        (a)=realloc((a), (b)*sizeof(t)); \
        (a2)=realloc((a2), (b)*sizeof(t2)); \
        for(i=((b)-(c));i<(b);++i) { \
            ((a)[i]) = NULL; \
            ((a2)[i]) = 0; \
        } \
    }

#define CONDREALLOCMN(x, b, c, m, n, t); \
    if((x)>=((b)-1)) { \
        (b) += (c); \
        (m)=realloc((m), (b)*sizeof(t)); \
        (n)=realloc((n), (b)*sizeof(t)); \
    }

typedef struct
{
    char **l;
    size_t *lz;
    int lbf;
    int asz;
} larr_t;

typedef struct /* av_c, a vector fo simple integers*/
{
    int vbf, vsz, *m, *n;
} av_c;

void optis(av_c *avc) // order only on m members
{
    int i, j, t, t2;
    for (i = 1; i < avc->vsz; i++) {
        t = avc->m[i];
        t2 = avc->n[i];
        for (j = i;j>0;j--) {
            if((avc->n[j-1] >t2) || ((avc->n[j-1]==t2) && (avc->m[j-1] >t))) {
                avc->m[j] = avc->m[j-1];
                avc->n[j] = avc->n[j-1];
            } else
                break;
        }
        avc->m[j] = t;
        avc->n[j] = t2;
    }
}

void prt_avc(av_c *avc)
{
    int i;
    printf("Avec printed out:\n"); 
    for(i=0;i<avc->vsz;++i)
        printf("(%i,%i) ", avc->m[i], avc->n[i]);
    printf("\n"); 
    return;
}

void findmx(av_c *avc)
{
    int i, r, c, k=0, rmx=0, cmx=0;
    for(i=0;i<avc->vsz;++i) {
        if(avc->m[i]>cmx)
            cmx=avc->m[i];
        if(avc->n[i]>rmx)
            rmx=avc->n[i];
    }
    cmx++; //dealing with indices
    rmx++;
    printf("grid %i rows x %i cols\n", rmx, cmx); 
    for(r=0;r<rmx;++r) {
        for(c=0;c<cmx;++c) {
 //           printf("%i,%i-%i,%i ", r, c, avc->n[k], avc->m[k]);
            if((c==avc->m[k]) && (r==avc->n[k])) {
                putchar('#');
                k++;
            } else
                putchar('.');
        }
        putchar('\n');
    }

    return;
}

av_c *crea_avc(int vbf)
{
    av_c *avc=malloc(sizeof(av_c));
    avc->vbf=vbf;
    avc->m=malloc(avc->vbf*sizeof(int));
    avc->n=malloc(avc->vbf*sizeof(int));
    avc->vsz=0;
    return avc;
}

void app_avc(av_c *avc, int m, int n)
{
    /* somewhat trivial, but idea is that, as avc is a container, it can be re-alloced inside a function */
    CONDREALLOCMN(avc->vsz, avc->vbf, GBUF, avc->m, avc->n, int);
    avc->m[avc->vsz]=m;
    avc->n[avc->vsz++]=n;
    return;
}

void memexact_avc(av_c *avc)
{
    /* somewhat trivial, but idea is that, as avc is a container, it can be re-alloced inside a function */
    avc->m=realloc(avc->m, avc->vsz*sizeof(int));
    avc->n=realloc(avc->n, avc->vsz*sizeof(int));
    return;
}

void free_avc(av_c *avc)
{
    free(avc->m);
    free(avc->n);
    free(avc);
    return;
}

larr_t *slurplines(char *fn)
{
    FILE *stream;
    // char *line = NULL;
    size_t len = 0;
    int i;

    stream = fopen(fn, "r");
    if (stream == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    larr_t *la=malloc(sizeof(larr_t));
    la->lbf=GBUF;
    la->asz=0;
    la->l=malloc(la->lbf*sizeof(char*)); // does need this, yes, though later it won't be free'd
    la->lz=malloc(la->lbf*sizeof(size_t)); // does need this, yes, though later it won't be free'd
    for(i=0;i<la->lbf;++i) {
        la->lz[i]=0;
        la->l[i]=NULL;
    }

    while ((la->lz[la->asz] = getline(la->l+la->asz, &len, stream)) != -1) {
        // nread is number of returned characters
        CONDREALLOC(la->asz, la->lbf, GBUF, la->l, char*, la->lz, size_t);
        // printf("Retrieved line of length (returned val method): %zu\n", nread);
        // printf("Retrieved line of length (strlen method): %zu\n", strlen(line));
        // fwrite(line, nread, 1, stdout);
        if(la->l[la->asz][la->lz[la->asz]-1]=='\n') // check the newline
            la->l[la->asz][la->lz[la->asz]-1]='\0'; // smash the newline, of course you could save a byte and reallocate, etc. etc.
        la->asz++;
    }

    /* now to see if normalise works */
    for(i=la->asz;i<la->lbf;++i)
        free(la->l[i]);
    la->l=realloc(la->l, la->asz*sizeof(char*)); // normalize
    la->lz=realloc(la->lz, la->asz*sizeof(size_t)); // normalize

    /* Ok that's it */
    fclose(stream);
    return la;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int i, j, k;

    larr_t *la=slurplines(argv[1]);
    // int **ma=malloc(la->asz*sizeof(int*));
    char *tk=NULL, *tk2=NULL;
    char t[]=",";
    av_c *avc=crea_avc(GBUF);
    for(i=0;i<la->asz;++i) {
        tk=strtok(la->l[i], t);
        if(tk==NULL) {
            break;
        }
        tk2=strtok(NULL, t);
        app_avc(avc, atoi(tk), atoi(tk2));
    }
    optis(avc);
    // prt_avc(avc);
    findmx(avc);
    memexact_avc(avc);
    free_avc(avc);
    for(i=0;i<la->asz;++i)
        free(la->l[i]);
    free(la->l);
    free(la->lz);
    free(la);
    return 0;
}
