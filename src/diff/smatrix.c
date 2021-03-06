#include "smatrix.h"

/* Statiques */
static Smatrix* smatrix_last(Smatrix *s);
static Smatrix* smatrix_at(Smatrix *s, ulint y);
static void smatrix_free_intern(Smatrix *s);

void smatrix_append(Smatrix *s, ulint y) {

    /* Première case */
    if(s->value == 0) {
        s->y = y;
        s->value = 1;
        s->next = NULL;
    }
    /* Autres case à ajouter */
    else {
        s = smatrix_last(s);
        s->next = (Smatrix*)malloc(sizeof(Smatrix));
        s->next->y = y;
        s->next->value = 1;
        s->next->next = NULL;
    }

}

static Smatrix* smatrix_last(Smatrix *s) {

    while(s && s->next)
        s = s->next;

    return s;
}


static void smatrix_free_intern(Smatrix *s) {

    Smatrix *tmp = NULL;

    while(s) {
        tmp = s;
        s = s->next;
        free(tmp);
    }
}

void smatrix_free(Smatrix s[], ulint length) {

    for(; length > 0; length--)
        smatrix_free_intern(s[length-1].next);

    free(s);
}

static Smatrix* smatrix_at(Smatrix *s, ulint y) {

    while(s) {
        if(s->value && s->y == y)
            break;
        s = s->next;
    }

    return s;
}

ulint smatrix_to_index(Smatrix s[], Index *i1, Index *i2, ulint start) {

    ulint i = 0, j = 0, max_length = 0, max_start = 0;
    Smatrix *tmp = NULL;
    ulint tmp_length = 0, tmp_start = 0;

    ulint changes = start;

    /* On passe tout à 1 */
    for(; i < i1->line_max; i++)
        i1->lines[i].modified = 1;

    for(; j < i2->line_max; j++)
        i2->lines[j].modified = 1;

    /* Les lignes avant start sont non modifiées */
    for(i = 0; i < start; i++) {
        i1->lines[i].modified = 0;
        i2->lines[i].modified = 0;
    }

    /* Pour chaque cols de la matrice */
    for(; i < i1->line_max; i++) {
        tmp = &s[i];

        /* Si elle contient au moins une valeur */
        if(tmp->value == 1) {
            max_length = 0; //Pour être certain que le premier cas est valide

            do {
                tmp_length = 1;
                tmp_start = tmp->y;

                for(j = i+1; j < i1->line_max; j++) {
                    /* On cherche le suivant en diagonale */
                    if(smatrix_at(&s[i], tmp_start + tmp_length) != NULL)
                        tmp_length++;
                    else
                        break; // Plus rien, on sort
                }

                /* Si on dépassé le max */
                if(tmp_length > max_length) {
                    max_length = tmp_length;
                    max_start = tmp_start;
                }


            } while((tmp = tmp->next));

            /* On marque les lignes */
            for(j = i; j < i + max_length; j++)
                i1->lines[j].modified = 0;

            for(j = max_start; j < max_start + max_length; j++)
                i2->lines[j].modified = 0;

            /* On avance */
            changes += max_length;
            i += max_length-1;
        }
    }

    return changes;
}
