#pragma once
#ifndef IMAGE_DATA_H
#define IMAGE_DATA_H

#include <memory>
#include <unordered_map>
#endif // IMAGE_DATA_H
#include "cluster.h"


class image_data
{
private:
    // one tage one cluster, using
    float neighbor_r=3;
    float desired_width=300;// the desired width of output;
    float desired_hight=300;// the desired height;
    float patch_size = 6;

    std::unordered_map<int, std::unique_ptr<cluster>> cluster_data;
    std::unordered_map<int, std::unique_ptr<sample>> sample_data;

    std::unordered_map<int, std::unique_ptr<cluster>> output_cluster;
    std::unordered_map<int, std::unique_ptr<sample>> output_sample_data;

public:
    image_data();
    //use this function to import cluster data
    void import_cluster_data();

    //for each sample in sample_data, calculate its neighborhood information

    void calculate_neighbor(int option);

    float calculate_distance(sample* nSo, sample* nS1);

    std::vector<int> histogram_distribution(vec2 input, cluster* cluster);
    void init_output_image();

};
