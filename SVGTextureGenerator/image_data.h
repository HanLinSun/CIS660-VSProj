#pragma once
#ifndef IMAGE_DATA_H
#define IMAGE_DATA_H

#include <memory>
#include <unordered_map>
#endif // IMAGE_DATA_H
#include "cluster.h"
#include <unordered_set>

class image_data
{
private:
    // one tage one cluster, using
    float neighbor_r=2;
    float patch_size = 30;
    float overlap_r = 1;
    float patch_number = 300;

public:
    
    float desired_width = 150;// the desired width of output;
    float desired_hight = 150;// the desired height;
    float input_width = 200;
    float input_hight = 250;

    std::unordered_map<int, std::unique_ptr<cluster>> cluster_data;
    std::unordered_map<int, std::unique_ptr<sample>> sample_data;

    std::unordered_map<int, std::unique_ptr<cluster>> output_cluster;
    std::unordered_map<int, std::unique_ptr<sample>> output_sample_data;

    std::unordered_map<int, std::vector<int>> sample_hisgraph;
    std::unordered_map<int, std::vector<int>> output_hisgraph;

    std::unordered_map<int, std::pair<int, vec2>> cluster_map;

    // final cluster map
    std::unordered_map<int, std::pair<int,vec2>> final_cluster_map;// value1: the cluster of output. value2: the origional cluster that cluster[value1] refers to.

    //std::unordered_map<int, int> cluster_compare;

    //std::unordered_map<int, std::unique_ptr<sample>> final_output_sample_data;
    std::unordered_map<int,std::vector<vec2>> final_output_sample_data; // index is the new cluster ID, as a value in 


    std::unordered_map<int, int> sample_pair;

    image_data();
    //use this function to import cluster data
    void calculate_hisgraph();//pre calculate the hisgraph


    //for each sample in sample_data, calculate its neighborhood information

    void calculate_neighbor(int option);

    float calculate_distance(sample* nSo, sample* nS1);

    std::vector<int> histogram_distribution(vec2 input, cluster* cluster);
    void init_output_image();// initialize the basic output data

    void pair_match(); // for each output sample, find a most close input sample;

    void optimize_output(); // overlap the previous output with closest sample's neighbor;


    void setPathNum(float in_patchNumber);
    void setOverlap(float in_overlap);
    void setPatchSize(float in_pathSize);
    void setNeighbour(float in_neighbour);




};
