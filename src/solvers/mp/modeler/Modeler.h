/*
 * Modeler.h:
 *    Tools to create a SCIP problem.
 *    In general, when you want to create a new scip object,
 *    you have to give a pointer to the pointer of the object.
 *
 *  Created on: 2015-02-23
 *      Author: legraina
 */

#ifndef SRC_MODELER_H_
#define SRC_MODELER_H_

/* standard library includes */
#include <cfloat>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <typeinfo>
#include "solvers/Solver.h"

#include "tools/MyTools.h"

/*
 * Var types
 */
enum VarType {VARTYPE_CONTINUOUS, VARTYPE_INTEGER, VARTYPE_BINARY};

/*
 * Rule Search Strategy
 */

enum SearchStrategy { BestFirstSearch, BreadthFirstSearch, DepthFirstSearch, HighestGapFirst };

/*
 * My Modeling objects
 * If the object is added to the vector objects_ of the Modeler, the modeler will also delete it at the end.
 */
struct MyObject {
	MyObject(const char* name):id_(s_count) {
		++s_count;
		char* name2 = new char[255];
		strncpy(name2, name, 255);
		name_ = name2;
		// std::cout << "LP : " << name2 << std::endl;
	}
	MyObject(const MyObject& myObject):id_(myObject.id_) {
		char* name2 = new char[255];
		strncpy(name2, myObject.name_, 255);
		name_ = name2;
		// std::cout << "LP : " << name2 << std::endl;
	}
  virtual ~MyObject(){//cout << "LP DEL : " << name_ << std::endl;
	  delete[] name_;}
	//count object
	static unsigned int s_count;
	//for the map rotations_
	int operator < (const MyObject& m) const { return this->id_ < m.id_; }

	const char* name_;
private:
	const unsigned int id_;
};

static const std::vector<double> DEFAULT_PATTERN;

struct MyVar: public MyObject{
	MyVar(const char* name, int index, double cost, VarType type, double lb, double ub, const std::vector<double>& pattern = DEFAULT_PATTERN):
		MyObject(name), index_(index), type_(type), cost_(cost), lb_(lb), ub_(ub), pattern_(pattern),
		iteration_creation_(0), active_count_(0), last_active_(0)
  {//		cout << "LP VAR: " << name << "    " << index << std::endl;
 }

	MyVar(const MyVar& var) :
		MyObject(var), index_(var.index_), type_(var.type_), cost_(var.cost_), lb_(var.lb_), ub_(var.ub_), pattern_(var.pattern_),
		iteration_creation_(var.iteration_creation_), active_count_(var.active_count_), last_active_(var.last_active_)
	{ }

	virtual ~MyVar(){ }

	int getIndex() const { return index_; }

	virtual VarType getVarType() {return type_;}

	double getCost() { return cost_; }

	virtual double getLB() const { return lb_; }

	virtual double getUB() const { return ub_; }

	void setCost(double cost) { cost_=cost; }

	virtual void setLB(double lb) { lb_=lb; }

	virtual void setUB(double ub) { ub_=ub; }

	virtual void setVarType(VarType newType) {type_=newType;}

	bool is_integer() { return type_ != VARTYPE_CONTINUOUS; }

	const std::vector<double>& getPattern() { return pattern_; }

	int getIterationCreation() { return iteration_creation_; }

	int getActiveCount() { return active_count_; }

	int getLastActive() { return last_active_; }

	void addActiveIteration(int iteration) {
		if(last_active_!=iteration){
			last_active_=iteration;
			++active_count_;
		}
	}

	void setActiveCounters(int iteration_creation, int active_count, int last_active){
		iteration_creation_ = iteration_creation;
		active_count_ = active_count;
		last_active_ = last_active;
	}

	// get the first day of the rotation corresponding to the column
	virtual int getFirstDay() const {
		return (int) pattern_[1];
	}

	// get the id of the nurse in charge of the rotation
	virtual int getNurseId() const {
		return (int) pattern_[0];
	}


protected:
	int index_; //count var
	VarType type_; //type of the variable
	double cost_; //cost of the variable
	double lb_; //lower bound
	double ub_; //upper bound
	const std::vector<double> pattern_;//pattern for a column
	int iteration_creation_; //save the iteration number at the creation of the variable
	int active_count_; //count the number of times where the variable is present in the solution
	int last_active_; //save the iteration number of the last activity
};

static const std::vector<MyVar*> EMPTY_VARS;

struct MyCons: public MyObject{
	MyCons(const char* name, int index, double lhs, double rhs):
		MyObject(name), index_(index), lhs_(lhs), rhs_(rhs)
  { //	cout << "LP CONS: " << name << std::endl;
	}

	MyCons(const MyCons& cons) :
		MyObject(cons.name_), index_(cons.index_), lhs_(cons.lhs_), rhs_(cons.rhs_)
	{ }

	virtual ~MyCons(){ }

	/*
	 * Getters
	 */

