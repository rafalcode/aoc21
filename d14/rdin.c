/* csvrd with hash */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "csvrdh.h"

#define GBUF 5
#define CONDREALLOCW(x, b, c, a, t); \
    if((x)>=((b)-1)) { \
        (b) += (c); \
        (a)=realloc((a), (b)*sizeof(t)); \
    }

typedef struct /* av_c, a vector fo simple integers*/
{
    size_t vbf, vsz;
    char *v;
} av_c;

void prt_avc(av_c *avc)
{
    size_t i;
    printf("av_c of size %zu\n", avc->vsz);
    for(i=0;i<avc->vsz;++i)
        putchar(avc->v[i]);
    printf("\n"); 
    return;
}

av_c *crea_avc(size_t vbf)
{
    av_c *avc=malloc(sizeof(av_c));
    avc->vbf=vbf;
    avc->v=malloc(avc->vbf*sizeof(char));
    avc->vsz=0;
    return avc;
}

void app_avc(av_c *avc, char mychar)
{
    /* somewhat trivial, but idea is that, as avc is a container, it can be re-alloced inside a function */
    CONDREALLOCW(avc->vsz, avc->vbf, GBUF, avc->v, char);
    avc->v[avc->vsz++]=mychar;
    return;
}

void memexact_avc(av_c *avc)
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

unsigned givehtsz(unsigned mnf)
{
    unsigned htsz=2*mnf/3;
    // try to grab a prime ... well just avoid 5-multiples, 3-multiples, and evens
    if(!(htsz%5)) 
        htsz++; // incrment by 1 if multiple of 5
    if(!(htsz%3)) 
        htsz++;
    if(!(htsz%2)) 
        htsz++;
    return htsz;
}

unsigned hashit(char *str, unsigned tsz) /* Dan Bernstein's one */
{
    unsigned long hash = 5381;
    int c;

    char *tstr=str;
    while ((c = *tstr++))
        hash = ((hash << 5) + hash) + c; /*  hash * 33 + c */

    return hash % tsz;
}

snodm **hashnam(aaw_c *aawc, unsigned tsz)
{
    unsigned i;

    snodm **stab=malloc(tsz*sizeof(snodm *));
    for(i=0;i<tsz;++i) 
        stab[i]=NULL;
    snodm *tsnod0, *tsnod2;

    unsigned tint;

    /* OK, we're going to loop through the map file container: i here follows the global SNP name index */
    int firstline=2;
    for(i=firstline; i< aawc->numl; ++i) {

        tint=hashit(aawc->aaw[i]->aw[0]->w, tsz); // hash the lab_id
        if( (stab[tint] == NULL) ) { // nothing in that slot right now.
            stab[tint]=malloc(sizeof(snodm));
            stab[tint]->aw=aawc->aaw[i];
            stab[tint]->idx=i;
            stab[tint]->n=NULL;
            continue;
        }
        tsnod2=stab[tint];
        while( (tsnod2 != NULL) ) {
            if(!strcmp(tsnod2->aw->aw[0]->w, aawc->aaw[i]->aw[0]->w)) {
                printf("Duppair:alert  %s vs %s\n", tsnod2->aw->aw[0]->w, aawc->aaw[i]->aw[0]->w);
                break;
            }
            tsnod0=tsnod2;
            tsnod2=tsnod2->n;
        }
        tsnod0->n=malloc(sizeof(snodm));
        tsnod0->n->aw = aawc->aaw[i];
        tsnod0->n->idx=i;
        tsnod0->n->n=NULL;
    }
    return stab;
}

int mu_nam(aaw_c *aawc, snodm **stam, unsigned tsz, char *w)
{
    snodm *tsnod2;
    unsigned tint;

    tint=hashit(w, tsz); // hash the snpname
    if( stam[tint] == NULL ) {
        printf("nomatch %s\n", w);
        return 1;
    }

    tsnod2=stam[tint];
    while( (tsnod2 != NULL) ) {
        if(!strcmp(tsnod2->aw->aw[0]->w, w)) {
//            printf("%s matches at %s\n", w, tsnod2->aw->aw[0]->w);
            return 0;
        }
        tsnod2=tsnod2->n;
    }
    printf("nomatch %s\n", w);
    return 1;
}

