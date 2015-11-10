#ifndef ACTION_TABLE_HPP
#define ACTION_TABLE_HPP

/* Or Prodicter Table for a LR1 parser.
 *
 * The ActionTable contains the minimum used to determain what the next
 * operation in parsing a CFG is, with some extras for error checking, while
 * the ActionTableGenerator contains to build a ActionTable from the
 * definition of the GFG, or produce information on why one cannot be built.
 */

class ActionTableGenerator;

// The basic action table contains only the data used in the final calculation
class ActionTable
{
private:
protected:
  std::map<StateT, std::map<SymbolT,/*? SROp ?*/> > data;
public:
  ActionTable ();
  ActionTable (ActionTableGenerator const &);

  virtual ~ActionTable ();

  ActionTable & operator= (ActionTableAdv const &);

  bool hasOp (StateT, SymbolT) const;
  /*SROp*/ lookUp (StateT, SymbolT) const;

  void setOp (StateT, SymbolT, /*SROp*/);
  void delOp (StateT, SymbolT);
};

/* The ActionTableGenerator creates ActionTables for a given context-free
 * grammer. There maybe be some options to resolve problems.
 */
class ActionTableGenerator
{
private:
  std::map<StateT, std::map<SymbolT,std::vector</*? SROp ?*/> > > data;

  CFGrammer const grammer;

  std::map<Rule, bool> nullable;
  std::map<Rule, std::set<SymbolT> > follow;
  std::map<Rule, std::set<SymbolT> > first;
protected:
public:
  ActionTableGenerator (CFGrammer);
  /* Create a basic ActionTableGenerator for the given CFG.
   * Params: The definition of the context-free grammer.
   */

  virtual ~ActionTableGenerator ();

  bool isNullable (Rule);
  /*
   */
  std::set<SymbolT> followSet (Rule);
  /*
   */
  std::set<SymbolT> firstSet (Rule);
  /*
   */

  ActionTable generate ();
  /* Create and return a new ActionTable from this generator.
   * Except: Throws execption if an ActionTable cannot be created.
   */

  bool canGenerate ();
  /* Check to see if the instance can currantly generate an ActionTable.
   * Return: True if a table can be generated, false otherwise.
   */
  void printProblems (std::ostream &);
  /* Print the problems that prevent a ActionTable from being generated.
   * Params: Stream to print to.
   * Effect: Characters are sent to stream.
   * Format:
   */
};

#endif//ACTION_TABLE_HPP
