#include <stdio.h>
#include "prover.h"
#include "symbol_table.h"
#include "clauses.h"

extern void insert_clause_set (tclause **clause, int type, int set);
extern int size_clause (literalslist *l);
extern int max_lit_clause (literalslist *l);
extern int classify_clause (literalslist *l);
extern tclause *get_clause (int clause);
extern literalslist *simplify_clause(literalslist *l);

extern int numclause;
extern int numfsubsumed;
extern int numbsubsumed;
extern int numtautologies;
extern int diamond_restricted_res;

tclause *resolve_clauses_gen2(tclause *c1, tclause *c2, tclause *c3) {
  tclause *resolvent = NULL;
  int num_subtype = 0;
  if (c1->subtype > BOX)
    num_subtype++;
  if (c2->subtype > BOX)
    num_subtype++;
  if (c3->subtype > BOX)
    num_subtype++;
  if (!diamond_restricted_res || num_subtype <= 1) {
    if (c2->right->literal == -c3->right->literal) {
      int subtype = BOX;
      if (c1->subtype > BOX)
        subtype = c1->subtype;
      else if (c2->subtype > BOX)
        subtype = c2->subtype;
      else if (c3->subtype > BOX)
        subtype = c3->subtype;
    resolvent = malloc(sizeof(tclause));
    resolvent->number = numclause++;
    resolvent->type = LITERAL;
    resolvent->subtype = subtype;
    resolvent->numdiamond = BOX;
    //resolvent->subtype = BOX;
    resolvent->modal_level = c1->modal_level;
    resolvent->agent = 0;
    resolvent->left = CTRUE;
    literalslist *l = malloc(sizeof(literalslist));
    l->literal = -c1->left;
    l->next = malloc(sizeof(literalslist));
    l->next->literal = -c2->left;
    l->next->next = malloc(sizeof(literalslist));
    l->next->next->literal = -c3->left;
    l->next->next->next = NULL;
    resolvent->right = simplify_clause(l);
    resolvent->size = size_clause(resolvent->right);
    resolvent->max_literal = max_lit_clause(resolvent->right);
    resolvent->min_literal = resolvent->right->literal;
    resolvent->class = classify_clause(resolvent->right);
    
    clauseslist *parent = malloc(sizeof(clauseslist));
    parent->clause_number = c1->number;
    parent->next = malloc(sizeof(clauseslist));
    parent->next->clause_number = c2->number;
    parent->next->next = malloc(sizeof(clauseslist));
    parent->next->next->clause_number = c3->number;
    parent->next->next->next = NULL;
    literalslist *literals = malloc(sizeof(literalslist));
    literals->literal = c2->right->literal;
    literals->next = NULL;
    struct justification *just = malloc(sizeof(justification));
    just->rule = GEN2;
    just->parents = parent;
    just->literals = literals;
    resolvent->just = just;
    resolvent->deleted = NULL;
    }
  }
  return resolvent;
}

void resolve_sets_gen2 (hmaxlitclauses *negative, hmaxlitclauses *positive) {
  hmaxlitclauses *neg, *pos1, *pos2;
  clauseslist *neglist, *poslist;

  for (neg = negative; neg != NULL; neg = neg->hmax.next) {
    for(neglist = neg->list; neglist != NULL; neglist = neglist->next) {
      tclause *aux_neg = get_clause(neglist->clause_number);
      for (pos1 = positive; pos1 != NULL; pos1 = pos1->hmax.next) {
	if (pos1->max > 0) {
	  poslist = pos1->list;
	  while (poslist != NULL) {
	    tclause *aux_pos = get_clause(poslist->clause_number);
	    int max = -aux_pos->max_literal;
	    HASH_FIND(hmax,positive,&max,sizeof(int),pos2);
	    if (pos2 != NULL) {
	      clauseslist *aux_candidate;
	      for (aux_candidate = pos2->list; aux_candidate != NULL; aux_candidate = aux_candidate->next) {
		tclause *aux = get_clause(aux_candidate->clause_number);
		tclause *resolvent = resolve_clauses_gen2(aux_neg,aux_pos,aux);
		if (resolvent != NULL) 
		  insert_clause_set(&resolvent,LITERAL,SOS);
	      }
	    }
	    poslist = poslist->next;
	  }
	}
      }
    }
  }
}

void do_gen2 (hml_mclauses *set) {
  hml_mclauses *aux = set;
  hagentclauses *hag = NULL;
  htypeclauses *htc = NULL;
  
  for (aux=set; aux != NULL; aux=aux->hml.next) {
    for (hag=aux->id; hag != NULL; hag=hag->hid.next) {
      htc=hag->type;
      hmaxlitclauses *positive = NULL, *negative = NULL;
      if (htc->type == MPOSITIVE) {
	positive = htc->lit;
	htc = htc->htype.next;
	if (htc != NULL)
	  negative = htc->lit;
      }
      else {
	negative = htc->lit;
	htc = htc->htype.next;
	if (htc != NULL)
	  positive = htc->lit;
      }
      if (positive != NULL && negative != NULL)
	resolve_sets_gen2(negative,positive);
    }
  }
}
