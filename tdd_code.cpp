//======= Copyright (c) 2025, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - graph
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     Jakub Lůčný <xlucnyj00@stud.fit.vutbr.cz>
// $Date:       $2025-02-22
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Martin Dočekal
 * @author Karel Ondřej
 *
 * @brief Implementace metod tridy reprezentujici graf.
 */

#include "tdd_code.h"


Graph::Graph(){}

Graph::~Graph(){
    clear();
}

std::vector<Node*> Graph::nodes() {
    return this->gNodes;
}

std::vector<Edge> Graph::edges() const{
    return this->gEdges;
}

Node* Graph::addNode(size_t nodeId) {
    // Check if node with given id already exists
    for (auto node : this->gNodes){
        if (nodeId == node->id){
            return nullptr;
        }
    }

    // If not, creates a new one
    Node* new_node = new Node();
    new_node->id = nodeId;
    new_node->color = 0;
    this->gNodes.push_back(new_node);

    return new_node;
}

bool Graph::addEdge(const Edge& edge){
    // Check for loop edge
    if (edge.a == edge.b){
        return false;
    }

    // Check for duplicates
    for (auto currEdge : this->gEdges){
        if (currEdge == edge){
            return false;
        }
    }

    // Add edge to the graph list
    this->gEdges.push_back(edge);
    // Create nodes if they don't exist yet
    addNode(edge.a);
    addNode(edge.b);

    return true;
}

void Graph::addMultipleEdges(const std::vector<Edge>& edges) {
    for (size_t i = 0; i < edges.size(); i++) {
        addEdge(edges.at(i));
    }
}

Node* Graph::getNode(size_t nodeId){
    // Go through all nodes in graph list to check if given node exists
    for(auto node : this->gNodes){
        if (node->id == nodeId){
            return node;
        }
    }

    return nullptr;
}

bool Graph::containsEdge(const Edge& edge) const{
    // Go through all edges in graph list to check if given edge exists
    for(auto currEdge : this->gEdges){
        if (currEdge == edge){
            return true;
        }
    }
    
    return false;
}

void Graph::removeNode(size_t nodeId){
    // Check if node exists
    if (getNode(nodeId)){
        // Check if node is part of any edges
        for (long i = edgeCount() - 1; i >= 0; i--){
            if (this->gEdges[i].a == nodeId || this->gEdges[i].b == nodeId){
                // remove edge
                this->gEdges.erase(this->gEdges.begin() + i);
            }
        }
        // Remove node
        for (long i = nodeCount() - 1; i >= 0; i--){
            if (this->gNodes[i]->id == nodeId){
                // remove node
                delete this->gNodes[i];
                this->gNodes.erase(this->gNodes.begin() + i);
            }
        }

        return;
    }

    // If node doesn't exist in graph
    throw std::out_of_range("Error: (out_of_range) trying to remove node that doesn't exist\n");
}

void Graph::removeEdge(const Edge& edge){
    // Check if given edge exists
    if (containsEdge(edge)){
        for(size_t i = 0; i < edgeCount(); i++){
            if (this->gEdges[i] == edge){
                // remove the edge
                this->gEdges.erase(this->gEdges.begin() + i);
                return;
            }
        }
    }

    throw std::out_of_range("Error: (out_of_range) trying to remove edge that doesn't exist\n");
}

size_t Graph::nodeCount() const{
    return this->gNodes.size();
}

size_t Graph::edgeCount() const{
    return this->gEdges.size();
}

size_t Graph::nodeDegree(size_t nodeId) const{

    Node* nodeExist = nullptr; 
    // Check if given node exists in graph
    for(auto node : this->gNodes){
        if (node->id == nodeId){
            nodeExist = node;
        }
    }

    // If it exists
    if (nodeExist){
        // Default value
        size_t degree = 0;

        for (auto edge : this->gEdges){
            if (edge.a == nodeId || edge.b == nodeId){
                degree++;
            }
        }
        return degree;
    }

    // Node doesn't exist
    throw std::out_of_range("Error: (out_of_range) trying to remove edge that doesn't exist\n");
    return 0;
}

size_t Graph::graphDegree() const{
    // Go through all nodes in graph list
    size_t maxDegree = 0;
    size_t currDegree = 0;

    // Check degree of every node
    for(Node* node : this->gNodes){
        currDegree = nodeDegree(node->id);

        if (currDegree > maxDegree){
            maxDegree = currDegree;
        }
    }

    return maxDegree;
}

// Using simple greedy coloring
void Graph::coloring(){
    for (auto node : this->gNodes){
        std::unordered_set<size_t> usedColors;

        // Check colors of adjacent nodes
        for (auto edge : this->gEdges){
            if (edge.a == node->id){
                Node* neighbor = getNode(edge.b);
                if (neighbor){
                    usedColors.insert(neighbor->color);
                }
            }

            if (edge.b == node->id) {
                Node* neighbor = getNode(edge.a);
                if (neighbor) {
                    usedColors.insert(neighbor->color);
                }
            }
        }

        // Find the smallest available color
        size_t color = 1;
        while (usedColors.count(color)) {
            color++;
        }

        // Assign the color to the node
        node->color = color;
    }
}

void Graph::clear() {
    for (auto node : this->gNodes){
        delete node;
    }

    this->gNodes.clear();
    this->gEdges.clear();
}

/*** Konec souboru tdd_code.cpp ***/
