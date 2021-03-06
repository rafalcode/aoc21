/* modification of matread but operating on words instead of floats */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "p4.h"

void optis(av_c *avc) // order only on m members
{
    int i, j, t, t2;
    for (i = 1; i < avc->vsz; i++) {
        t = avc->m[i];
        t2 = avc->n[i];
        for (j = i; ((j > 0) && (avc->m[j-1]) < t); j--) {
            avc->m[j] = avc->m[j-1];
            avc->n[j] = avc->n[j-1];
        }
        avc->m[j] = t;
        avc->n[j] = t2;
    }
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

void lesslast_avc(av_c *avc)
{
    /* somewhat trivial, but idea is that, as avc is a container, it can be re-alloced inside a function */
    if(avc->vsz>1) {
        avc->vsz--;
        // avc->m=realloc(avc->m, avc->vsz*sizeof(int));
        // avc->n=realloc(avc->n, avc->vsz*sizeof(int));
    } else {
        avc->vsz=0;
    }
    return;
}

void free_avc(av_c *avc)
{
    free(avc->m);
    free(avc->n);
    free(avc);
    return;
}

void prt_avc(av_c *avc)
{
    int i;
    for(i=0;i<avc->vsz;++i) 
        printf((i==avc->vsz-1)?"(%i,%i)\n":"(%i,%i) ", avc->m[i], avc->n[i]);
    return;
}

void prt_avc2(av_c *avc)
{
    int i;
    printf("sz%i) ", avc->vsz);
    for(i=0;i<avc->vsz;++i) 
        printf((i==avc->vsz-1)?"(%i,%i)\n":"(%i,%i) ", avc->m[i], avc->n[i]);
    return;
}

void prt_avc4(av_c *avc)
{
    printf("Top 3 size=%i\n", avc->m[0]*avc->m[1]*avc->m[2]);
    return;
}

void prt_avc3(av_c *avc)
{
    printf("Basin size=%i\n", avc->vsz+1);
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
        for(j=0;j<aawc->aaw[i]->al;++j)
            printf((j!=aawc->aaw[i]->al-1)?"%s ":"%s\n", aawc->aaw[i]->aw[j]->w);
    }
}

av_c *glow0(aaw_c *aawc) /* print line and word details, but not the words themselves */
{
    int i, k;
    int *a=malloc(4*sizeof(int));
    av_c *avc=crea_avc(GBUF);
    int su=0;
    for(i=0;i<aawc->numl;++i) {
        for(k=0;k<aawc->aaw[i]->aw[0]->lp1; k++) {
                a[0]=(i>0)?aawc->aaw[i-1]->aw[0]->w[k]-48:99;
                a[2]=(i<aawc->numl-1)?aawc->aaw[i+1]->aw[0]->w[k]-48:99;
                a[3]=(k>0)?aawc->aaw[i]->aw[0]->w[k-1]-48:99;
                a[1]=(k<aawc->aaw[i]->aw[0]->lp1-2)?aawc->aaw[i]->aw[0]->w[k+1]-48:99;
                // printf("(%i,%i) %i: %i %i %i %i\n", i, k, aawc->aaw[i]->aw[0]->w[k]-48, a[0], a[1], a[2], a[3]); 
                if(((aawc->aaw[i]->aw[0]->w[k]-48)<a[0]) & ((aawc->aaw[i]->aw[0]->w[k]-48)<a[1]) & ((aawc->aaw[i]->aw[0]->w[k]-48)<a[2]) & ((aawc->aaw[i]->aw[0]->w[k]-48)<a[3]) ) {
                    app_avc(avc, k, i);
                    su+=(aawc->aaw[i]->aw[0]->w[k]-48);
                }
        }
    }
    printf("su=%i, risk=%i\n", su, su+avc->vsz); 
    free(a);
    return avc;
}

