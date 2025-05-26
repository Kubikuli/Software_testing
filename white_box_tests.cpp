//======= Copyright (c) 2025, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - test suite
//
// $NoKeywords: $ivs_project_1 $white_box_tests.cpp
// $Author:     Jakub Lůčný <xlucnyj00@stud.fit.vutbr.cz>
// $Date:       $2025-02-28
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author Jakub Lůčný
 * 
 * @brief Implementace testu hasovaci tabulky.
 */

#include <vector>

#include "gtest/gtest.h"

#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy hasovaci tabulky, testujte nasledujici:
// 1. Verejne rozhrani hasovaci tabulky
//     - Vsechny funkce z white_box_code.h
//     - Chovani techto metod testuje pro prazdnou i neprazdnou tabulku.
// 2. Chovani tabulky v hranicnich pripadech
//     - Otestujte chovani pri kolizich ruznych klicu se stejnym hashem 
//     - Otestujte chovani pri kolizich hashu namapovane na stejne misto v 
//       indexu
//============================================================================//
using namespace ::testing;

// Fixture for tests with empty hash map
class EmptyHashMap : public Test{
    void SetUp(){
        hashMap = hash_map_ctor();
    }

    void TearDown(){
        hash_map_dtor(hashMap);
    }

protected:
    hash_map_t *hashMap;
};

// Fixture for tests with nonempty hash map
class NonEmptyHashMap : public Test{

    void SetUp(){
        hashMap = hash_map_ctor();
        std::vector<const char*> fruits = {"apple", "pear", "banana", "orange"};
        int idx = 2;
        for (auto fruit : fruits){
            hash_map_put(hashMap, fruit, idx);
            idx++;
        }
    }

    void TearDown(){
        hash_map_dtor(hashMap);
    }

protected:
    hash_map_t *hashMap;
};

// Fixture for tests with hash map 
// where two elements have same hash but different keys
class SameHashDifferentKeys : public Test{

    void SetUp(){
        hashMap = hash_map_ctor();
        std::vector<const char*> fruits = {"apple", "leapp", "banana", "orange"};
        int idx = 2;
        for (auto fruit : fruits){
            hash_map_put(hashMap, fruit, idx);
            idx++;
        }
    }

    void TearDown(){
        hash_map_dtor(hashMap);
    }

protected:
    hash_map_t *hashMap;
};

// Fixture for tests with hash map 
// where two elements have different hash but map to same index
class HashCollision : public Test{

    void SetUp(){
        hashMap = hash_map_ctor();
        // "abc" and "bce" have different hashes, but with hash map
        // of size 8 map to the same index
        std::vector<const char*> fruits = {"abc", "apple", "bce", "banana"};
        int idx = 2;
        for (auto fruit : fruits){
            hash_map_put(hashMap, fruit, idx);
            idx++;
        }
    }

    void TearDown(){
        hash_map_dtor(hashMap);
    }

protected:
    hash_map_t *hashMap;
};

// Start of EMPTY hash map tests
TEST_F(EmptyHashMap, hash_map_clear){
    hash_map_clear(hashMap);
    EXPECT_EQ(hashMap->used, 0);
    EXPECT_EQ(hashMap->allocated, 8);
    EXPECT_EQ(hashMap->first, nullptr);
    EXPECT_EQ(hashMap->last, nullptr);
}

TEST_F(EmptyHashMap, hash_map_reserve){
    auto retCode = hash_map_reserve(hashMap, 8);
    EXPECT_EQ(retCode, OK);

    retCode = hash_map_reserve(hashMap, 4);
    EXPECT_EQ(retCode, OK);

    retCode = hash_map_reserve(hashMap, 16);
    EXPECT_EQ(retCode, OK);
}

TEST_F(EmptyHashMap, hash_map_size){
    EXPECT_EQ(hash_map_size(hashMap), 0);
}

TEST_F(EmptyHashMap, hash_map_capacity){
    EXPECT_EQ(hash_map_capacity(hashMap), 8);
}

TEST_F(EmptyHashMap, hash_map_contains){
    EXPECT_EQ(hash_map_contains(hashMap, "apple"), 0);

    EXPECT_EQ(hash_map_contains(hashMap, "pear"), 0);
}

TEST_F(EmptyHashMap, hash_map_put){
    auto retCode = hash_map_put(hashMap, "apple", 5);
    EXPECT_EQ(retCode, OK);
}

TEST_F(EmptyHashMap, hash_map_get){
    int dst = -42;
    auto retCode = hash_map_get(hashMap, "apple", &dst);
    EXPECT_EQ(retCode, KEY_ERROR);
    EXPECT_EQ(dst, -42);
}

TEST_F(EmptyHashMap, hash_map_remove){
    auto retCode = hash_map_remove(hashMap, "apple");
    EXPECT_EQ(retCode, KEY_ERROR);
}

TEST_F(EmptyHashMap, hash_map_pop){
    int dst = -42;
    auto retCode = hash_map_pop(hashMap, "apple", &dst);
    EXPECT_EQ(retCode, KEY_ERROR);
    EXPECT_EQ(dst, -42);
}

// Start of NONEMPTY hash map tests
TEST_F(NonEmptyHashMap, hash_map_clear){
    hash_map_clear(hashMap);
    EXPECT_EQ(hashMap->used, 0);
    EXPECT_EQ(hashMap->allocated, 8);
    EXPECT_EQ(hashMap->first, nullptr);
    EXPECT_EQ(hashMap->last, nullptr);

    for (size_t i = 0; i < hashMap->allocated; i++){
        EXPECT_EQ(hashMap->index[i], nullptr);
    }
}

TEST_F(NonEmptyHashMap, hash_map_reserve){
    auto retCode = hash_map_reserve(hashMap, 8);
    EXPECT_EQ(retCode, OK);

    retCode = hash_map_reserve(hashMap, 3);
    EXPECT_EQ(retCode, VALUE_ERROR);
    auto retVal = hash_map_capacity(hashMap);
    EXPECT_EQ(retVal, 8);

    retCode = hash_map_reserve(hashMap, 16);
    EXPECT_EQ(retCode, OK);
    retVal = hash_map_capacity(hashMap);
    EXPECT_EQ(retVal, 16);
}

TEST_F(NonEmptyHashMap, hash_map_size){
    auto retVal = hash_map_size(hashMap);
    EXPECT_EQ(retVal, 4);
}

TEST_F(NonEmptyHashMap, hash_map_capacity){
    auto retVal = hash_map_capacity(hashMap);
    EXPECT_EQ(retVal, 8);
}

TEST_F(NonEmptyHashMap, hash_map_contains){
    auto retVal = hash_map_contains(hashMap, "apple");
    EXPECT_NE(retVal, 0);

    retVal = hash_map_contains(hashMap, "grapes");
    EXPECT_EQ(retVal, 0);
}

TEST_F(NonEmptyHashMap, hash_map_put_retCodes){
    auto retCode = hash_map_put(hashMap, "apple", 9);
    EXPECT_EQ(retCode, KEY_ALREADY_EXISTS);

    retCode = hash_map_put(hashMap, "pineapple", 2);
    EXPECT_EQ(retCode, OK);
}

// Test for correct resizing when 2/3 are used
TEST_F(NonEmptyHashMap, hash_map_put_resizing){
    hash_map_put(hashMap, "pineapple", 4);
    EXPECT_EQ(hash_map_capacity(hashMap), 8);

    hash_map_put(hashMap, "grapes", 2);
    EXPECT_EQ(hash_map_capacity(hashMap), 8);

    hash_map_put(hashMap, "strawberry", 1);
    EXPECT_EQ(hash_map_capacity(hashMap), 16);
}

// Test trying to insert already existing item,
// that was added before resizing
TEST_F(NonEmptyHashMap, hash_map_put_3){
    hash_map_put(hashMap, "pineapple", 4);
    hash_map_put(hashMap, "grapes", 2);
    hash_map_put(hashMap, "strawberry", 1);

    auto retCode = hash_map_put(hashMap, "pineapple", 2);
    EXPECT_EQ(retCode, KEY_ALREADY_EXISTS);
}

// Checking correct size updating
TEST_F(NonEmptyHashMap, hash_map_put_usedSize){
    hash_map_put(hashMap, "pineapple", 4);
    EXPECT_EQ(hash_map_size(hashMap), 5);

    hash_map_put(hashMap, "grapes", 7);
    EXPECT_EQ(hash_map_size(hashMap), 6);

    hash_map_put(hashMap, "grapes", 1);
    EXPECT_EQ(hash_map_size(hashMap), 6);
}

TEST_F(NonEmptyHashMap, hash_map_get){
    int dst = -42;
    auto retCode = hash_map_get(hashMap, "apple", &dst);
    EXPECT_EQ(retCode, OK);
    EXPECT_NE(dst, -42);

    dst = -42;
    retCode = hash_map_get(hashMap, "grapes", &dst);
    EXPECT_EQ(retCode, KEY_ERROR);
    EXPECT_EQ(dst, -42);
}

TEST_F(NonEmptyHashMap, hash_map_remove){
    auto retCode = hash_map_remove(hashMap, "apple");
    EXPECT_EQ(retCode, OK);

    retCode = hash_map_remove(hashMap, "apple");
    EXPECT_EQ(retCode, KEY_ERROR);

    retCode = hash_map_remove(hashMap, "grapes");
    EXPECT_EQ(retCode, KEY_ERROR);
}

TEST_F(NonEmptyHashMap, hash_map_remove_usedSize){
    EXPECT_EQ(hash_map_size(hashMap), 4);

    // not in the hashMap
    hash_map_remove(hashMap, "grapes");

    EXPECT_EQ(hash_map_size(hashMap), 4);

    // present in hashMap
    hash_map_remove(hashMap, "apple");

    EXPECT_EQ(hash_map_size(hashMap), 3);
}

