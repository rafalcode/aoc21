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

typedef struct
{
    char **l;
    size_t *lz;
    int lbf;
    int asz;
} larr_t;

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

void runm00(int **ma, int nr, int nc)
{
    int i, j;

    for(i=0;i<nr;++i)
        for(j=0;j<nc; j++) {
            if(i==0) {
                if(j==0) {
                    if(ma[i][j+1]>9) ma[i][j]++;
                    if(ma[i+1][j+1]>9) ma[i][j]++;
                    if(ma[i+1][j]>9) ma[i][j]++;
                } else if(j==nc-1) {
                    if(ma[i][j-1]>9) ma[i][j]++;
                    if(ma[i+1][j-1]>9) ma[i][j]++;
                    if(ma[i+1][j]>9) ma[i][j]++;
                } else {
                    if(ma[i][j+1]>9) ma[i][j]++;
                    if(ma[i+1][j+1]>9) ma[i][j]++;
                    if(ma[i+1][j]>9) ma[i][j]++;
                    if(ma[i][j-1]>9) ma[i][j]++;
                    if(ma[i+1][j-1]>9) ma[i][j]++;
                }
            } else if(i==nr-1) {
                if(j==0) {
                    if(ma[i][j+1]>9) ma[i][j]++;
                    if(ma[i-1][j+1]>9) ma[i][j]++;
                    if(ma[i-1][j]>9) ma[i][j]++;
                } else if(j==nc-1) {
                    if(ma[i][j-1]>9) ma[i][j]++;
                    if(ma[i-1][j-1]>9) ma[i][j]++;
                    if(ma[i-1][j]>9) ma[i][j]++;
                } else {
                    if(ma[i][j+1]>9) ma[i][j]++;
                    if(ma[i-1][j+1]>9) ma[i][j]++;
                    if(ma[i-1][j]>9) ma[i][j]++;
                    if(ma[i][j-1]>9) ma[i][j]++;
                    if(ma[i-1][j-1]>9) ma[i][j]++;
                }
            } else {
                if(j==0) {
                    if(ma[i][j+1]>9) ma[i][j]++;
                    if(ma[i-1][j+1]>9) ma[i][j]++;
                    if(ma[i+1][j+1]>9) ma[i][j]++;
                    if(ma[i-1][j]>9) ma[i][j]++;
                    if(ma[i+1][j]>9) ma[i][j]++;
                } else if(j==nc-1) {
                    if(ma[i][j-1]>9) ma[i][j]++;
                    if(ma[i-1][j-1]>9) ma[i][j]++;
                    if(ma[i+1][j-1]>9) ma[i][j]++;
                    if(ma[i-1][j]>9) ma[i][j]++;
                    if(ma[i+1][j]>9) ma[i][j]++;
                } else {
                    if(ma[i][j+1]>9) ma[i][j]++;
                    if(ma[i][j-1]>9) ma[i][j]++;
                    if(ma[i-1][j-1]>9) ma[i][j]++;
                    if(ma[i-1][j]>9) ma[i][j]++;
                    if(ma[i-1][j+1]>9) ma[i][j]++;
                    if(ma[i+1][j-1]>9) ma[i][j]++;
                    if(ma[i+1][j]>9) ma[i][j]++;
                    if(ma[i+1][j+1]>9) ma[i][j]++;
                }
            }
        }
    return;
}