	double getLhs() { return lhs_; }

	double getRhs() { return rhs_; }

	void setLhs(double lhs) { lhs_=lhs; }

	void setRhs(double rhs) { rhs_=rhs; }

	int getIndex(){ return index_; }

protected:
	int index_;
	double lhs_; //left hand side == lower bound
	double rhs_; //rihgt hand side == upper bound
};

/*
 * My pricer
 */
struct MyPricer{
	MyPricer(const char* name): name_(name){ }
	virtual ~MyPricer() { }

	//name of the pricer handler
	//
	const char* name_;

	/* perform pricing */
	//return true if optimal
	virtual std::vector<MyVar*> pricing(double bound=0, bool before_fathom = true)=0;

	// set pricer parameters
	virtual void initPricerParameters(SolverParam parameters) {}

	   // METHODS - Forbidden shifts, nurses, starting days, etc.
   //
   // !!! WARNING !!! : SOME METHODS ARE NOT YET IMPLEMENTED IN THE SUBPROBLEM (ALTHOUGH THE NECESSARY STRUCTURES MAY
   //                   ALREADY BE THERE !!!
   //
   // Shifts
   virtual void forbidShift(int k, int s)=0;
//   virtual void forbidShifts(set<pair<int,int> > shifts)=0;
//   virtual void authorizeShift(int k, int s)=0;
//   virtual void clearForbiddenShifts()=0;
   // Nurses
   virtual void forbidNurse(int nurseId)=0;
//   virtual void forbidNurses(set<int,int> nurses)=0;
   virtual void authorizeNurse(int nurseId)=0;
   virtual void clearForbiddenNurses()=0;
   // Starting days
   virtual void forbidStartingDay(int k)=0;
//   virtual void forbidStartingDays(set<int> days)=0;
   virtual void authorizeStartingDay(int k)=0;
   virtual void clearForbiddenStartingDays()=0;
   // Ending days
   virtual void forbidEndingDay(int k)=0;
//   virtual void forbidEndingDays(set<int> days)=0;
//   virtual void authorizeEndingDay(int k)=0;
//   virtual void clearForbiddenEndingDays()=0;
};

/* Exception to stop the solver */
struct FeasibleStop: public std::exception{
	FeasibleStop(std::string str){ std::cout << str << std::endl; }
};

struct InfeasibleStop: public std::exception{
	InfeasibleStop(std::string str){ std::cout << str << std::endl; }
};

struct OptimalStop: public std::exception{
	OptimalStop(std::string str){ std::cout << str << std::endl; }
};

struct TimeoutStop: public std::exception{
	TimeoutStop(std::string str){ std::cout << str << std::endl; }
};

/* Structures to store a branching candidate and its potential children */
struct MyBranchingNode{
	friend struct MyBranchingCandidate;
	MyBranchingNode() {}

	const std::vector<double>& getLb() const {
		return LB;
	}

	void setLb(int index, double lb) {
		LB[index] = lb;
	}

	const std::vector<double>& getLhs() const {
		return lhs;
	}

	void setLhs(int index, double  lhs) {
		this->lhs[index] = lhs;
	}

	const std::vector<double>& getRhs() const {
		return rhs;
	}

	void setRhs(int index, double rhs) {
		this->rhs[index] = rhs;
	}

	const std::vector<double>& getUb() const {
		return UB;
	}

	void setUb(int index, double ub) {
		UB[index] = ub;
	}

protected:
 	std::vector<double> LB, UB;
 	std::vector<double> rhs, lhs;
};

struct MyBranchingCandidate{
	MyBranchingCandidate() {}

	int createNewChild(){
		children_.push_back(MyBranchingNode());
		initialize(children_.back());
		return children_.size()-1;
	}

	MyBranchingNode& getChild(int index){
		return children_[index];
	}

	void swapChildren(int index1, int index2){
		MyBranchingNode& copyNode = children_[index1];
		children_[index1] = children_[index2];
		children_[index2] = copyNode;
	}

	void initialize(MyBranchingNode& node){
		for(MyVar* var: branchingVars_){
			node.LB.push_back(var->getLB());
			node.UB.push_back(var->getUB());
		}

		for(MyCons* cons: branchingCons_){
			node.lhs.push_back(cons->getLhs());
			node.rhs.push_back(cons->getRhs());
		}
	}

	int addBranchingVar(MyVar* var){
		branchingVars_.push_back(var);
		for(MyBranchingNode& child: children_){
			child.LB.push_back(var->getLB());
			child.UB.push_back(var->getUB());
		}
		return branchingVars_.size()-1;
	}

	int addNewBranchingVar(MyVar* var){
		newBranchingVars_.push_back(var);
		return addBranchingVar(var);
	}

	int addBranchingCons(MyCons* cons){
		branchingCons_.push_back(cons);
		for(MyBranchingNode& child: children_){
			child.lhs.push_back(cons->getLhs());
			child.rhs.push_back(cons->getRhs());
		}
		return branchingCons_.size()-1;
	}

