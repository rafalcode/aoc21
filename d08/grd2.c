/* modification of matread but operating on words instead of floats */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "grd.h"

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
#ifdef DBG2
    printf("typeszs: aaw_c: %zu aw_c: %zu w_c: %zu\n", sizeof(aaw_c), sizeof(aw_c), sizeof(w_c));
#endif

    aaw_c *aawc=processinpf(argv[1]);
#ifdef DBG
    prtaawcdbg(aawc);
#else
    prtaawcdata(aawc); // just the metadata
    // prtaawcplain(aawc); // printout original text as well as you can.
#endif
    // printf("Numlines: %zu\n", aawc->numl); 
    int k, i, j, sum =0;
    // for(i=0;i<aawc->numl;++i)
    //     for(j=11;j<15;++j)    
    //         if((aawc->aaw[i]->aw[j]->lp1 == 3) | (aawc->aaw[i]->aw[j]->lp1 == 4) | (aawc->aaw[i]->aw[j]->lp1 == 5) | (aawc->aaw[i]->aw[j]->lp1 == 8))
    //             sum++;
    // printf("sum=%i\n",sum); 
    // 1 and 7
    // 1 and 7
    int *ns=malloc(10*sizeof(int));
    int *ns2=malloc(10*sizeof(int));
    for(i=0;i<10;++i) 
        ns[i]=-1;
    // i=0; // line 0 for testing
    int *tn=calloc(5, sizeof(int));
    int l0, s0, jj;
    unsigned char *m=malloc(3*sizeof(unsigned char));
    unsigned char *m5=malloc(5*sizeof(unsigned char));
    int nu, sumnu=0;
    int ii, s2, s3, s9, s6;
    for(i=0;i<aawc->numl;++i) {

        for(j=0;j<10;++j) {
            switch(aawc->aaw[i]->aw[j]->lp1) {
                case 3:
                    // printf("%s\n", aawc->aaw[i]->aw[j]->w);
                    ns[1]=j; break;
                case 4:
                    ns[7]=j; break;
                case 5:
                    ns[4]=j; break;
                case 8:
                    ns[8]=j; break;
                default:
                    break;
            }
        }

        // try comparing pos 1 and pos 7
        // memset(m, 0, 3*sizeof(unsigned char));
        // for(k=0;k<2;++k)
        //     for(j=0;j<3;++j) {
        //         if(aawc->aaw[i]->aw[ns[1]]->w[k] == aawc->aaw[i]->aw[ns[7]]->w[j]) {
        //             m[j]=1;
        //             break;
        //         }
        //     }
        // for(k=0;k<3;++k) {
        //     // printf("%i", (int)m[i]);
        //     if(!m[k])
        //         lets[0]= aawc->aaw[i]->aw[ns[7]]->w[k];
        // }

        // identify 2 via 4 with whom (of the 3) only 2 matches
        memset(m5, 0, 5*sizeof(unsigned char));
        for(j=0;j<10;++j) {
            switch(aawc->aaw[i]->aw[j]->lp1) {
                case 3: case 4: case 5: case 8:
                    break;
                case 6:
                    s2=0;
                    for(k=0;k<4;++k) {
                        for(ii=0;ii<5;++ii) {
                            if(aawc->aaw[i]->aw[ns[4]]->w[k] == aawc->aaw[i]->aw[j]->w[ii]) {
                                s2++;
                                break;
                            }
                        }
                    }
                    if(s2==2) {
                        ns[2]=j;
                        break;
                    }
            }
        }
        // now1 will only match 3 twice, and remaining 5 segment number must be 5
        for(j=0;j<10;++j) {
            switch(aawc->aaw[i]->aw[j]->lp1) {
                case 3: case 4: case 5: case 8:
                    break;
                case 6:
                    if(j==ns[2])
                        continue;
                    s3=0;
                    for(k=0;k<2;++k) {
                        for(ii=0;ii<5;++ii) {
                            if(aawc->aaw[i]->aw[ns[1]]->w[k] == aawc->aaw[i]->aw[j]->w[ii]) {
                                s3++;
                                break;
                            }
                        }
                    }
                    if(s3==2) {
                        ns[3]=j;
                    } else
                        ns[5]=j;
            }
        }
        s9=0;
        for(j=0;j<10;++j) {
            switch(aawc->aaw[i]->aw[j]->lp1) {
                case 3: case 4: case 5: case 8: case 6:
                    break;
                case 7:
                    // of the 3 6seg numbers, only 3 will match 9 5 times.
                    s9=0;
                    for(k=0;k<5;++k) {
                        for(ii=0;ii<6;++ii) {
                            if(aawc->aaw[i]->aw[ns[3]]->w[k] == aawc->aaw[i]->aw[j]->w[ii]) {
                                s9++;
                                break;
                            }
                        }
                    }
                    if(s9==5)
                        ns[9]=j;
            }
        }
        s6=0;
        for(j=0;j<10;++j) {
            switch(aawc->aaw[i]->aw[j]->lp1) {
                case 3: case 4: case 5: case 8: case 6:
                    break;
                case 7:
                    if(j==ns[9])
                        continue;
                    // of the 2 remaining (less 9) 6seg numbers, only 5 will match 6 5 times.
                    s6=0;
                    for(k=0;k<5;++k) {
                        for(ii=0;ii<6;++ii) {
                            if(aawc->aaw[i]->aw[ns[5]]->w[k] == aawc->aaw[i]->aw[j]->w[ii]) {
                                s6++;
                                break;
                            }
                        }
                    }
                    if(s6==5) {
                        ns[6]=j;
                    } else
                        ns[0]=j;
            }
        }
        //need to reverse indices for problem exposition.
        for(j=0;j<10;++j)
            ns2[ns[j]]=j;

        // now identify number on last line
        for(j=11;j<15;++j) {
            l0=aawc->aaw[i]->aw[j]->lp1;
            // printf("trying: %s  ..... \n", aawc->aaw[i]->aw[j]->w);
            for(k=0;k<10;++k) {
                if(aawc->aaw[i]->aw[k]->lp1 != l0) {
//                    printf("skip: %s\n", aawc->aaw[i]->aw[k]->w);
                    continue;
                }
                s0=0;
                // printf("%s vs %s\n", aawc->aaw[i]->aw[k]->w, aawc->aaw[i]->aw[j]->w);
                for(ii=0;ii<(l0-1);++ii)
                    for(jj=0;jj<(l0-1);++jj)
                        if(aawc->aaw[i]->aw[k]->w[jj] == aawc->aaw[i]->aw[j]->w[ii]) {
                            s0++;
                            break;
                        }
                if(s0==(l0-1)) {
                    // printf("SUCCEEDED: %s vs %s i.e. num %i\n", aawc->aaw[i]->aw[k]->w, aawc->aaw[i]->aw[j]->w, ns2[k]);
                    tn[j-11]=ns2[k];
                    break;
                }
            }
        }
        nu=1000*tn[0]+100*tn[1]+10*tn[2]+tn[3];
        // printf("nu is %i ", nu);
        sumnu+=nu;
    }
    printf("total of all reconnected numbers=%i\n", sumnu); 


    free_aawc(&aawc);
    free(ns);
    free(ns2);
    free(m);
    free(m5);
    free(tn);

    return 0;
}