void runm0(int **ma, int nr, int nc)
{
    int i, j;

    for(i=0;i<nr/2;i++) {
        for(j=0;j<nc/2;++j) {
            if(ma[i][j]>9) {
                if(i==0) {
                    if(j==0) {
                        ma[i][j+1]++; ma[i+1][j+1]++; ma[i+1][j]++;
                    } else if(j==nc-1) {
                        ma[i][j-1]++; ma[i+1][j-1]++; ma[i+1][j]++;
                    } else {
                        ma[i+1][j-1]++; ma[i][j+1]++; ma[i+1][j+1]++; ma[i+1][j]++; ma[i][j-1]++;
                    }
                } else if(i==nr-1) {
                    if(j==0) {
                        ma[i][j+1]++; ma[i-1][j+1]++; ma[i-1][j]++;
                    } else if(j==nc-1) {
                        ma[i][j-1]++; ma[i-1][j-1]++; ma[i-1][j]++;
                    } else {
                        ma[i][j+1]++; ma[i-1][j+1]++; ma[i-1][j]++; ma[i][j-1]++; ma[i-1][j-1]++;
                    }
                } else {
                    if(j==0) {
                        ma[i][j+1]++; ma[i-1][j+1]++; ma[i+1][j+1]++; ma[i-1][j]++; ma[i+1][j]++;
                    } else if(j==nc-1) {
                        ma[i][j-1]++; ma[i-1][j-1]++; ma[i+1][j-1]++; ma[i-1][j]++; ma[i+1][j]++;
                    } else {
                        ma[i][j+1]++; ma[i][j-1]++; ma[i-1][j-1]++; ma[i-1][j]++; ma[i-1][j+1]++; ma[i+1][j-1]++; ma[i+1][j]++; ma[i+1][j+1]++;
                    }
                }
            } 
        }
    }
    for(i=nr-1;i>=nr/2;i--) {
        for(j=nc-1;j>=nc/2;j--) {
            if(ma[i][j]>9) {
                if(i==0) {
                    if(j==0) {
                        ma[i][j+1]++; ma[i+1][j+1]++; ma[i+1][j]++;
                    } else if(j==nc-1) {
                        ma[i][j-1]++; ma[i+1][j-1]++; ma[i+1][j]++;
                    } else {
                        ma[i+1][j-1]++; ma[i][j+1]++; ma[i+1][j+1]++; ma[i+1][j]++; ma[i][j-1]++;
                    }
                } else if(i==nr-1) {
                    if(j==0) {
                        ma[i][j+1]++; ma[i-1][j+1]++; ma[i-1][j]++;
                    } else if(j==nc-1) {
                        ma[i][j-1]++; ma[i-1][j-1]++; ma[i-1][j]++;
                    } else {
                        ma[i][j+1]++; ma[i-1][j+1]++; ma[i-1][j]++; ma[i][j-1]++; ma[i-1][j-1]++;
                    }
                } else {
                    if(j==0) {
                        ma[i][j+1]++; ma[i-1][j+1]++; ma[i+1][j+1]++; ma[i-1][j]++; ma[i+1][j]++;
                    } else if(j==nc-1) {
                        ma[i][j-1]++; ma[i-1][j-1]++; ma[i+1][j-1]++; ma[i-1][j]++; ma[i+1][j]++;
                    } else {
                        ma[i][j+1]++; ma[i][j-1]++; ma[i-1][j-1]++; ma[i-1][j]++; ma[i-1][j+1]++; ma[i+1][j-1]++; ma[i+1][j]++; ma[i+1][j+1]++;
                    }
                }
            } 
        }
    }
    return;
}

void runma(int **ma, int nr, int nc)
{
    int i, j;

    for(i=0;i<nr;i++) {
        for(j=0;j<nc;++j) {
            if(ma[i][j]>9) {
                if(i==0) {
                    if(j==0) {
                        ma[i][j+1]++; ma[i+1][j+1]++; ma[i+1][j]++;
                    } else if(j==nc-1) {
                        ma[i][j-1]++; ma[i+1][j-1]++; ma[i+1][j]++;
                    } else {
                        ma[i+1][j-1]++; ma[i][j+1]++; ma[i+1][j+1]++; ma[i+1][j]++; ma[i][j-1]++;
                    }
                } else if(i==nr-1) {
                    if(j==0) {
                        ma[i][j+1]++; ma[i-1][j+1]++; ma[i-1][j]++;
                    } else if(j==nc-1) {
                        ma[i][j-1]++; ma[i-1][j-1]++; ma[i-1][j]++;
                    } else {
                        ma[i][j+1]++; ma[i-1][j+1]++; ma[i-1][j]++; ma[i][j-1]++; ma[i-1][j-1]++;
                    }
                } else {
                    if(j==0) {
                        ma[i][j+1]++; ma[i-1][j+1]++; ma[i+1][j+1]++; ma[i-1][j]++; ma[i+1][j]++;
                    } else if(j==nc-1) {
                        ma[i][j-1]++; ma[i-1][j-1]++; ma[i+1][j-1]++; ma[i-1][j]++; ma[i+1][j]++;
                    } else {
                        ma[i][j+1]++; ma[i][j-1]++; ma[i-1][j-1]++; ma[i-1][j]++; ma[i-1][j+1]++; ma[i+1][j-1]++; ma[i+1][j]++; ma[i+1][j+1]++;
                    }
                }
            } 
        }
    }
    return;
}

void run2ma(int **ma, int nr, int nc, int **m2, int nr2, int nc2)
{
    int i, j;

    for(i=0;i<nr;i++) {
        for(j=0;j<nc;++j) {
            if(ma[i][j]==m2[i][j])
                printf("O "); 
            else
                printf("X "); 
        }
        printf("\n"); 
    }
    return;
}

void runmar(int **ma, int nr, int nc)
{
    int i, j;

    for(i=(nr-1);i>=0;i--) {
        for(j=(nc-1);j>=0;--j) {
            if(ma[i][j]>9) {
                if(i==0) {
                    if(j==0) {
                        ma[i][j+1]++; ma[i+1][j+1]++; ma[i+1][j]++;
                    } else if(j==nc-1) {
                        ma[i][j-1]++; ma[i+1][j-1]++; ma[i+1][j]++;
                    } else {
                        ma[i+1][j-1]++; ma[i][j+1]++; ma[i+1][j+1]++; ma[i+1][j]++; ma[i][j-1]++;
                    }
                } else if(i==nr-1) {
                    if(j==0) {
                        ma[i][j+1]++; ma[i-1][j+1]++; ma[i-1][j]++;
                    } else if(j==nc-1) {
                        ma[i][j-1]++; ma[i-1][j-1]++; ma[i-1][j]++;
                    } else {
                        ma[i][j+1]++; ma[i-1][j+1]++; ma[i-1][j]++; ma[i][j-1]++; ma[i-1][j-1]++;
                    }
                } else {
                    if(j==0) {
                        ma[i][j+1]++; ma[i-1][j+1]++; ma[i+1][j+1]++; ma[i-1][j]++; ma[i+1][j]++;
                    } else if(j==nc-1) {
                        ma[i][j-1]++; ma[i-1][j-1]++; ma[i+1][j-1]++; ma[i-1][j]++; ma[i+1][j]++;
                    } else {
                        ma[i][j+1]++; ma[i][j-1]++; ma[i-1][j-1]++; ma[i-1][j]++; ma[i-1][j+1]++; ma[i+1][j-1]++; ma[i+1][j]++; ma[i+1][j+1]++;
                    }
                }
            } 
        }
    }
    return;
}

