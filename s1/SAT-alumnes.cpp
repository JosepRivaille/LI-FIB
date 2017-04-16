#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
using namespace std;

/****************************
* CONSTANTS AND DEFINITIONS *
****************************/
#define UNDEF 0
#define TRUE 1
#define FALSE -1

const int ALL_DEFINED = 0;
const int DECISION_MARK = 0;

typedef vector<int> Clause;
typedef vector<Clause*> Clause_Appear;

/******************************
* DATA STRUCTURES AND GLOBALS *
******************************/
uint num_vars;
vector<int> model;

uint num_clauses;
vector<Clause> clauses;

uint index_of_next_lit_to_propagate;
uint decision_level;

vector<int> model_stack;

// Literal lit appears in clauses x1 .. xn -> v[lit] = vector<int>
vector<Clause_Appear> clauses_where_appear_positive;
vector<Clause_Appear> clauses_where_appear_negative;

vector<int> heuristic_order;

// Statistics
chrono::time_point<chrono::system_clock> t_start, t_end;
unsigned long propagations_count, decisions_count;

/**********
* HELPERS *
**********/
inline int current_value_in_model(int lit) {
  uint mask = -(lit < 0);
  uint abs_lit = (lit ^ mask) - mask;
  int value_in_model = model[abs_lit];
  return (value_in_model ^ mask) - mask;
}

inline void set_literal_to_true(int lit) {
  uint mask = -(lit < 0);
  uint abs_lit = (lit ^ mask) - mask;
  model[abs_lit] = mask + (mask == 0);
  model_stack.push_back(lit);
}

inline void check_model(){
  for (uint i = 0; i < num_clauses; ++i){
    bool some_true = false;
    for (int j = 0; not some_true and j < clauses[i].size(); ++j) {
      some_true = (current_value_in_model(clauses[i][j]) == TRUE);
    } 
    if (not some_true) {
      cout << "Error in model, clause is not satisfied: ";
      for (int j = 0; j < clauses[i].size(); ++j) {
        cout << clauses[i][j] << " ";
      } cout << endl;
      exit(1);
    }
  }
}

inline void print_and_finish(bool sat) {
  t_end = chrono::system_clock::now();
  chrono::duration<double> elapsed_seconds = t_end - t_start;
  string sat_insat = sat ? "SATISFIABLE" : "UNSATISFIABLE";
  cout << "=============[ Problem Statistics ]==============" << endl;
  cout << "|  Number of variables: " << num_vars << endl;
  cout << "|  Number of clauses: " << num_clauses << endl;
  cout << "|  SAT/INSAT: " << sat_insat << endl;
  cout << "|  Time: " << elapsed_seconds.count() << endl;
  cout << "|  Decisions: " << decisions_count << endl;
  cout << "|  Propagations: " << propagations_count << endl;
  cout << "=================================================" << endl;
  exit(sat ? 20 : 10);
}

inline void initial_clauses() {
  for (uint i = 0; i < num_clauses; ++i) {
    if (clauses[i].size() == 1) {
      int lit = clauses[i][0];
      int val = current_value_in_model(lit);
      if (val == FALSE) print_and_finish(false);
      else if (val == UNDEF) set_literal_to_true(lit);
    }
  }
}

/******************
* INITIALIZATIONS *
******************/
inline void read_clauses() {
  // Skip comments
  char c = cin.get();
  while (c == 'c') {
    while (c != '\n') c = cin.get(); 
    c = cin.get();
  }
  string aux;
  // Read "cnf numVars numClauses"
  cin >> aux >> num_vars >> num_clauses;
}

inline void init_data_structures() {
  clauses.resize(num_clauses);
  model.resize(num_vars + 1, UNDEF);
  heuristic_order.resize(num_vars + 1, UNDEF);
  clauses_where_appear_positive.resize(num_vars + 1);
  clauses_where_appear_negative.resize(num_vars + 1);
}

inline void fill_data_structures() {
  for (uint i = 0; i < num_clauses; ++i) {
    int lit;
    while (cin >> lit and lit != 0) {
      lit > 0
        ? clauses_where_appear_positive[lit].push_back(&clauses[i])
        : clauses_where_appear_negative[-lit].push_back(&clauses[i]);
      clauses[i].push_back(lit);
      ++heuristic_order[abs(lit)];
    }
  }
  index_of_next_lit_to_propagate = decision_level = decisions_count = propagations_count = 0;
}

inline void init_sat() {
  t_start = chrono::system_clock::now();
  read_clauses();
  init_data_structures();
  fill_data_structures();
}

/*****************
* SOLVER METHODS *
*****************/
inline int get_next_decision_literal_heuristic() {
  uint lit;
  int max_priority = -1;
  for (uint i = 0; i < num_vars; ++i) {
    if (model[i] == UNDEF and heuristic_order[i] > max_priority) {
      max_priority = heuristic_order[i];
      lit = i;
    }
  }
  return max_priority == -1 ? ALL_DEFINED : lit;
}

inline bool propagate() {
  int propagated_literal = model_stack[index_of_next_lit_to_propagate];
  uint propagated_variable = abs(propagated_literal);
  vector<Clause*>& clauses_affected = propagated_literal > 0
    ? clauses_where_appear_negative[propagated_variable]
    : clauses_where_appear_positive[propagated_variable];

  for (uint i = 0; i < clauses_affected.size(); ++i) {
    Clause& clause = *clauses_affected[i];

    uint num_undefs = 0;
    int last_lit_undef = 0;
    bool some_lit_true = false;

    for (uint j = 0; not some_lit_true and j < clause.size(); ++j) {
      int lit = clause[j];
      int val = current_value_in_model(lit);
      num_undefs += (val == UNDEF);
      last_lit_undef = lit - ((lit - last_lit_undef) & val);
      some_lit_true = (val == TRUE);
    }

    if (not some_lit_true and num_undefs <= 1) {
      if (num_undefs == 1) {
        set_literal_to_true(last_lit_undef);
      } else {
        heuristic_order[propagated_variable]++;
        return true;
      }
    }
  }
  return false;
}

inline bool propagate_gives_conflict() {
  while (index_of_next_lit_to_propagate < model_stack.size()) {
    if (propagate()) return true;
    ++propagations_count;
    ++index_of_next_lit_to_propagate;
  }
  return false;
}

inline void decide_positive_literal(int lit) {
  model_stack.push_back(DECISION_MARK);
  ++decisions_count;
  ++index_of_next_lit_to_propagate;
  ++decision_level;
  set_literal_to_true(lit);
}

inline void decide_negative_literal(int lit) {
  model_stack.pop_back();
  index_of_next_lit_to_propagate = model_stack.size();
  --decision_level;
  set_literal_to_true(-lit);
}

void backtrack() {
  uint i = model_stack.size() - 1;
  int lit = 0;
  while (model_stack[i] != DECISION_MARK) {
    lit = model_stack[i--];
    model[abs(lit)] = UNDEF;
    model_stack.pop_back();
  }
  decide_negative_literal(lit);
}

/*******
* MAIN *
********/
int main() { 
  init_sat();
  initial_clauses();

  // DPLL algorithm (Davis–Putnam–Logemann–Loveland)
  while (true) {
    while (propagate_gives_conflict()) {
      if (decision_level == 0) print_and_finish(false);
      backtrack();
    }

    int decision_lit = get_next_decision_literal_heuristic();
    if (decision_lit == ALL_DEFINED) {
      check_model();
      print_and_finish(true);
    }

    decide_positive_literal(decision_lit);
  }
}