char mu2(aaw_c *aawc, snodm **stam, unsigned tsz, char *w)
{
    snodm *tsnod2;
    unsigned tint;

    tint=hashit(w, tsz); // hash the snpname
    if( stam[tint] == NULL ) {
        printf("nomatch %s\n", w);
        return '\0';
    }

    tsnod2=stam[tint];
    while( (tsnod2 != NULL) ) {
        if(!strcmp(tsnod2->aw->aw[0]->w, w)) {
            return tsnod2->aw->aw[2]->w[0];
        }
        tsnod2=tsnod2->n;
    }
    printf("nomatch %s\n", w);
    return '\0';
}

void freechainharr(snodm **stam, size_t tsz)
{
    int i;
    snodm *tsnod0, *tsnod2;
    for(i=0; i<tsz; ++i) {
        if( (stam[i] != NULL) ) {
            while( (stam[i]->n != NULL) ) {
                tsnod0=stam[i];
                tsnod2=stam[i]->n;
                while((tsnod2->n != NULL) ){
                    tsnod0=tsnod2;
                    tsnod2=tsnod2->n;
                }
                free(tsnod0->n);
                tsnod0->n=NULL;
            }
            free(stam[i]);
        }
    }
    free(stam);
    return;
}

void prtchaharr(snodm **stam, unsigned tsz)
{
    unsigned i;
    snodm *tsnod2;
    for(i=0;i<tsz;++i) {
        printf("Tablepos %i: ", i); 
        tsnod2=stam[i];
        while(tsnod2) {
            printf("%s ", tsnod2->aw->aw[0]->w);
            tsnod2=tsnod2->n;
        }
        putchar('\n');
    }
    return;
}

w_c *crea_wc(unsigned initsz)
{
    w_c *wc=malloc(sizeof(w_c));
    wc->lp1=initsz;
    wc->t=STRG;
    wc->w=malloc(wc->lp1*sizeof(char));
    return wc;
}

void reall_wc(w_c **wc, unsigned *cbuf)
{
    w_c *twc=*wc;
    unsigned tcbuf=*cbuf;
    tcbuf += CBUF;
    twc->lp1=tcbuf;
    twc->w=realloc(twc->w, tcbuf*sizeof(char));
    *wc=twc; /* realloc can often change the ptr */
    *cbuf=tcbuf;
    return;
}

void norm_wc(w_c **wc)
{
    w_c *twc=*wc;
    twc->w=realloc(twc->w, twc->lp1*sizeof(char));
    *wc=twc; /* realloc can often change the ptr */
    return;
}

void free_wc(w_c **wc)
{
    w_c *twc=*wc;
    free(twc->w);
    free(twc);
    return;
}

aw_c *crea_awc(unsigned initsz)
{
    int i;
    aw_c *awc=malloc(sizeof(aw_c));
    awc->ab=initsz;
    awc->al=awc->ab;
    awc->aw=malloc(awc->ab*sizeof(w_c*));
    for(i=0;i<awc->ab;++i) 
        awc->aw[i]=crea_wc(CBUF);
    return awc;
}

void reall_awc(aw_c **awc, unsigned buf)
{
    int i;
    aw_c *tawc=*awc;
    tawc->ab += buf;
    tawc->al=tawc->ab;
    tawc->aw=realloc(tawc->aw, tawc->ab*sizeof(aw_c*));
    for(i=tawc->ab-buf;i<tawc->ab;++i)
        tawc->aw[i]=crea_wc(CBUF);
    *awc=tawc;
    return;
}

void norm_awc(aw_c **awc)
{
    int i;
    aw_c *tawc=*awc;
    /* free the individual w_c's */
    for(i=tawc->al;i<tawc->ab;++i) 
        free_wc(tawc->aw+i);
    /* now release the pointers to those freed w_c's */
    tawc->aw=realloc(tawc->aw, tawc->al*sizeof(aw_c*));
    *awc=tawc;
    return;
}

void free_awc(aw_c **awc)
{
    int i;
    aw_c *tawc=*awc;
    for(i=0;i<tawc->al;++i) 
        free_wc(tawc->aw+i);
    free(tawc->aw); /* unbelieveable: I left this out, couldn't find where I leaking the memory! */
    free(tawc);
    return;
}

aaw_c *crea_aawc(unsigned initsz)
{
    int i;
    unsigned lbuf=initsz;
    aaw_c *aawc=malloc(sizeof(aaw_c));
    aawc->numl=0;
    aawc->aaw=malloc(lbuf*sizeof(aw_c*));
    for(i=0;i<initsz;++i) 
        aawc->aaw[i]=crea_awc(WABUF);
    return aawc;
}