	int addNewBranchingCons(MyCons* cons){
		newBranchingCons_.push_back(cons);
		return addBranchingCons(cons);
	}

	const std::vector<MyCons*>& getBranchingCons() const {
		return branchingCons_;
	}

	const std::vector<MyVar*>& getBranchingVars() const {
		return branchingVars_;
	}

	const std::vector<MyBranchingNode>& getChildren() const {
		return children_;
	}

	const std::vector<MyCons*>& getNewBranchingCons() const {
		return newBranchingCons_;
	}

	const std::vector<MyVar*>& getNewBranchingVars() const {
		return newBranchingVars_;
	}

protected:
 	std::vector<MyVar*> branchingVars_, newBranchingVars_;
 	std::vector<MyCons*> branchingCons_, newBranchingCons_;
 	std::vector<MyBranchingNode> children_;
};

/*
 * My branching rule
 */
struct MyBranchingRule{
	MyBranchingRule(const char* name): name_(name), searchStrategy_(BestFirstSearch) { }
	virtual ~MyBranchingRule() { }

	//name of the branching rule handler
	//
	const char* name_;

	/* compute logical fixing decisions */
	virtual bool column_candidates(MyBranchingCandidate& candidate)=0; // add the new children to the candidate (just column node). return true if a child is created

	/* compute branching decisions */
	virtual bool branching_candidates(MyBranchingCandidate& candidate)=0; // add the new children to the candidate (other nodes). return true if a child is created

	void set_search_strategy(SearchStrategy searchStrategy){ searchStrategy_ = searchStrategy; }

protected:
	SearchStrategy searchStrategy_;
};

/* Represents the nodes of the branching tree */
struct MyNode{

	MyNode(): index_(0), pParent_(0), bestLB_(LARGE_SCORE), highestGap_(0), bestLagLB_(-LARGE_SCORE), lastLagLB_(-LARGE_SCORE) {}
	MyNode(int index, MyNode* pParent):
		index_(index), pParent_(pParent), bestLB_(pParent->bestLB_), highestGap_(0), bestLagLB_(pParent->bestLagLB_), lastLagLB_(pParent->bestLB_) {}
	virtual ~MyNode() {}

	const int index_;

	//parent
	MyNode* pParent_;

	inline void pushBackChild(MyNode* child){
		children_.push_back(child);
	}

	inline void updateBestLB(double newLB){
		bestLB_ = newLB;
		//if not root
		if(pParent_){
			double gap = (bestLB_ - pParent_->bestLB_)/pParent_->bestLB_;
			if(gap > pParent_->highestGap_) pParent_->highestGap_ = gap;
		}
	}

	inline double getHighestGap(){
		//if root, it is the best
		if(!pParent_)
			return LARGE_SCORE;

		//otherwise compare the current gap
		return pParent_->highestGap_ ;
	}

	// The quality of a node is used to sort the candidate list (siblings of the branching tree)
	// They are sorted in ascending order
	inline double getQuality(){
		//if root, does not apply
		if(!pParent_)
			return LARGE_SCORE;
		//otherwise return parent's best LB
		return pParent_->getBestLB() ;
	}

	inline double getBestLB() { return bestLB_; }

	// STAB
	inline double getBestLagLB() { return bestLagLB_; }
	inline void setBestLagLB(double bestLagLB) {bestLagLB_ = bestLagLB;}

	// LAGLB
	inline double getLastLagLB() {return lastLagLB_;}
	inline void setLastLagLB(double lb) {lastLagLB_ = lb;}


	inline void setDepth(int depth) { depth_ = depth; }

	inline double getDepth() { return depth_; }

	virtual std::string write() {
    std::stringstream out;
		out << "MyNode: (depth=" << depth_ << ",LB=" << bestLB_ << ")";
		return out.str();
	}

protected:
	double bestLB_;
	//highest gap between the bestLB_ and the computed bestLB_ of the children
	double highestGap_;
	int depth_;

	// STAB : best lagrangian bound obtained at this node
	double bestLagLB_;

	// LAGLB: last Lagrangian lower bound in the column generation
	double lastLagLB_;

	//children
 	std::vector<MyNode*> children_;
};

struct MyTree {
	MyTree(): tree_size_(1), nb_nodes_last_incumbent_(-2), diveDepth_(0), diveLength_(LARGE_SCORE), min_depth_(0),
		  nb_nodes_since_dive_(-2),currentNode_(0),
		  best_lb_in_root(LARGE_SCORE), best_lb(LARGE_SCORE), best_ub(LARGE_SCORE) {}
	virtual ~MyTree() {}

	inline void setRootLB(double bestLBRoot){ best_lb_in_root = bestLBRoot; }

	inline double getRootLB(){ return best_lb_in_root; }

