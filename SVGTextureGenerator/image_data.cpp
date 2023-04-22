#include "image_data.h"
#include <glm/detail/func_geometric.hpp>
#include <cmath>
#include <random>
#include <iostream>


image_data::image_data()
{
    


}
void image_data::calculate_hisgraph()
{
    for (auto& [key2, sample] : this->sample_data) {
        std::vector<int> hs2 = histogram_distribution(this->sample_data[key2].get()->position, cluster_data[this->sample_data[key2]->cluster_ID].get());
        this->sample_hisgraph[key2] = hs2;
    }

    for (auto& [key1, sample] : this->output_sample_data) {
        std::vector<int> hs1 = histogram_distribution(this->output_sample_data[key1].get()->position, output_cluster[this->output_sample_data[key1]->cluster_ID].get());
        this->output_hisgraph[key1] = hs1;
    }
}
void image_data::calculate_neighbor(int option) {
    if (option == 0) {
        for (auto& [key1, sample] : this->sample_data) {
            for (auto& [key2, target] : this->sample_data) {
                float phy_distance = glm::distance(sample->position, target->position);
                if (phy_distance < this->neighbor_r) {
                    sample->neighborhood[target->index] = phy_distance;
                    //B->neighborhood[A->index]=distance;
                }
            }
        }
    
    
    }
    else if (option == 1) {
        for (auto& [key1, sample] : this->output_sample_data) {
            for (auto& [key2, target] : this->output_sample_data) {
                float phy_distance = glm::distance(sample->position, target->position);
                if (phy_distance < this->neighbor_r) {
                    sample->neighborhood[target->index] = phy_distance;
                    //B->neighborhood[A->index]=distance;
                }
            }
        }
    }
};



float image_data::calculate_distance(sample* nSo, sample* nS1) {
    float distance = INFINITY;
    float sample_distance = 0;//
    float w;//sample weighted difference
    for (auto& [key1, distance1] : nSo->neighborhood) {
        for (auto& [key2, distance2] : nS1->neighborhood) {

            sample_distance = abs(distance1 - distance2);
            //std::vector<int> hs1 = histogram_distribution(this->output_sample_data[key1].get()->position, output_cluster[this->output_sample_data[key1]->cluster_ID].get());
            //std::vector<int> hs2 = histogram_distribution(this->sample_data[key2].get()->position, cluster_data[this->sample_data[key2]->cluster_ID].get());
            std::vector<int> hs1 = this->output_hisgraph[key1];
            std::vector<int> hs2 = this->sample_hisgraph[key2];
            w = 0.f;
            for (int i = 0; i < 24; i++) {
                if (hs1[i] + hs2[i] != 0) {
                    w += pow(hs1[i] - hs2[i], 2.f) / (hs1[i] + hs2[i]);
                }
            }

            sample_distance += 0.5 * w;

            distance = min(distance, sample_distance);
        }
    }
    return distance;
}


std::vector<int> image_data::histogram_distribution(vec2 input, cluster* m_cluster) {
    std::vector<int> output(24, 0);

    for (auto& m_sample : m_cluster->sample_list) {
        vec2 d_v = m_sample - input;
        float distance = glm::distance(m_sample, input);
        int multiplyer = 0;
        if (distance < 6) {
            multiplyer = 0;
        }
        else if (distance < 10) {
            multiplyer = 1;
        }
        else {
            multiplyer = 2;

        }

        if (d_v[0] > 0 && d_v[1] > 0 && d_v[0] > d_v[1]) {
            output[0 + multiplyer * 8] += 1;
        }
        else if (d_v[0] > 0 && d_v[1] > 0 && d_v[0] < d_v[1]) {
            output[1 + multiplyer * 8] += 1;
        }
        else if (d_v[0] < 0 && d_v[1]>0 && d_v[0] < d_v[1]) {
            output[2 + multiplyer * 8] += 1;
        }
        else if (d_v[0] < 0 && d_v[1]>0 && abs(d_v[0]) > abs(d_v[1])) {
            output[3 + multiplyer * 8] += 1;
        }
        else if (d_v[0] < 0 && d_v[1]<0 && abs(d_v[0])>abs(d_v[1])) {
            output[4 + multiplyer * 8] += 1;
        }
        else if (d_v[0] < 0 && d_v[1] < 0 && abs(d_v[0]) < abs(d_v[1])) {
            output[5 + multiplyer * 8] += 1;
        }
        else if (d_v[0] > 0 && d_v[1] < 0 && abs(d_v[0]) < abs(d_v[1])) {
            output[6 + multiplyer * 8] += 1;
        }
        else if (d_v[0] > 0 && d_v[1]<0 && abs(d_v[0])>abs(d_v[1])) {
            output[7 + multiplyer * 8] += 1;
        }

    }

    return output;
}

float m_ramdom(int low, int high) {
    return low + rand()%(high - low + 1);

}

//initialize the output image
void image_data::init_output_image()
{
    //assign random patch;
    //low + rand() % (high - low + 1)

    int counter = 0;

    int key = 0;
    //int cluster_count = 0;
    while (counter < this->patch_number) {
        vec2 curr_pos = vec2(m_ramdom(neighbor_r, this->input_hight - this->neighbor_r), m_ramdom(neighbor_r, this->input_width - this->neighbor_r));
        vec2 new_pos = vec2(m_ramdom(neighbor_r, this->desired_hight - this->neighbor_r), m_ramdom(neighbor_r, this->desired_width - this->neighbor_r));

        float length = this->patch_size / 2;

        vec2 distance_vect = new_pos - curr_pos;
        
        //first step : assign ramdom position to selected pitch  
        for (auto& [key1, m_sample] : this->sample_data) {

            /*
            if (m_sample->cluster_ID == 7) {

                vec2 distance_from_center = m_sample->position - curr_pos;
                std::cout <<"x: " << distance_from_center.x<<"y: " << distance_from_center.y << std::endl;
            }
            */
            vec2 distance_from_center = m_sample->position - curr_pos;
            if (-length < distance_from_center.x && distance_from_center.x < length && -length < distance_from_center.y && distance_from_center.y < length) {

                std::unique_ptr<sample> new_sample = std::make_unique<sample>();
                new_sample->position = new_pos + distance_from_center;
                new_sample->index = key;
                //std::cout << "x: " << distance_from_center.x << "y: " << distance_from_center.y << std::endl;
                new_sample->cluster_ID = counter;
                
                if (this->cluster_map.find(m_sample->cluster_ID) == cluster_map.end()) {
                    cluster_map[counter].first = m_sample->cluster_ID;
                    cluster_map[counter].second = distance_vect;

                
                }

                
                this->output_sample_data[key] = std::move(new_sample);
                key += 1;
            }
        }
        counter += 1;
    
    }
    

    std::unordered_map<int, int> cluster_compare;
    int cluster_index = 0;

    //second step: generat new output cluster/ sample dict's information
    for (auto& [key, m_sample] : this->output_sample_data) {
        //std::cout << m_sample->cluster_ID << std::endl;
        if (cluster_compare.find(m_sample->cluster_ID) == cluster_compare.end()) {
            cluster_compare[m_sample->cluster_ID] = cluster_index;

            final_cluster_map[cluster_index] = cluster_map[m_sample->cluster_ID];// 
            

            std::unique_ptr<cluster> new_cluster= std::make_unique<cluster>();
            new_cluster->sample_list.push_back(m_sample->position);
            new_cluster->set_id(cluster_index);
            this->output_cluster[cluster_index] = std::move(new_cluster);
            this->output_sample_data[key]->cluster_ID = cluster_index;
            //m_sample->cluster_ID = cluster_index;
            cluster_index += 1;
        }
        else {
            this->output_cluster[cluster_compare[m_sample->cluster_ID]]->sample_list.push_back(m_sample->position);
            this->output_sample_data[key]->cluster_ID = cluster_compare[m_sample->cluster_ID];
        }


    
    }
    this->calculate_neighbor(1);

    calculate_hisgraph();

}

void image_data::pair_match()
{
    float m_distance;
    int matched_ID = -1;
    std::cout << "start" << std::endl;
    for (auto& [key1, output] : this->output_sample_data) {

        float min_distance = 10000.f;

        for (auto& [key2, input] : this->sample_data) {
            m_distance = this->calculate_distance(output.get(), input.get());
            if (m_distance < min_distance) {
                min_distance = m_distance;
                matched_ID = key2;
            }
        }
        this->sample_pair[key1] = matched_ID;
    }

    std::cout << "finish" << std::endl;
    
}
// this is the 6.4 part of the paper
void image_data::optimize_output()
{
    int key = 0;
    for (auto& [output_key,sample_key] : this->sample_pair) {

        vec2 output_pos = this->output_sample_data[output_key]->position;
        vec2 sample_pos = this->sample_data[sample_key]->position;
        vec2 distanceVec;
        vec2 new_pos;
        
        for (auto&[value,distance]: this->sample_data[sample_key]->neighborhood) {
            distanceVec = this->sample_data[value]->position- sample_pos;
            new_pos = output_pos + distanceVec;
            /*
            std::unique_ptr<sample> new_sample = std::make_unique<sample>();
            new_sample->position = new_pos;
            new_sample->index = key;
            //std::cout << "x: " << distance_from_center.x << "y: " << distance_from_center.y << std::endl;
            new_sample->cluster_ID = 0;

            this->final_output_sample_data[key] = std::move(new_sample);*/

            this->final_output_sample_data[this->output_sample_data[output_key]->cluster_ID].push_back(new_pos);
            /*
            
            if (this->final_output_sample_data.find(this->output_sample_data[output_key]->cluster_ID)!= final_output_sample_data.end()) {

                this->final_output_sample_data[this->output_sample_data[output_key]->cluster_ID].push_back(new_pos);

            }
            else {
                this->final_output_sample_data[this->output_sample_data[output_key]->cluster_ID] = std::vector(new_pos);
            }
            */
            //this->final_output_sample_data.push_back(new_pos);

            key += 1;

        }

    }






}

