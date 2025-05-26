//======= Copyright (c) 2025, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     Jakub Lůčný <xlucnyj00@stud.fit.vutbr.cz>
// $Date:       $2025-02-22
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author Jakub Lůčný
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"
#include <gmock/gmock.h>

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//
using namespace ::testing;

// Fixture for tests with empty tree
class EmptyTree : public Test{
protected:
    BinaryTree tree;
};

// Fixture for tests with nonempty tree
class NonEmptyTree : public Test{

    // Unused variable, just for inserting multiple nodes into tree
    std::vector<std::pair<bool, Node_t *>> outNewNodes;

    // Constructor creates tree with 7 nodes
    void SetUp(){
        tree.InsertNodes({1, 3, 5, 2, 4, 7, 6}, outNewNodes);
    }

protected:
    BinaryTree tree;
};

// Start of EMPTY TREE tests
TEST_F(EmptyTree, InsertNode){
    auto pair1 = tree.InsertNode(1);
    auto pair2 = tree.InsertNode(100);

    EXPECT_EQ(pair1.first, true);
    EXPECT_NE(pair1.second, nullptr);

    EXPECT_EQ(pair2.first, true);
    EXPECT_NE(pair2.second, nullptr);

    // Check if values are actually present in the tree
    std::vector<Node_t *> NonLeafNodes;
    tree.GetNonLeafNodes(NonLeafNodes);

    // Get keys from NonLeafNodes
    std::vector<int> keys;
    for (auto node : NonLeafNodes) {
        keys.push_back(node->key);
    }

    // Check if keys match the expected values
    EXPECT_THAT(keys, UnorderedElementsAre(1, 100));
}

TEST_F(EmptyTree, DeleteNode){
    for (int i = 0; i < 20; i++){
        EXPECT_FALSE(tree.DeleteNode(i));
    }

    EXPECT_FALSE(tree.DeleteNode(42));
}

TEST_F(EmptyTree, FindNode){
    for (int i = 20; i < 40; i++){
        EXPECT_EQ(tree.FindNode(i), nullptr);
    }

    EXPECT_EQ(tree.FindNode(100), nullptr);
}

// Start of NONEMPTY TREE tests
TEST_F(NonEmptyTree, InsertNode){
    auto pair1 = tree.InsertNode(1);
    auto pair2 = tree.InsertNode(100);

    EXPECT_EQ(pair1.first, false);
    ASSERT_NE(pair1.second, nullptr);

    EXPECT_EQ(pair2.first, true);
    ASSERT_NE(pair2.second, nullptr);

    std::vector<Node_t *> NonLeafNodes;
    tree.GetNonLeafNodes(NonLeafNodes);

    // Get keys from NonLeafNodes
    std::vector<int> keys;
    for (auto node : NonLeafNodes) {
        keys.push_back(node->key);
    }

    // Check if keys match the expected values
    EXPECT_THAT(keys, UnorderedElementsAre(1, 2, 3, 4, 5, 6, 7, 100));
}

TEST_F(NonEmptyTree, DeleteNode){
    EXPECT_EQ(tree.DeleteNode(1), true);
    EXPECT_EQ(tree.DeleteNode(5), true);
    EXPECT_EQ(tree.DeleteNode(100), false);

    std::vector<Node_t *> NonLeafNodes;
    tree.GetNonLeafNodes(NonLeafNodes);

    // Get keys from NonLeafNodes
    std::vector<int> keys;
    for (auto node : NonLeafNodes) {
        keys.push_back(node->key);
    }

    // Check if keys match the expected values
    EXPECT_THAT(keys, UnorderedElementsAre(2, 3, 4, 6, 7));
}

TEST_F(NonEmptyTree, FindNode){
    EXPECT_NE(tree.FindNode(1), nullptr);
    EXPECT_NE(tree.FindNode(6), nullptr);
    EXPECT_EQ(tree.FindNode(100), nullptr);
}

