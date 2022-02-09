//
// Created by joelpryde on 2/9/2022.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "table.h"

TEST_CASE( "table init", "[table]" )
{
    Table table;
    initTable(&table);
    freeTable(&table);

    //REQUIRE( table == 5 );
}