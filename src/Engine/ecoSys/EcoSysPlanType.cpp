/*
 * ====================== EcoSysPlanType.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2020.01.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "EcoSysPlanType.h"


//--------------- Libs ------------------//
#include "magic_enum.hpp"

//--------------- Engine ------------------//
#include "tprAssert.h"

//-- tmp debug --
#include <string>
#include <iostream>
using std::cout;
using std::endl;

EcoSysPlanType str_2_ecoSysPlanType( const std::string &str_ )noexcept{

    auto labelOP = magic_enum::enum_cast<EcoSysPlanType>(str_);
    if( labelOP.has_value() ){
        return *labelOP;
    }else{
        cout << "can't find EcoSysPlanType: " << str_ << endl;
        tprAssert(0);
        return EcoSysPlanType::BegIdx; // never reach
    }
}