// Start of AXIOMS tests
TEST(TreeAxioms, Axiom1){
    // Create sample tree (empty)
    BinaryTree tree;

    // Get all leaf nodes (none)
    std::vector<Node_t *> LeafNodes;
    tree.GetLeafNodes(LeafNodes);

    for (auto leaf : LeafNodes){
        EXPECT_EQ(leaf->color, BLACK);
    }

    // Put 3 nodes into the tree
    std::vector<std::pair<bool, Node_t *>> outNewNodes;
    tree.InsertNodes({5, 1, 2}, outNewNodes);

    // Check axiom
    tree.GetLeafNodes(LeafNodes);
    for (auto leaf : LeafNodes){
        EXPECT_EQ(leaf->color, BLACK);
    }

    tree.DeleteNode(1);
    // Check axiom
    tree.GetLeafNodes(LeafNodes);
    for (auto leaf : LeafNodes){
        EXPECT_EQ(leaf->color, BLACK);
    }

    tree.DeleteNode(5);
    // Check axiom
    tree.GetLeafNodes(LeafNodes);
    for (auto leaf : LeafNodes){
        EXPECT_EQ(leaf->color, BLACK);
    }

    tree.DeleteNode(2);
    // Check axiom
    tree.GetLeafNodes(LeafNodes);
    for (auto leaf : LeafNodes){
        EXPECT_EQ(leaf->color, BLACK);
    }

    // Refill tree with 9 nodes
    tree.InsertNodes({5, 1, 2, 3, 4, 6, 7, 8, 9}, outNewNodes);
    // Check axiom
    tree.GetLeafNodes(LeafNodes);
    for (auto leaf : LeafNodes){
        EXPECT_EQ(leaf->color, BLACK);
    }
}

TEST(TreeAxioms, Axiom2){
    // Create sample tree (empty)
    BinaryTree tree;

    // Get all nodes (none)
    std::vector<Node_t *> allNodes;
    tree.GetAllNodes(allNodes);

    // Check the axiom
    for (auto node : allNodes){
        if (node->color == RED){
            EXPECT_EQ(node->pLeft->color, BLACK);
            EXPECT_EQ(node->pRight->color, BLACK);
        }
    }

    // Put 3 nodes into the tree
    std::vector<std::pair<bool, Node_t *>> outNewNodes;
    tree.InsertNodes({5, 1, 2}, outNewNodes);

    tree.GetAllNodes(allNodes);
    for (auto node : allNodes){
        if (node->color == RED){
            EXPECT_EQ(node->pLeft->color, BLACK);
            EXPECT_EQ(node->pRight->color, BLACK);
        }
    }

    tree.DeleteNode(2);

    tree.GetAllNodes(allNodes);
    for (auto node : allNodes){
        if (node->color == RED){
            EXPECT_EQ(node->pLeft->color, BLACK);
            EXPECT_EQ(node->pRight->color, BLACK);
        }
    }

    // Refill tree to have 9 nodes
    tree.InsertNodes({2, 3, 4, 6, 7, 8, 9}, outNewNodes);

    tree.GetAllNodes(allNodes);
    for (auto node : allNodes){
        if (node->color == RED){
            EXPECT_EQ(node->pLeft->color, BLACK);
            EXPECT_EQ(node->pRight->color, BLACK);
        }
    }
}

TEST(TreeAxioms, Axiom3){
    // Create sample tree (empty)
    BinaryTree tree;

    // Get all leaf nodes (none)
    std::vector<Node_t *> LeafNodes;
    tree.GetLeafNodes(LeafNodes);

    // Vector to save how many black nodes are between root and every leaf node
    std::vector<size_t> blackToLeaf;

    for (auto leaf : LeafNodes){
        auto blackCnt = 0;
        while (leaf != nullptr){
            if (leaf->color == BLACK){
                blackCnt++;
            }
            leaf = leaf->pParent;
        }
        blackToLeaf.push_back(blackCnt);
    }

    // Check if all values in vector are the same
    if (blackToLeaf.size() > 0){
        auto expectedVal = blackToLeaf.front();
        for (auto val : blackToLeaf){
            EXPECT_EQ(val, expectedVal);
        }
    }

    // Put 9 nodes into the tree
    std::vector<std::pair<bool, Node_t *>> outNewNodes;
    tree.InsertNodes({2, 3, 4, 6, 7, 8, 9, 5, 1}, outNewNodes);

    for (auto leaf : LeafNodes){
        auto blackCnt = 0;
        while (leaf != nullptr){
            if (leaf->color == BLACK){
                blackCnt++;
            }
            leaf = leaf->pParent;
        }
        blackToLeaf.push_back(blackCnt);
    }

    // Check if all values in vector are the same
    if (blackToLeaf.size() > 0){
        auto expectedVal = blackToLeaf.front();
        for (auto val : blackToLeaf){
            EXPECT_EQ(val, expectedVal);
        }
    }
}

/*** Konec souboru black_box_tests.cpp ***/