	inline void setCurrentNode(MyNode* currentNode) {
		currentNode_ = currentNode;
		--tree_size_;
		//one more node without new incumbent and since last dive
		++nb_nodes_last_incumbent_;
		++nb_nodes_since_dive_;
		//we start a new dive
		if(diveDepth_ > 0 && diveLength_ == LARGE_SCORE)
			diveLength_ = 1+diveDepth_;
	}

	/* clear tree */
	void clear(){
		for(MyNode* node: tree_)
			delete node;
		tree_.clear();
		activeTreeMapping_.clear();
	}

	inline std::string writeCurrentNode() {
		if(currentNode_) return currentNode_->write();
		else return "";
	}
	inline std::vector<MyNode*> addToMapping(const int nbLeaves, const int diveDepth) {
		const int size = tree_.size();
	 	std::vector<MyNode*> leaves(nbLeaves);
		for(int i=0; i<nbLeaves; ++i){
			leaves[i] = tree_[size - nbLeaves + i];
			leaves[i]->setDepth(diveDepth + 1);
		}
		activeTreeMapping_.insert(std::pair<MyNode*, std::vector<MyNode*> >(currentNode_, leaves));
		//finally update the current node for the moment.
		//Will not change for the first node as diving
		currentNode_ = leaves[0];
		tree_size_ += nbLeaves -1; //-1 as diving
		//one more node without new incumbent
		++nb_nodes_last_incumbent_;
		++nb_nodes_since_dive_;
		//we dive
		diveDepth_ = diveDepth;

		return leaves;
	}

	//when diving with just one node, the previous methos is not called, so we update the right arguments
	inline void updateDive(){
		currentNode_ = tree_.back();
		//one more node without new incumbent
		++nb_nodes_last_incumbent_;
		++nb_nodes_since_dive_;
		++diveDepth_;
	}

	inline void eraseCurrentSibblings(){
		activeTreeMapping_.erase(currentNode_->pParent_);
		//update min_depth_
		min_depth_ = LARGE_SCORE;
		for(std::pair<MyNode*,std::vector<MyNode*>> p: activeTreeMapping_)
			if(p.first->getDepth() < min_depth_) min_depth_ = p.first->getDepth();
	}

	inline MyNode* getNode(const int nodeIndex) {
		return tree_[nodeIndex];
	}

	inline double getBestLB(){
		best_lb = currentNode_->getBestLB();
		for(std::pair<MyNode*, std::vector<MyNode*> > p: activeTreeMapping_)
			for(MyNode* node: p.second)
				if(best_lb > node->getBestLB())
					best_lb = node->getBestLB();
		if(best_lb == LARGE_SCORE)
			return LARGE_SCORE;
		return best_lb;
	}

	inline double get_best_lb() {return best_lb;}

	inline void setBestUB(double ub) {
		/* reinitialize nb_nodes_last_incumbent_ */
		if(ub + 1 < best_ub) nb_nodes_last_incumbent_=0;
		if(ub < best_ub) best_ub = ub;
	}

	inline double getBestUB() { return best_ub; }

	inline double getCurrentLB(){return currentNode_->getBestLB();}

	//Reset and clear solving parameters
	virtual void reset() {
		clear();
		best_ub = LARGE_SCORE;
		currentNode_=0;
		tree_size_ = 1;
		nb_nodes_last_incumbent_=0;
		nb_nodes_since_dive_=0;
		diveDepth_=0;
		diveLength_ = LARGE_SCORE;
		best_lb_in_root = LARGE_SCORE;
		best_lb = LARGE_SCORE;
	}

	inline int getTreeSize(){ return tree_size_; }

	inline int getNbNodesSinceLastIncumbent() { return nb_nodes_last_incumbent_; }

	inline void resetNbNodesSinceLastDive(){ nb_nodes_since_dive_ = 0; }

	inline int getDiveLength() { return diveLength_; }

	inline int getNbDives() { return nb_nodes_since_dive_ / diveLength_; } //nb_nodes_last_incumbent_

	inline void updateNodeLB(double lb){
		if(best_lb_in_root > lb)
			best_lb_in_root = lb;
		currentNode_->updateBestLB(lb);
		updateStats(currentNode_);
	}

	// STAB: getter and setter for lagrangian bound
	inline double getNodeBestLagLB(){return currentNode_->getBestLagLB();}
	inline double getNodeLastLagLB(){return currentNode_->getLastLagLB();}
	inline bool updateNodeLagLB(double lb){
		currentNode_->setLastLagLB(lb);
		if (lb > currentNode_->getBestLagLB()+EPSILON) {
			currentNode_->setBestLagLB(lb);
			return true;
		}
		return false;
	}

	double getObjective(){ return best_ub; }

	double getRelaxedObjective() { return best_lb_in_root; }

	inline void pushBackNewNode(){
		pushBackNode(new MyNode);
	}

	inline void pushBackNode(MyNode* node){
		tree_.push_back(node);
		//just for pushing root
		if(currentNode_)
			currentNode_->pushBackChild(node);
	}

	MyNode* getCurrentNode() { return currentNode_; }

	virtual bool is_columns_node() { return false; }

	virtual bool continueDiving(){return false;}

	inline void addCurrentNodeToStack(){
		//currentNode_ is finally
		tree_size_ ++;
		--nb_nodes_last_incumbent_;
		--nb_nodes_since_dive_;
	}

	virtual void addForbiddenShifts(LiveNurse* pNurse, std::set<std::pair<int,int> >& forbidenShifts) { return; }

	/*
	 * Stats
	 */

	virtual void updateStats(MyNode* node) {}

	virtual std::string writeBranchStats() { return ""; }

	void printStats() { std::cout << writeBranchStats(); }

protected:
	//mapping between the Siblings and MyNode*
	//a sibblings contains a list of all its leaves MyNode
  std::map<MyNode*, std::vector<MyNode*>> activeTreeMapping_;
	//branching tree
 	std::vector<MyNode*> tree_;
	//tree size, number of nodes since last incumbent, depth of the current dive, length of a dive
	int tree_size_, nb_nodes_last_incumbent_, diveDepth_, diveLength_, min_depth_, nb_nodes_since_dive_;
	//current node
	MyNode* currentNode_;

	//best lb in root and current best lb
	double best_lb_in_root, best_lb;
	//best current upper bound found
	double best_ub;
};



//-----------------------------------------------------------------------------
//
//  C l a s s   M o d e l e r
//
// Generic class of models
//
//
//-----------------------------------------------------------------------------



class Modeler {
public:

	Modeler(): pPricer_(0), pBranchingRule_(0), pTree_(new MyTree()) { }

	virtual ~Modeler(){
		for(MyObject* object: objects_)
			delete object;
		objects_.clear();
	}

	//solve the model
	virtual int solve(bool relaxation = false)=0;

	//Reset and clear solving parameters
	virtual void reset(bool rollingHorizon=false) { pTree_->reset(); }

	//Add a pricer
	virtual int addObjPricer(MyPricer* pPricer){
		pPricer_ = pPricer;
		return 1;
	}

	//Add a branching rule
	virtual int addBranchingRule(MyBranchingRule* pBranchingRule){
		pBranchingRule_ = pBranchingRule;
		pBranchingRule_->set_search_strategy(searchStrategy_);
		return 1;
	}

	//Add a tree
	virtual int addTree(MyTree* pTree){
		if(pTree_) delete pTree_;
		pTree_ = pTree;
		return 1;
	}

	virtual void addForbiddenShifts(LiveNurse* pNurse, std::set<std::pair<int,int> >& forbidenShifts) {
		pTree_->addForbiddenShifts(pNurse, forbidenShifts);
	}


	/*
	 * Class methods for pricer and branching rule
	 */

	//return true if optimal
	inline std::vector<MyVar*> pricing(double bound=0, bool before_fathom = true){
		if(pPricer_)
			return pPricer_->pricing(bound, before_fathom);
		return EMPTY_VARS;
	}

	inline bool branching_candidates(MyBranchingCandidate& candidate){
		if(pBranchingRule_)
			return pBranchingRule_->branching_candidates(candidate);
		return false;
	}

	//remove all bad candidates from fixingCandidates
	inline bool column_candidates(MyBranchingCandidate& candidate){
		if(pBranchingRule_)
			return pBranchingRule_->column_candidates(candidate);
		return false;
	}
	//Set search strategy
	inline void set_search_strategy(SearchStrategy searchStrategy){
		if(pBranchingRule_)
			pBranchingRule_->set_search_strategy(searchStrategy);
	}

	/*
	 * Create variable:
	 *    var is a pointer to the pointer of the variable
	 *    var_name is the name of the variable
	 *    lhs, rhs are the lower and upper bound of the variable
	 *    vartype is the type of the variable: VARTYPE_CONTINUOUS, VARTYPE_INTEGER, VARTYPE_BINARY
	 */

protected:
	virtual int createVar(MyVar** var, const char* var_name, int index, double objCoeff,
			double lb, double ub, VarType vartype, const std::vector<double>& pattern, double score)=0;

public:
	inline void createPositiveVar(MyVar** var, const char* var_name, double objCoeff, const std::vector<double>& pattern = DEFAULT_PATTERN, double score = 0, double ub = DBL_MAX){
		ub = (ub==DBL_MAX)? infinity_:ub;
		createVar(var, var_name, ++var_count, objCoeff, 0.0, ub, VARTYPE_CONTINUOUS, pattern, score);
	}

	inline void createIntVar(MyVar** var, const char* var_name, double objCoeff, const std::vector<double>& pattern = DEFAULT_PATTERN, double score = 0, double ub = DBL_MAX){
		ub = (ub==DBL_MAX)? infinity_:ub;
		createVar(var, var_name, ++var_count, objCoeff, 0, ub, VARTYPE_INTEGER, pattern, score);
		integerCoreVars_.push_back(*var);
	}

	inline void createBinaryVar(MyVar** var, const char* var_name, double objCoeff, const std::vector<double>& pattern = DEFAULT_PATTERN, double score = 0){
		createVar(var, var_name, ++var_count, objCoeff, 0.0, 1.0, VARTYPE_BINARY, pattern, score);
		binaryCoreVars_.push_back(*var);
	}

protected:
	virtual int createColumnVar(MyVar** var, const char* var_name, int index, double objCoeff, const std::vector<double>& pattern, double dualObj,
			double lb, double ub, VarType vartype, double score)=0;

public:
	inline void createPositiveColumnVar(MyVar** var, const char* var_name, double objCoeff, const std::vector<double>& pattern, double dualObj = 99999, double score = 0, double ub = DBL_MAX){
		ub = (ub==DBL_MAX)? infinity_:ub;
		createColumnVar(var, var_name, ++var_count, objCoeff, pattern, dualObj, 0.0, ub, VARTYPE_CONTINUOUS, score);
	}

	inline void createIntColumnVar(MyVar** var, const char* var_name, double objCoeff, const std::vector<double>& pattern, double dualObj = 99999, double score = 0, double ub = DBL_MAX){
		ub = (ub==DBL_MAX)? infinity_:ub;
		createColumnVar(var, var_name, ++var_count, objCoeff, pattern, dualObj, 0, ub, VARTYPE_INTEGER, score);
	}

	inline void createBinaryColumnVar(MyVar** var, const char* var_name, double objCoeff, const std::vector<double>& pattern, double dualObj = 99999, double score = 0){
		createColumnVar(var, var_name, ++var_count, objCoeff, pattern, dualObj, 0.0, 1.0, VARTYPE_BINARY, score);
	}

	/*
	 * Create linear constraint:
	 *    con is a pointer to the pointer of the constraint
	 *    con_name is the name of the constraint
	 *    lhs, rhs are the lower and upper bound of the constraint
	 *    nonZeroVars is the number of non-zero coefficients to add to the constraint
	 *    vars is an array of pointers to the variables to add to the constraints (with non-zero coefficient)
	 *    coeffs is the array of coefficient to add to the constraints
	 */

protected:
	virtual int createConsLinear(MyCons** cons, const char* con_name, int index, double lhs, double rhs,
		 	std::vector<MyVar*> vars = {}, std::vector<double> coeffs = {})=0;

public:
	//Add a lower or equal constraint
	inline void createLEConsLinear(MyCons** cons, const char* con_name, double rhs,
		 	std::vector<MyVar*> vars = {}, std::vector<double> coeffs = {}){
		createConsLinear(cons, con_name, ++cons_count, -infinity_, rhs, vars, coeffs);
	}

	//Add a greater or equal constraint
	inline void createGEConsLinear(MyCons** cons, const char* con_name, double lhs,
		 	std::vector<MyVar*> vars = {}, std::vector<double> coeffs = {}){
		createConsLinear(cons, con_name, ++cons_count, lhs, infinity_, vars, coeffs);
	}

	//Add an equality constraint
	inline void createEQConsLinear(MyCons** cons, const char* con_name, double eq,
		 	std::vector<MyVar*> vars = {}, std::vector<double> coeffs = {}){
		createConsLinear(cons, con_name, ++cons_count, eq, eq, vars, coeffs);
	}

	//Add final linear constraints
protected:
	virtual int createFinalConsLinear(MyCons** cons, const char* con_name, int index, double lhs, double rhs,
		 	std::vector<MyVar*> vars = {}, std::vector<double> coeffs = {})=0;

public:
	inline void createFinalLEConsLinear(MyCons** cons, const char* con_name, double rhs,
		 	std::vector<MyVar*> vars = {}, std::vector<double> coeffs = {}){
		createFinalConsLinear(cons, con_name, ++cons_count, -infinity_, rhs, vars, coeffs);
	}

	inline void createFinalGEConsLinear(MyCons** cons, const char* con_name, double lhs,
		 	std::vector<MyVar*> vars = {}, std::vector<double> coeffs = {}){
		createFinalConsLinear(cons, con_name, ++cons_count, lhs, infinity_, vars, coeffs);
	}

	inline void createFinalEQConsLinear(MyCons** cons, const char* con_name, double eq,
		 	std::vector<MyVar*> vars = {}, std::vector<double> coeffs = {}){
		createFinalConsLinear(cons, con_name, ++cons_count, eq, eq, vars, coeffs);
	}

	virtual void createCutLinear(MyCons** cons, const char* con_name, double lhs, double rhs,
		 	std::vector<MyVar*> vars = {}, std::vector<double> coeffs = {})=0;

	/*
	 * Add variables to constraints
	 */

	virtual int addCoefLinear(MyCons* cons, MyVar* var, double coeff, bool transformed=false)=0;

	/*
	 * Add new Column to the problem
	 */

	inline void createColumn(MyVar** var, const char* var_name, double objCoeff, const std::vector<double>& pattern, double dualObj,  VarType vartype,
		 	std::vector<MyCons*> cons = {}, std::vector<double> coeffs = {}, bool transformed = false, double score = 0){
		switch(vartype){
		case VARTYPE_BINARY:
			createBinaryColumnVar(var, var_name, objCoeff, pattern, dualObj, score);
			break;
		case VARTYPE_INTEGER:
			createIntColumnVar(var, var_name, objCoeff, pattern, dualObj, score);
			break;
		default:
			createPositiveColumnVar(var, var_name, objCoeff, pattern, dualObj, score);
			break;
		}

		for(unsigned int i=0; i<cons.size(); i++)
			addCoefLinear(cons[i], *var, coeffs[i], transformed);
	}

	inline void createPositiveColumn(MyVar** var, const char* var_name, double objCoeff, const std::vector<double>& pattern, double dualObj,
		 	std::vector<MyCons*> cons = {}, std::vector<double> coeffs = {}, bool transformed = false, double score = 0){
		createColumn(var, var_name, objCoeff, pattern, dualObj, VARTYPE_CONTINUOUS, cons, coeffs, transformed, score);
	}

	inline void createBinaryColumn(MyVar** var, const char* var_name, double objCoeff, const std::vector<double>& pattern, double dualObj,
		 	std::vector<MyCons*> cons = {}, std::vector<double> coeffs = {}, bool transformed = false, double score = 0){
		createColumn(var, var_name, objCoeff, pattern, dualObj, VARTYPE_BINARY, cons, coeffs, transformed, score);
	}

	inline void createIntColumn(MyVar** var, const char* var_name, double objCoeff, const std::vector<double>& pattern, double dualObj,
		 	std::vector<MyCons*> cons = {}, std::vector<double> coeffs = {}, bool transformed = false, double score = 0){
		createColumn(var, var_name, objCoeff, pattern, dualObj, VARTYPE_INTEGER, cons, coeffs, transformed, score);
	}

	/*
	 * get the primal values
	 */

	virtual bool isInteger(MyVar* var){
		double value = getVarValue(var);
		double fractionalPart = round(value) - value;
		if( abs(fractionalPart) < EPSILON )
			return true;
		return false;
	}

	virtual double getVarValue(MyVar* var)=0;

	//compute the total cost of a multiple vectors of MyObject* in the solution
	template<typename V> inline double getVarValue(std::vector<V>& vector){
		double value = 0 ;
		for(V& vect: vector)
			value += getVarValue(vect);
		return value;
	}

	inline std::vector<double> getVarValues(std::vector<MyVar*>& vars){
	 	std::vector<double> values(vars.size());
		for(unsigned int i=0; i<vars.size(); ++i)
			values[i] = getVarValue(vars[i]);
		return values;
	}

	/*
	 * Get the dual variables
	 */

	virtual double getDual(MyCons* cons, bool transformed = false)=0;

	inline std::vector<double> getDuals(std::vector<MyCons*>& cons, bool transformed = false){
	 	std::vector<double> dualValues(cons.size());
		for(unsigned int i=0; i<cons.size(); ++i)
			dualValues[i] = getDual(cons[i], transformed);
		return dualValues;
	}

	/*
	 * Get the reduced cost
	 */

	virtual double getReducedCost(MyVar* var)=0;

	/*
	 * Getters and setters
	 */

	//compute the total cost of MyObject* in the solution
	virtual double getTotalCost(MyVar* var, bool print = false)=0;

	//compute the total cost of a vector of MyObject* in the solution
	template<typename T>  inline double getTotalCost(std::map<MyVar*, T>& map0, bool print = false){
		double value = 0 ;
		for(std::pair<MyObject*, T>& var: map0)
			value += getTotalCost(var.first, print);
		return value;
	}

	//compute the total cost of a multiple vectors of MyObject* in the solution
	template<typename V> inline double getTotalCost(std::vector<V>& vector, bool print = false){
		double value = 0 ;
		for(V& vect: vector)
			value += getTotalCost(vect, print);
		return value;
	}

	/**************
	 * Parameters *
	 *************/
	virtual int setVerbosity(int v)=0;

	/**************
	 * Outputs *
	 *************/

	virtual int printStats() { pTree_->printStats(); return 1; };

	virtual int printBestSol()=0;

	virtual int writeProblem(std::string fileName)=0;

	virtual int writeLP(std::string fileName)=0;

	virtual void toString(MyObject* obj){ std::cout << obj->name_ << std::endl; }

	/**************
	 * Getters *
	 *************/

	template<typename M> M getModel(){
    std::string error = "This template has not been implemented.";
		Tools::throwError(error.c_str());
	}

	inline std::vector<MyVar*>& getBinaryCoreVars(){ return binaryCoreVars_; }

	inline std::vector<MyVar*>& getIntegerCoreVars(){ return integerCoreVars_; }

	inline std::vector<MyVar*>& getPositiveCoreVars(){ return positiveCoreVars_; }