TEST_F(NonEmptyHashMap, hash_map_pop){
    int dst = -42;
    auto retCode = hash_map_pop(hashMap, "apple", &dst);
    EXPECT_EQ(retCode, OK);
    EXPECT_NE(dst, -42);

    dst = -42;
    retCode = hash_map_pop(hashMap, "apple", &dst);
    EXPECT_EQ(retCode, KEY_ERROR);
    EXPECT_EQ(dst, -42);

    retCode = hash_map_pop(hashMap, "grapes", &dst);
    EXPECT_EQ(retCode, KEY_ERROR);
    EXPECT_EQ(dst, -42);

    // x maps to last index
    hash_map_put(hashMap, "x", 0);
    retCode = hash_map_pop(hashMap, "x", &dst);
    EXPECT_EQ(retCode, OK);
    EXPECT_NE(dst, -42);
}

// Start of SameHashDifferentKeys hash map tests
TEST_F(SameHashDifferentKeys, hash_map_size){
    EXPECT_EQ(hash_map_size(hashMap), 4);
}

TEST_F(SameHashDifferentKeys, hash_map_contains){
    auto retVal = hash_map_contains(hashMap, "apple");
    EXPECT_NE(retVal, 0);

    retVal = hash_map_contains(hashMap, "leapp");
    EXPECT_NE(retVal, 0);

    retVal = hash_map_contains(hashMap, "banana");
    EXPECT_NE(retVal, 0);

    retVal = hash_map_contains(hashMap, "ppale");
    EXPECT_EQ(retVal, 0);
}

TEST_F(SameHashDifferentKeys, hash_map_put){
    auto retCode = hash_map_put(hashMap, "ppale", 9);
    EXPECT_EQ(retCode, OK);

    auto retVal = hash_map_contains(hashMap, "ppale");
    EXPECT_NE(retVal, 0);
}

TEST_F(SameHashDifferentKeys, hash_map_get){
    int dst = -42;
    auto retCode = hash_map_get(hashMap, "leapp", &dst);
    EXPECT_EQ(retCode, OK);
    EXPECT_NE(dst, -42);
}

TEST_F(SameHashDifferentKeys, hash_map_remove){
    auto retCode = hash_map_remove(hashMap, "apple");
    EXPECT_EQ(retCode, OK);

    auto retVal = hash_map_contains(hashMap, "leapp");
    EXPECT_NE(retVal, 0);
}

TEST_F(SameHashDifferentKeys, hash_map_pop){
    int dst = -42;
    auto retCode = hash_map_pop(hashMap, "apple", &dst);
    EXPECT_EQ(retCode, OK);
    EXPECT_NE(dst, -42);

    dst = -42;
    retCode = hash_map_pop(hashMap, "leapp", &dst);
    EXPECT_EQ(retCode, OK);
    EXPECT_NE(dst, -42);
}

// Start of HashCollision hash map tests
TEST_F(HashCollision, hash_map_size){
    EXPECT_EQ(hash_map_size(hashMap), 4);
}

TEST_F(HashCollision, hash_map_contains){
    auto retVal = hash_map_contains(hashMap, "abc");
    EXPECT_NE(retVal, 0);

    retVal = hash_map_contains(hashMap, "bce");
    EXPECT_NE(retVal, 0);

    retVal = hash_map_contains(hashMap, "apple");
    EXPECT_NE(retVal, 0);

    retVal = hash_map_contains(hashMap, "abe");
    EXPECT_EQ(retVal, 0);
}

TEST_F(HashCollision, hash_map_put){
    auto retCode = hash_map_put(hashMap, "aad", 9);
    EXPECT_EQ(retCode, OK);

    auto retVal = hash_map_contains(hashMap, "aad");
    EXPECT_NE(retVal, 0);
}

TEST_F(HashCollision, hash_map_get){
    int dst = -42;
    auto retCode = hash_map_get(hashMap, "bce", &dst);
    EXPECT_EQ(retCode, OK);
    EXPECT_NE(dst, -42);
}

TEST_F(HashCollision, hash_map_remove){
    auto retCode = hash_map_remove(hashMap, "abc");
    EXPECT_EQ(retCode, OK);

    retCode = hash_map_remove(hashMap, "aad");
    EXPECT_EQ(retCode, KEY_ERROR);

    auto retVal = hash_map_contains(hashMap, "bce");
    EXPECT_NE(retVal, 0);
}

TEST_F(HashCollision, hash_map_pop){
    int dst = -42;
    auto retCode = hash_map_pop(hashMap, "abc", &dst);
    EXPECT_EQ(retCode, OK);
    EXPECT_NE(dst, -42);

    dst = -42;
    retCode = hash_map_pop(hashMap, "bce", &dst);
    EXPECT_EQ(retCode, OK);
    EXPECT_NE(dst, -42);
}
/*** Konec souboru white_box_tests.cpp ***/