void runma2(int **ma, int nr, int nc)
{
    int i, j;

    for(i=0;i<nr;i++) {
        for(j=(nc-1);j>=0;--j) {
            if(ma[i][j]>9) {
                if(i==0) {
                    if(j==0) {
                        ma[i][j+1]++; ma[i+1][j+1]++; ma[i+1][j]++;
                    } else if(j==nc-1) {
                        ma[i][j-1]++; ma[i+1][j-1]++; ma[i+1][j]++;
                    } else {
                        ma[i+1][j-1]++; ma[i][j+1]++; ma[i+1][j+1]++; ma[i+1][j]++; ma[i][j-1]++;
                    }
                } else if(i==nr-1) {
                    if(j==0) {
                        ma[i][j+1]++; ma[i-1][j+1]++; ma[i-1][j]++;
                    } else if(j==nc-1) {
                        ma[i][j-1]++; ma[i-1][j-1]++; ma[i-1][j]++;
                    } else {
                        ma[i][j+1]++; ma[i-1][j+1]++; ma[i-1][j]++; ma[i][j-1]++; ma[i-1][j-1]++;
                    }
                } else {
                    if(j==0) {
                        ma[i][j+1]++; ma[i-1][j+1]++; ma[i+1][j+1]++; ma[i-1][j]++; ma[i+1][j]++;
                    } else if(j==nc-1) {
                        ma[i][j-1]++; ma[i-1][j-1]++; ma[i+1][j-1]++; ma[i-1][j]++; ma[i+1][j]++;
                    } else {
                        ma[i][j+1]++; ma[i][j-1]++; ma[i-1][j-1]++; ma[i-1][j]++; ma[i-1][j+1]++; ma[i+1][j-1]++; ma[i+1][j]++; ma[i+1][j+1]++;
                    }
                }
            } 
        }
    }
    return;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int i, j, k;

    larr_t *la=slurplines(argv[1]);
    larr_t *l2=slurplines(argv[2]);
    // int **ma=malloc(la->asz*sizeof(int*));
    int nr=la->asz;
    int nc=la->lz[0]-1; // excluding the blank char.
    int nr2=l2->asz;
    int nc2=l2->lz[0]-1;
    int **ma=malloc(nr*sizeof(int*));
    int **m2=malloc(nr2*sizeof(int*));
    for(i=0;i<nr2;++i)
        m2[i]=malloc(nc2*sizeof(int));
    for(i=0;i<nr2;++i)
        for(j=0;j<nc2; j++)
            m2[i][j]=l2->l[i][j]-48;
    for(i=0;i<nr;++i)
        ma[i]=malloc(nc*sizeof(int));
    for(i=0;i<nr;++i)
        for(j=0;j<nc; j++)
            ma[i][j]=la->l[i][j]-48;
    printf("Initial state:\n"); 
    for(i=0;i<nr;++i) {
        for(j=0;j<nc; j++)
            printf("%i ", ma[i][j]);
        printf("\n"); 
    }
    for(k=0;k<2;++k) {
        for(i=0;i<nr;++i)
            for(j=0;j<nc; j++)
                ma[i][j]++;
        runm00(ma, nr, nc);
        for(i=0;i<nr;++i)
            for(j=0;j<nc; j++)
                if(ma[i][j]>9)
                    ma[i][j]=0;

        printf("\n"); 
        for(i=0;i<nr;++i) {
            for(j=0;j<nc; j++)
                printf("%i ", ma[i][j]);
            printf("\n"); 
        }
        if(k==1) {
            printf("\n"); 
            run2ma(ma, nr, nc, m2, nr2, nc2);
        }
    }

    /* and now to free */
    for(i=0;i<la->asz;++i)
        free(la->l[i]);
    free(la->l);
    free(la->lz);
    free(la);
    for(i=0;i<l2->asz;++i)
        free(l2->l[i]);
    free(l2->l);
    free(l2->lz);
    free(l2);
    for(i=0;i<nr2;++i)
        free(m2[i]);
    for(i=0;i<nr;++i)
        free(ma[i]);
    free(ma);
    free(m2);

    exit(EXIT_SUCCESS);
}
