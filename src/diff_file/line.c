#include "line.h"


/* ===============================================
                    line_search

    Permet de chercher une lignes line dans f1, a
    partir de la ligne courante (comprise).
    ----------------------------------------------
    t_index *line : ligne à chercher.
    t_index *f1   : fichier où chercher la ligne.
    ----------------------------------------------
    Retour : Retour le numéro de ligne en cas de
             succès, -1 sinon.
   =============================================== */
int line_search(t_index *line, t_index *f1) {

    unsigned int sav = f1->line, line_to_search = 0;
    line_error ret = _NO_ERROR;

    while(ret == _NO_ERROR && line_compare(line, f1) != 0)
        ret = line_next(f1);

    line_to_search = f1->line;
    line_go_to(f1, sav);

    if(ret == _NO_ERROR)
        return line_to_search;
    else
        return -1;
}

/* ===============================================
                    line_compare

    Permet de comparer deux lignes
    ----------------------------------------------
    t_index *f : fichier où avancer d'une ligne.
    ----------------------------------------------
    Retour : Retour 0 en cas de succès, ou sinon
             retourne l'erreur.
   =============================================== */
int line_compare(t_index *l1, t_index *l2) {

    char tmp1 = 0, tmp2 = 0;

    if(l1 && l2) {
        /* On se place */
        line_go_to(l1, l1->line);
        line_go_to(l2, l2->line);

        /* On compare */
        while((tmp1 = fgetc(l1->f)) == (tmp2 = fgetc(l2->f))) {
            /* Si fin de ligne, on saute */
            if(tmp1 == END_LINE || tmp1 == EOF) {
                line_go_to(l1, l1->line); // On revient à la ligne en cours
                line_go_to(l2, l2->line);
                return 0;
            }
        }
        line_go_to(l1, l1->line); // On revient à la ligne en cours
        line_go_to(l2, l2->line);

        if((tmp1 == END_LINE && tmp2 == EOF) || (tmp2 == END_LINE && tmp1 == EOF))
            return 0;
        else
            return 1;

    } else
        return -1;

}

/* ===============================================
                    line_next

    Permet de passer à la ligne suivante dans
    le fichier
    ----------------------------------------------
    t_index *f : fichier où avancer d'une ligne.
    ----------------------------------------------
    Retour : Retour 0 en cas de succès, ou sinon
             retourne l'erreur.
   =============================================== */
line_error line_next(t_index *f) {
    if(f) {
        if(f->line+1 < f->line_max) {
            f->line++;
            if(!(f->lines))
                fseek(f->f, f->index[f->line], SEEK_SET);
            return _NO_ERROR;
        } else
            return END_FILE;
    } else
        return EMPTY_STRUCT;
}

/* ===============================================
                    line_previous

    Permet de revenir à la ligne précédente dans
    le fichier
    ----------------------------------------------
    t_index *f : fichier où remonter d'une ligne.
    ----------------------------------------------
    Retour : Retour 0 en cas de succès, ou sinon
             retourne l'erreur.
   =============================================== */
line_error line_previous(t_index *f) {
    if(f) {
        if(f->line > 0) {
            f->line--;
            if(!(f->lines))
                fseek(f->f, f->index[f->line], SEEK_SET);
            return _NO_ERROR;
        } else
            return START_FILE;
    } else
        return EMPTY_STRUCT;
}

/* ===============================================
                    line_go_to

    Permet d'aller à la ligne indiquée. Si celle-ci
    dépasse les bornes de l'index, une erreur sera
    renvoyée et le curseur déplacé au nombre le
    plus proche.
    ----------------------------------------------
    t_index *f : fichier où changer de ligne.
    n          : numéro de ligne
    ----------------------------------------------
    Retour : Retour 0 en cas de succès, ou sinon
             retourne l'erreur.
   =============================================== */
line_error line_go_to(t_index *f, unsigned int n) {

    if(f) {
        if(n >= f->line_max) {
            f->line = f->line_max-1;
            if(!(f->lines))
                fseek(f->f, f->index[f->line], SEEK_SET);
            return END_FILE;
        } else {
            f->line = n;
            if(!(f->lines))
                fseek(f->f, f->index[f->line], SEEK_SET);
            return _NO_ERROR;
        }
    } else
        return EMPTY_STRUCT;
}

/* ===============================================
                  lines_display

    Affiche les lignes de f de start à end (compris)
    en ajoutant line_start avant chaque ligne.
    Saute une ligne après chaque ligne écrite.
    ----------------------------------------------
    t_index *f       : fichier où sont les lignes.
    u int start      : première ligne.
    u int end        : dernière ligne.
    char* line_start : à afficher avant chaque ligne
   =============================================== */
void lines_display(t_index *f, unsigned int start, unsigned int end, const char *line_start) {
    lines_display_lenght(f, start, end, line_start, 0);
}

/* ===============================================
                lines_display_lenght

    Affiche les lignes de f de start à end (compris)
    en ajoutant line_start avant chaque ligne avec
    un maximum de lenght caractères. 0 permet de ne
    pas utiliser de limitation.
    Saute une ligne après chaque ligne écrite.
    ----------------------------------------------
    t_index *f       : fichier où sont les lignes.
    u int start      : première ligne.
    u int end        : dernière ligne.
    char* line_start : à afficher avant chaque ligne
    u int lenght     : nombre de char max
   =============================================== */
void lines_display_lenght(t_index *f, unsigned int start, unsigned int end, const char *line_start, unsigned int lenght) {
    unsigned int i = 0, j = 0;
    char c = 0;

    if(f) {
        if(start <= end && end <= f->line_max) {
            for(i = start; i <= end; i++, j = 0) {

                if(line_start)
                    fputs(line_start, stdout);

                if(lenght > 0) {
                    if(f->lines) {
                        for(j = 0; j < strlen(f->lines[i]) && j < lenght && j != '\n'; j++) {
                            if(p->expand_tab && f->lines[i][j] == '\t' )
                                print_space(p->size_tab);
                            else
                                putchar(f->lines[i][j]);
                        }

                        putchar('\n');
                    } else {
                        fseek(f->f, f->index[i], SEEK_SET);

                        while((c = getc(f->f)) != END_LINE && c != EOF && j < lenght) {
                            if(p->expand_tab && c == '\t')
                                print_space(p->size_tab);
                            else
                                putchar(c);
                            j++;
                        }

                        putchar('\n');
                    }
                } else {
                    if(f->lines) {
                        fputs(f->lines[i],stdout);

                        j = strlen(f->lines[i]);
                        if(j == 0 || f->lines[i][j-1] != '\n')
                            fputc((int)'\n', stdout);


                    } else {
                        fseek(f->f, f->index[i], SEEK_SET);

                        while((c = getc(f->f)) != END_LINE && c != EOF) {
                            if(p->expand_tab && c == '\t' )
                                print_space(p->size_tab);
                            else if(c != '\r')
                                putchar(c);
                        }

                        if(c == END_LINE)
                            putchar('\n');
                    }
                }
            }
        }
    }
}

void print_space(unsigned int n) {
    unsigned int i = 0;

    for(i = 0; i < n; i++)
        fputc(' ', stdout);
}