void free_aawc(aaw_c **aw)
{
    int i;
    aaw_c *taw=*aw;
    for(i=0;i<taw->numl;++i) /* tried to release 1 more, no go */
        free_awc(taw->aaw+i);
    free(taw->aaw);
    free(taw);
}

void prtaawcdbg(aaw_c *aawc)
{
    int i, j, k;
    for(i=0;i<aawc->numl;++i) {
        printf("l.%u(%u): ", i, aawc->aaw[i]->al); 
        for(j=0;j<aawc->aaw[i]->al;++j) {
            printf("w_%u: ", j); 
            if(aawc->aaw[i]->aw[j]->t == NUMS) {
                printf("NUM! "); 
                continue;
            } else if(aawc->aaw[i]->aw[j]->t == PNI) {
                printf("PNI! "); 
                continue;
            } else if(aawc->aaw[i]->aw[j]->t == STCP) {
                printf("STCP! "); 
                continue;
            }
            for(k=0;k<aawc->aaw[i]->aw[j]->lp1-1; k++)
                putchar(aawc->aaw[i]->aw[j]->w[k]);
            printf("/%u ", aawc->aaw[i]->aw[j]->lp1-1); 
        }
        printf("\n"); 
    }
}

void prtaawcdata(aaw_c *aawc) /* print line and word details, but not the words themselves */
{
    int i, j;
    for(i=0;i<aawc->numl;++i) {
        printf("L%u(%uw):", i, aawc->aaw[i]->al); 
        for(j=0;j<aawc->aaw[i]->al;++j) {
            printf("l%ut", aawc->aaw[i]->aw[j]->lp1-1);
            switch(aawc->aaw[i]->aw[j]->t) {
                case NUMS: printf("N "); break;
                case PNI: printf("I "); break;
                case STRG: printf("S "); break;
                case STCP: printf("C "); break; /* closing punctuation */
                case SCST: printf("Z "); break; /* starting capital */
                case SCCP: printf("Y "); break; /* starting capital and closing punctuation */
                case ALLC: printf("A "); break; /* horrid! all capitals */
            }
        }
    }
    printf("\n"); 
    printf("L is a line, l is length of word, S is normal string, C closing punct, Z, starting cap, Y Starting cap and closing punct.\n"); 
}

void prtaawcplain(aaw_c *aawc) /* print line and word details, but not the words themselves */
{
    int i, j;
    for(i=0;i<aawc->numl;++i) {
        printf("L%u(%uw):", i, aawc->aaw[i]->al); 
        for(j=0;j<aawc->aaw[i]->al;++j) {
            // printf("s1+%u ", aawc->aaw[i]->aw[j]->lp1);
            printf((j!=aawc->aaw[i]->al-1)?"%s ":"%s\n", aawc->aaw[i]->aw[j]->w);
        }
    }
}

aaw_c *processinpf(char *fname)
{
    /* declarations */
    FILE *fp=fopen(fname,"r");
    int i;
    size_t couc /*count chars per line */, couw=0 /* count words */;
    int c, oldc='\0';
    boole inword=0;
    unsigned lbuf=LBUF /* buffer for number of lines */, cbuf=CBUF /* char buffer for size of w_c's: reused for every word */;
    aaw_c *aawc=crea_aawc(lbuf); /* array of words per line */

    while( (c=fgetc(fp)) != EOF) {
        if( (c== '\n') | (c == ' ') | (c == '\t') ) {
            if( inword==1) { /* cue word-ending procedure */
                aawc->aaw[aawc->numl]->aw[couw]->w[couc++]='\0';
                aawc->aaw[aawc->numl]->aw[couw]->lp1=couc;
                SETCPTYPE(oldc, aawc->aaw[aawc->numl]->aw[couw]->t);
                norm_wc(aawc->aaw[aawc->numl]->aw+couw);
                couw++; /* verified: this has to be here */
            }
            if(c=='\n') { /* cue line-ending procedure */
                if(aawc->numl ==lbuf-1) {
                    lbuf += LBUF;
                    aawc->aaw=realloc(aawc->aaw, lbuf*sizeof(aw_c*));
                    for(i=lbuf-LBUF; i<lbuf; ++i)
                        aawc->aaw[i]=crea_awc(WABUF);
                }
                aawc->aaw[aawc->numl]->al=couw;
                norm_awc(aawc->aaw+aawc->numl);
                aawc->numl++;
                couw=0;
            }
            inword=0;
        } else if(inword==0) { /* a normal character opens word */
            if(couw ==aawc->aaw[aawc->numl]->ab-1) /* new word opening */
                reall_awc(aawc->aaw+aawc->numl, WABUF);
            couc=0;
            cbuf=CBUF;
            aawc->aaw[aawc->numl]->aw[couw]->w[couc++]=c;
            GETLCTYPE(c, aawc->aaw[aawc->numl]->aw[couw]->t); /* MACRO: the firt character gives a clue */
            inword=1;
        } else if(inword) { /* simply store */
            if(couc == cbuf-1)
                reall_wc(aawc->aaw[aawc->numl]->aw+couw, &cbuf);
            aawc->aaw[aawc->numl]->aw[couw]->w[couc++]=c;
            /* if word is a candidate for a NUM or PNI (i.e. via its first character), make sure it continues to obey rules: a MACRO */
            IWMODTYPEIF(c, aawc->aaw[aawc->numl]->aw[couw]->t);
        }
        oldc=c;
    } /* end of big for statement */
    fclose(fp);

    /* normalization stage */
    for(i=aawc->numl; i<lbuf; ++i) {
        free_awc(aawc->aaw+i);
    }
    aawc->aaw=realloc(aawc->aaw, aawc->numl*sizeof(aw_c*));

    return aawc;
}

