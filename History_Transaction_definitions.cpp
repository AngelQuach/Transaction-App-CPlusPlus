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

// Constructor
// TASK 1
//
Transaction::Transaction( std::string ticker_symbol,  unsigned int day_date,  
        unsigned int month_date,  unsigned year_date, 
        bool buy_sell_trans,  unsigned int number_shares,  
        double trans_amount ){
    if(buy_sell_trans == true){
        this->trans_type = "Buy";
    } else{
        this->trans_type = "Sell";
    }
    this->p_next = nullptr;
    this->acb = 0;
    this->share_balance = 0;
    this->acb_per_share = 0;
    this->cgl = 0;
    this->symbol = ticker_symbol;
    this->day = day_date;
    this->month = month_date;
    this->year = year_date;
    this->shares = number_shares;
    this->amount = trans_amount;
    this->trans_id = assigned_trans_id;
    ++assigned_trans_id;
};

// Destructor
// TASK 1
//
Transaction::~Transaction(){
    //all work is done in ~History
};

// Overloaded < operator.
// TASK 2
//
bool Transaction::operator<( Transaction const &other ){
    //check if left operand equal than right operand
    if(this->get_year() == other.get_year()){
        if(this->get_month() == other.get_month()){
            if(this->get_day() == other.get_day()){
                return ( (this->get_trans_id()) > (other.get_trans_id()) );
            } else{
                return ( (this->get_day()) < (other.get_day()) );
            }
        } else{
            return ( (this->get_month()) < (other.get_month()) );
        }
    } else{
        return ( (this->get_year()) < (other.get_year()) );
    }
};

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
    //initialize the linked list to be empty
    p_head = nullptr;
};

// Destructor
// TASK 3
//
History::~History(){
    while(p_head != nullptr){
        Transaction *p_old_head{this->get_p_head()};
        p_head = p_head->get_next(); 
        delete p_old_head;
        p_old_head = nullptr; 
    }                                                                              
};

// read_history(...): Read the transaction history from file.
// TASK 4
//
void History::read_history(){
    Transaction *temp{this->get_p_head()};
    ece150::open_file();
    //while there are still entries in the txt doc
    while(ece150::next_trans_entry() == true){
        Transaction *trans = new Transaction(ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(), 
                ece150::get_trans_year(), ece150::get_trans_type(), ece150::get_trans_shares(), ece150::get_trans_amount());
        //if history is empty
        if(temp == nullptr){
            this->p_head = trans;
            trans->set_next(nullptr);
        } 
        //if not, find the tail and add the new entry at the tail
        else{
            while((temp->get_next()) != nullptr){
                temp = temp->get_next();
            }
            temp->set_next(trans);
            trans->set_next(nullptr);
        }
        //reset temp to the head of the linked list
        temp = this->get_p_head();
    }
    ece150::close_file();
};

// insert(...): Insert transaction into linked list.
// TASK 5
//
void History::insert(Transaction *p_new_trans){
    //temporary pointer used to loop through linked list
    Transaction *temp{this->get_p_head()};
    //if history is empty
    if(temp == nullptr){
        this->p_head = p_new_trans;
        p_new_trans->set_next(nullptr);
    } 
    //if not empty, find the tail of the list and add the new transaction
    else{
        while((temp->get_next() != nullptr)){
            temp = temp->get_next();
        }
        temp->set_next(p_new_trans);
        p_new_trans->set_next(nullptr);
    }
};

// sort_by_date(): Sort the linked list by trade date.
// TASK 6
void History::sort_by_date(){
    //loop thru the list
    Transaction *loop{nullptr};
    //pointer that stores the address of the entry after the curr entry
    Transaction *next{nullptr};
    //temp pointer that stores the current p_head
    Transaction *curr(this->get_p_head());

    //check if linked list is empty or only contains one entry, if yes return
    if(curr == nullptr || (curr)->get_next() == nullptr){
        return;
    }

    Transaction* prev_curr = curr;
    curr = curr->get_next();

    //else, there are more than one entry in the linked list
    //sort the list
    while(curr != nullptr){

        if((*prev_curr) < (*curr)){
            prev_curr = curr;
            curr = curr->get_next();
            continue;
        }

        //loop thru the list to find the entry (temp) that is later than head
        //starts from current p_head
        loop = this->get_p_head();
        Transaction* prev_loop = nullptr;

        while( (*loop) < (*curr) ){
            prev_loop = loop;
            loop = loop->get_next();
        }

        //Remove current entry from the list
        next = curr->get_next();
        prev_curr->set_next(next);

        //if the current entry is earlier than the head
        if(prev_loop == nullptr){
            curr->set_next(this->get_p_head());
            p_head = curr;
        } else{
            //Insert current
            curr->set_next(loop);
            prev_loop->set_next(curr);
        }
        //Check next entry
        curr = next;
    }
}

// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//
void History::update_acb_cgl(){
    //update for the p_head first
    //p_head is supposed to be a Buy
    Transaction *curr {this->get_p_head()};
    curr->set_acb(curr->get_amount());
    curr->set_share_balance(curr->get_shares());
    curr->set_acb_per_share((curr->get_acb()) / (curr->get_share_balance()));

    //update for the other entries
    while(curr->get_next() != nullptr){
        //for buy
        if((curr->get_next())->get_trans_type() == true){
            (curr->get_next())->set_acb(curr->get_acb() + (curr->get_next())->get_amount());
            (curr->get_next())->set_share_balance(curr->get_share_balance() + (curr->get_next())->get_shares());
            (curr->get_next())->set_acb_per_share(((curr->get_next())->get_acb()) / ((curr->get_next())->get_share_balance()));
        }
        //for sell
        else{
            (curr->get_next())->set_acb(curr->get_acb() - ((curr->get_next())->get_shares())*(curr->get_acb_per_share()));
            (curr->get_next())->set_share_balance(curr->get_share_balance() - (curr->get_next())->get_shares());
            (curr->get_next())->set_acb_per_share(((curr->get_next())->get_acb()) / ((curr->get_next())->get_share_balance()));
            (curr->get_next())->set_cgl((curr->get_next())->get_amount() - ((curr->get_next())->get_shares()) * (curr->get_acb_per_share()));
        }
        curr = curr->get_next();
    } 
}

// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8
double History::compute_cgl(unsigned int year){
    //check from p_head to find the start of the ideal list
    //p_head is supposed to be a Buy
    Transaction *curr {this->get_p_head()};
    double sum{0};

    //if p_head is in the same year
    if(curr->get_year() == year){
        sum += curr->get_cgl();
    }
    //check whether the transaction is in the same year, if yes, add up
    while(curr->get_next() != nullptr){
        if((curr->get_next())->get_year() == year){
            sum += (curr->get_next())->get_cgl();
        }
        curr = curr->get_next();
    }
    return sum;
}

// print() Print the transaction history.
//TASK 9
//
void History::print(){
    Transaction *temp{this->get_p_head()};
    std::cout << "========== BEGIN TRANSACTION HISTORY ==========" << std::endl;
    while(temp != nullptr){
        temp->print();
        temp = temp->get_next();
    }
    std::cout << "========== END TRANSACTION HISTORY ==========" << std::endl;
};

// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }
