#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//
// g++ -o main History_Transaction_definitions.cpp project4.cpp main.cpp -std=c++11
// Constructor
// TASK 1
//
Transaction::Transaction(std::string ticker_symbol, unsigned int day_date, unsigned int month_date, unsigned year_date, bool buy_sell_trans, unsigned int number_shares,double trans_amount) 
{
  this->trans_id = assigned_trans_id; 
  assigned_trans_id++;
  this->symbol = ticker_symbol;
  this->day = day_date;
  this->month = month_date;
  this->year = year_date;
  this->p_next = nullptr; 
  if (buy_sell_trans){
    this->trans_type = "Buy";
  }
  else { 
    this->trans_type = "Sell";
  }
  this->shares = number_shares;
  this->amount = trans_amount;
  double acb = 0.0;
  double acb_per_share = 0.0;
  unsigned int share_balance = 0;
  double cgl = 0.0;
  }
  
// Destructor
// TASK 1
//
Transaction::~Transaction() {
  this->p_next = nullptr; 
  }

// Overloaded < operator.
// TASK 2
// 
bool Transaction::operator<( Transaction const &other ) { 
  if (this->year < other.year) {
    return true;
  }
  else if (this->year > other.year) {
    return false;
  }
  else {
    if (this->month < other.month){
      return true;
    }
    else if (this->month > other.month){
      return false;
    }
    else {
      if (this->day < other.day) {
        return true;
      }
      else if (this->day > other.day) {
        return false;
      }
      else {
        if (this->trans_id > other.trans_id){
          return true;
        }
        else {
          return false;
        }
      }
    }
  }

}
// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK 3
//
History::History(){
  this->p_head = nullptr;
}
// Destructor
// TASK 3
//
History::~History(){
  //Transaction* p_head {get_p_head()};
  Transaction* temp = {get_p_head()};
  while (p_head != nullptr)
  {
    p_head = p_head->get_next();
    delete temp;
    temp = p_head;
  } 
  delete temp;
  temp = nullptr;
}
// read_history(...): Read the transaction history from file.
// TASK 4
//
void History::read_history(){
  ece150::open_file();
  while (ece150::next_trans_entry()){
    Transaction* p_new = new Transaction(ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(), ece150::get_trans_year(), ece150::get_trans_type(), ece150::get_trans_shares(), ece150::get_trans_amount());
    insert(p_new);
  }
  ece150::close_file();
}
// insert(...): Insert transaction into linked list.
// TASK 5
//
void History::insert( Transaction *p_new_trans ){
  Transaction* next{get_p_head()};
  if (next == nullptr) //if list points to nothing, new trans becomes the head.
  { 
    p_head = p_new_trans;
  } else {
  while (next->get_next() != nullptr)
  {
    next = next->get_next();
  } 
  next->set_next(p_new_trans);
  }
  next = nullptr;
}
// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//
void History::sort_by_date(){
  Transaction* original = p_head;
  Transaction* sort = nullptr;
  Transaction* test_head = nullptr;
  Transaction* test_tail = nullptr;
  if (p_head == nullptr){
    return;
    }
while(p_head != nullptr){
    original = p_head;  
    p_head = p_head->get_next();
    original->set_next(nullptr);

    if (sort == nullptr){
      sort = original;
      continue;
    }
    // head
    if (*original < *sort){
      original->set_next(sort);
      sort = original;
    }
    else {
        test_head = sort;
        test_tail = sort->get_next();
        while((test_tail != nullptr) && (*test_tail < *original)){
          test_head = test_tail;
          test_tail = test_tail->get_next();
        }

        if (test_tail == nullptr){
          // tail
          test_head->set_next(original);
          continue;
        }else{
          // middle
          test_head->set_next(original);
          original->set_next(test_tail);
        }
      }
  } 
  p_head = sort;
}
// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//
void History::update_acb_cgl() {
  Transaction *temp {get_p_head()};
  double acb{0.0};
  int share_balance {0};
  double acb_share {0.0};
  double cgl {0.0};
  while (temp != nullptr){
    if (temp->get_trans_type()){
      acb += (temp->get_amount());
      share_balance += (temp->get_shares());
      acb_share = (acb/share_balance);
    }
    else {
      acb -= (temp->get_shares()*acb_share);
      share_balance -= (temp->get_shares());
      cgl = (temp->get_amount() - (temp->get_shares()*acb_share));
    }
    temp->set_acb(acb);
    temp->set_share_balance(share_balance);
    temp->set_cgl(cgl);
    cgl = 0.0;
    temp->set_acb_per_share(acb_share);
    temp = temp->get_next();
  }
}
// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8
double History::compute_cgl( unsigned int year) {
  Transaction* temp = p_head;
  while(year != temp->get_year()){
    temp = temp->get_next();
  }
  double cgl {0.0};
  while(temp != nullptr && year == temp->get_year()){
    cgl += temp->get_cgl();
    temp = temp->get_next();
  }
  return cgl;
}
// print() Print the transaction history.
//TASK 9
//

void History::print() {
  Transaction* head {get_p_head()};
  if (head == nullptr) {
    std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;
    std::cout << "========== END TRANSACTION HISTORY ============" << std::endl;
    return;
  }
  std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;
  while (head != nullptr) {
    head->print();
    head = head->get_next();
  }
  std::cout << "========== END TRANSACTION HISTORY ============" << std::endl;
}
// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }