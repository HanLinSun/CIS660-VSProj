#pragma once
#ifndef CLUSTER_H
#define CLUSTER_H

#include <unordered_map>
#include <vector>
#endif // CLUSTER_H

#include <glm/common.hpp>
#include <glm/vec3.hpp>

using namespace glm;


struct sample {
    int index = 0;
    vec2 position = vec2(0);
    int cluster_ID;
    vec3 col = vec3(0);
    //value1: neighborhoodID
    //value2: neighborhood distance
    std::unordered_map<int, float> neighborhood;

};


// the cluster a sample belonging to. one element only have one cluster!
class cluster {
private:
    int id = 0;


public:
    cluster();
    std::vector<vec2> sample_list;

    void sample_cluster();
    void set_id(int);
    int get_id();

};