int main(int argc, char *argv[])
{
    /* argument accounting */
    if(argc!=2) {
        printf("Error. Pls supply argument (name of text file).\n");
        exit(EXIT_FAILURE);
    }

    aaw_c *aawc=processinpf(argv[1]);

    char c;
    int i, k=0;
    av_c *avc=crea_avc(GBUF);
    while( (c=aawc->aaw[0]->aw[0]->w[k++]) != '\0')
        app_avc(avc, c);
    for(i=0;i<avc->vsz;++i)
        putchar(avc->v[i]); 
    printf("\n"); 

    unsigned htsz=givehtsz(aawc->numl);
    snodm **stam = hashnam(aawc, htsz);
    // prtchaharr(stam, htsz);
    av_c *av2=NULL;
    char t[3]={0};
    size_t kk=0, j;
    char *s=aawc->aaw[0]->aw[0]->w;
    size_t sl=aawc->aaw[0]->aw[0]->lp1-1;
    char *s2=malloc(sl*sizeof(char));
    memcpy(s2, s, sl*sizeof(char));
    for(j=0;j<14;++j) {  
        // printf("start str: "); 
        // for(i=0;i<sl;++i) 
        //     putchar(s2[i]);
        // printf("\n"); 
        av2=crea_avc(GBUF);
        kk=0;
        for(i=0;i<sl-1;++i) {
            t[0]=s2[kk++];
            t[1]=s2[kk];
            app_avc(av2, t[0]);
            app_avc(av2, mu2(aawc, stam, htsz, t));
            //app_avc(av2, t[1]);
        }
        app_avc(av2, s2[sl-1]);
        sl=av2->vsz;
        s2=realloc(s2, sl*sizeof(char));
        memcpy(s2, av2->v, sl*sizeof(char));
        printf("end str: "); 
        for(i=0;i<sl;++i) 
            putchar(s2[i]);
        printf("\n"); 
        free_avc(av2);
    }
    freechainharr(stam, htsz);

    size_t cou[10]={0};
    for(i=0;i<sl;++i) {
        switch(s2[i]) {
            case 'B':
                cou[0]++; break;
            case 'C':
                cou[1]++; break;
            case 'F':
                cou[2]++; break;
            case 'H':
                cou[3]++; break;
            case 'K':
                cou[4]++; break;
            case 'N':
                cou[5]++; break;
            case 'O':
                cou[6]++; break;
            case 'P':
                cou[7]++; break;
            case 'S':
                cou[8]++; break;
            case 'V':
                cou[9]++; break;
        }
    }
    size_t mx=0;
    size_t mn=0xFFFFFFFFFFFFFFFF;
    for(i=0;i<10;++i) {
        if(mn>cou[i])
            mn=cou[i];
        if(mx<cou[i])
            mx=cou[i];
    }
    for(i=0;i<10;++i) 
        printf("%zu ", cou[i]);
    printf("\n"); 
    printf("max-min=%zu\n", mx-mn); 

    free_aawc(&aawc);
    free_avc(avc);
    free(s2);

    return 0;
}