void glow2(aaw_c *aawc, av_c *avc)
{
    int cc, rr, j, i;
    int *a=malloc(4*sizeof(int));
    av_c *av2=NULL;
    av_c *av3=crea_avc(GBUF);
    for(j=0;j<avc->vsz;++j) {
        av2=crea_avc(GBUF);
        cc=avc->m[j]; // aka. x
        rr=avc->n[j]; //aka y
        aawc->aaw[rr]->aw[0]->w[cc]='9'; // clobber
        a[0]=(rr>0)?aawc->aaw[rr-1]->aw[0]->w[cc]-48:9;
        if(a[0]!=9)
            app_avc(av2, cc, rr-1);
        if(rr>0)
            aawc->aaw[rr-1]->aw[0]->w[cc]='9'; // clobb
        a[2]=(rr<aawc->numl-1)?aawc->aaw[rr+1]->aw[0]->w[cc]-48:9;
        if(a[2]!=9)
            app_avc(av2, cc, rr+1);
        if(rr<aawc->numl-1)
            aawc->aaw[rr+1]->aw[0]->w[cc]='9';
        a[3]=(cc>0)?aawc->aaw[rr]->aw[0]->w[cc-1]-48:9;
        if(a[3]!=9)
            app_avc(av2, cc-1, rr);
        if(cc>0)
            aawc->aaw[rr]->aw[0]->w[cc-1]='9';
        a[1]=(cc<aawc->aaw[rr]->aw[0]->lp1-2)?aawc->aaw[rr]->aw[0]->w[cc+1]-48:9;
        if(a[1]!=9)
            app_avc(av2, cc+1, rr);
        if(cc<aawc->aaw[rr]->aw[0]->lp1-2)
            aawc->aaw[rr]->aw[0]->w[cc+1]='9';
        for(i=0;i<av2->vsz;++i) {
            cc=av2->m[i];
            rr=av2->n[i];
            // lesslast_avc(av2); 
            aawc->aaw[rr]->aw[0]->w[cc]='9'; // clobber
            a[0]=(rr>0)?aawc->aaw[rr-1]->aw[0]->w[cc]-48:9;
            if(a[0]!=9)
                app_avc(av2, cc, rr-1);
            if(rr>0)
                aawc->aaw[rr-1]->aw[0]->w[cc]='9';
            a[2]=(rr<aawc->numl-1)?aawc->aaw[rr+1]->aw[0]->w[cc]-48:9;
            if(a[2]!=9)
                app_avc(av2, cc, rr+1);
            if(rr<aawc->numl-1)
                aawc->aaw[rr+1]->aw[0]->w[cc]='9';
            a[3]=(cc>0)?aawc->aaw[rr]->aw[0]->w[cc-1]-48:9;
            if(a[3]!=9)
                app_avc(av2, cc-1, rr);
            if(cc>0)
                aawc->aaw[rr]->aw[0]->w[cc-1]='9';
            a[1]=(cc<aawc->aaw[rr]->aw[0]->lp1-2)?aawc->aaw[rr]->aw[0]->w[cc+1]-48:9;
            if(a[1]!=9)
                app_avc(av2, cc+1, rr);
            if(cc<aawc->aaw[rr]->aw[0]->lp1-2)
                aawc->aaw[rr]->aw[0]->w[cc+1]='9';
        }
        prt_avc3(av2);
        app_avc(av3, av2->vsz+1, 0);
        free_avc(av2);
    }
    prt_avc2(av3);
    optis(av3);
    prt_avc4(av3);
    free(a);
    free_avc(av3);
    return;
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
    av_c *avc=glow0(aawc);
    prt_avc(avc);
    glow2(aawc, avc);
    // for(i=0;i<avc->vsz;++i)
    //    printf("(%i,%i) ", avc->m[i], avc->n[i]);
    // printf("\n"); 

    free_aawc(&aawc);
    memexact_avc(avc);
    free_avc(avc);

    return 0;
}