	inline int getVerbosity() { return verbosity_; }

	inline virtual void setBestUB(double ub) { pTree_->setBestUB(ub); }

	inline virtual double getObjective(){ return pTree_->getBestUB(); }
	virtual double getObjective(int index) { return LARGE_SCORE; }


	inline virtual double getRelaxedObjective() { return pTree_->getRootLB(); }

	inline double getCurrentLB() { return pTree_->getCurrentLB(); }

	inline void updateNodeLB(double lb) { pTree_->updateNodeLB(lb); }

	// STAB
	inline double getNodeBestLagLB() {return pTree_->getNodeBestLagLB();}
	inline double getNodeLastLagLB() {return pTree_->getNodeLastLagLB();}
	inline bool updateNodeLagLB(double lb) {return pTree_->updateNodeLagLB(lb);}

	inline double getRootLB() { return pTree_->getRootLB(); }

	inline double getBestLB() { return pTree_->getBestLB(); }
	inline double get_best_lb() { return pTree_->get_best_lb(); }

	inline int getTreeSize() { return pTree_->getTreeSize(); }

	inline std::string writeCurrentNode() { return pTree_->writeCurrentNode(); }

	inline bool is_columns_node() { return pTree_->is_columns_node(); }

	inline void updateDive() { return pTree_->updateDive(); }

	inline bool continueDiving() { return pTree_->continueDiving(); }

	inline void addCurrentNodeToStack() { pTree_->addCurrentNodeToStack(); }

	inline int getNbDives() { return pTree_->getNbDives(); }

	inline void resetNbNodesSinceLastDive() { pTree_->resetNbNodesSinceLastDive(); }

	inline virtual int nbSolutions() { return 0; }

	inline void setSearchStrategy(SearchStrategy searchStrategy){
		searchStrategy_ = searchStrategy;
		set_search_strategy(searchStrategy);
	}

	inline SearchStrategy getSearchStrategy(){ return searchStrategy_; }

	inline virtual void setParameters(SolverParam parameters){
		parameters_ = parameters;
		setVerbosity(parameters_.verbose_);
		logfile_ = parameters.logfile_;
	}
	inline std::string logfile() {return logfile_;}

	inline SolverParam& getParameters() { return parameters_; }

	inline void setLogFile(std::string fileName) {logfile_ = fileName;}

	inline void setInfinity(double inf) {infinity_=inf;}

	//get the variables that are generating during the resolution (columns)
	const std::vector<MyVar*>& getActiveColumns() const {
    return activeColumnVars_;
  }

	void addActiveColumn(MyVar* var, int index=-1) {
		activeColumnVars_.push_back(var);
	}

	virtual void clearActiveColumns() {
		activeColumnVars_.clear();
	}

	// Fix every rotation to one : this is useful only when the active columns
	// are only the rotations included in a provided initial solution
	virtual void fixEveryRotation() {}

	// fix/unfix all the rotations variables starting from the input vector of days
	virtual void fixRotationsStartingFromDays(const std::vector<bool>& isFixDay) {}
	virtual void unfixRotationsStartingFromDays(const std::vector<bool>& isUnfixDay){}

	// fix/unfix all the rotations variables of the input nurses
	virtual void fixRotationsOfNurses(const std::vector<bool>& isFixNurse) {}
	virtual void unfixRotationsOfNurses(const std::vector<bool>& isUnfixNurse){}

	// relax/unrelax the integrality of all the rotations variables starting from the input vector of days
	virtual void relaxRotationsStartingFromDays(const std::vector<bool>& isRelaxDay){}
	virtual void unrelaxRotationsStartingFromDays(const std::vector<bool>& isUnRelaxDay){}

	// Set the value of the active columns with those in the best solution
	virtual void setActiveColumnsValuesWithBestSol() {}

	// Clear the active column, set the active columns with those in the best solution,
	// and set the primal values accordingly
	virtual bool loadBestSol() {return true;}

	// Get te current level in the branch and bound tree
	//
	virtual int getCurrentTreeLevel() {return 0;};

	// get the current number of objects
	//
	int getNbObjectsInMemory() {return objects_.size();}


protected:
	//store all MyObject*
 	std::vector<MyObject*> objects_;
 	std::vector<MyVar*> binaryCoreVars_;
 	std::vector<MyVar*> integerCoreVars_;
 	std::vector<MyVar*> positiveCoreVars_;
 	std::vector<MyVar*> activeColumnVars_;
	int var_count = -1, cons_count = -1;

	MyPricer* pPricer_;
	MyBranchingRule* pBranchingRule_;
	MyTree* pTree_;

	int verbosity_ = 0;
	SearchStrategy searchStrategy_ = BestFirstSearch;

	SolverParam parameters_;

	// log file where outputs must be written
  std::string logfile_="";

	//Coin data
	double infinity_=1.2343423E23;
};


#endif /* SRC_MODELER_H_ */