//
//  main.cpp
//  Proj5-Particle_Swarm_Optimization
//
//  Created by Kelsey Kelley on 4/18/19.
//  Copyright © 2019 Kelsey Kelley. All rights reserved.
//

#include <cmath>
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;

double mdist(double width, double height){
    return sqrt(pow(width,2.0)) + pow(height,2.0)/2.0;
}

double pdist(double posx, double posy){
    return sqrt(pow(posx-20.0, 2.0)+pow(posy-7.0,2.0));
}

double ndist(double xp, double yp){
    return sqrt(pow(xp + 20.0, 2.0) + pow(yp + 7.0, 2.0));
}

double Q1(double xp, double yp, double width, double height){
    return 100.0 * (1.0 - (pdist(xp, yp) / mdist(width, height)));
}

double Q2(double xp, double yp, double width, double height){
    double val;
    
    val = 10 - pow(pdist(xp, yp), 2.0);
    
    if(val > 0){
        return 9 * val + 10 * (1 - (pdist(xp, yp) / mdist(width, height))) + 70 * (1 - (ndist(xp, yp) / mdist(width, height)));
    }
    
    return 9 * 0 + 10 * (1 - (pdist(xp, yp) / mdist(width, height))) + 70 * (1 - (ndist(xp, yp) / mdist(width, height)));
}

int main(int argc, char *argv[]){
    int iterations, particles, maxvelocity, count;
    double inertia, cognition, social, xerr, yerr, width, height, xgb, ygb;
    FILE *fp;
    
   /* if(argc != 6){
        fprintf(stderr, "You need 5 arguments: # iterations, # particles, inertia, cognition param, social param\n");
        exit(0);
    }*/
    
    iterations = 1000;
    particles = 40;
    inertia = 0.8;
    cognition = 2.4;
    social = 1.8;
    width = 100.0;
    height = 100.0;
    maxvelocity = 10;
    
    
    fp = fopen("Qual2mvc>serror.csv", "ab+");
    fprintf(fp, "# particles: %d, Inertia: %f, Cognition: %f, Social: %f, Max velocity: %d\n\n", particles, inertia, cognition, social, maxvelocity);
    
    vector<double> pos_x;
    vector<double> pos_y;
    vector<double> vel_x;
    vector<double> vel_y;
    vector<double> pbx;
    vector<double> pby;
    
    pos_x.resize(particles,0);
    pos_y.resize(particles,0);
    vel_x.resize(particles,0);
    vel_y.resize(particles,0);
    pbx.resize(particles,0);
    pby.resize(particles,0);
    
    xgb = 0.0;
    ygb = 0.0;
    
    //Initialize all particles' position, velocity, personal best, and global best
    srand((unsigned)time(NULL));
    for(int i = 0; i < particles; i++){
        pos_x[i] = (rand() % (101)) - 50.0;
        pos_y[i] = (rand() % (101)) - 50.0;
        vel_x[i] = 0.0;
        vel_y[i] = 0.0;
        pbx[i] = (double)i;
        pby[i] = (double)i;
        if(Q2(pos_x[i], pos_y[i], width, height) < Q2(xgb, ygb, width, height)){
            xgb = pos_x[i];
            ygb = pos_y[i];
        }
    }
    
    count = 0;
    xerr = 1.0;
    yerr = 1.0;
    
    //Runs until error in x and y coordinates drops below 0.01 or until given number of iterations
    while(xerr > 0.01 && yerr > 0.01){
        for(int i = 0; i < particles; i++){
            //Update velocity
            vel_x[i] = inertia * vel_x[i] + cognition * (rand() % 10000 / 10000.0) * (pbx[i] - pos_x[i]) + social * (rand() % 10000 / 10000.0) * (xgb - pos_x[i]);
            vel_y[i] = inertia * vel_y[i] + cognition * (rand() % 10000 / 10000.0) * (pby[i] - pos_y[i]) + social * (rand() % 10000 / 10000.0) * (ygb - pos_y[i]);
            
            //Scale velocity
            if(pow(vel_x[i], 2.0) + pow(vel_y[i], 2.0) > pow(maxvelocity, 2.0)){
                vel_x[i] = (maxvelocity / sqrt(pow(vel_x[i], 2.0) + pow(vel_y[i], 2.0))) * vel_x[i];
                vel_y[i] = (maxvelocity / sqrt(pow(vel_y[i], 2.0) + pow(vel_y[i], 2.0))) * vel_y[i];
            }
            
            //Update position
            pos_x[i] = pos_x[i] + vel_x[i];
            pos_y[i] = pos_y[i] + vel_y[i];
            
            //Update personal best
            if(Q1(pos_x[i], pos_y[i], width, height) > Q2(pbx[i], pby[i], width, height)){
                pbx[i] = pos_x[i];
                pby[i] = pos_y[i];
            }
            
            //Update global best
            if(Q1(pos_x[i], pos_y[i], width, height) > Q2(xgb, ygb, width, height)){
                xgb = pos_x[i];
                ygb = pos_y[i];
            }
            
            //Calculate average err in x and y coordinates
            xerr += pow(pos_x[i] - xgb, 2.0);
            yerr += pow(pos_y[i] - ygb, 2.0);
        }
        
        /*fprintf(fp, "Particle:, Position x:, Position y:\n");
        for(int i = 0; i < particles; i++){
            fprintf(fp, "%d, %f, %f\n", i, pos_x[i], pos_y[i]);
        }*/
        
        xerr = sqrt((1.0 / (2.0 * particles)) * xerr);
        yerr = sqrt((1.0 / (2.0 * particles)) * yerr);
        
        //fprintf(fp, " , Error in x:, Error in y:\n");
        fprintf(fp, "%d, %f, %f\n", count, xerr, yerr);
        
        if(count == iterations){
            break;
        }
        count++;
    }
    
    /*fprintf(fp, " , Global x:, Global y:\n");
    fprintf(fp, " , %f, %f\n\n", xgb, ygb);
    fprintf(fp, "Particle:, Position x:, Position y:\n");
    for(int i = 0; i < particles; i++){
        fprintf(fp, "%d, %f, %f\n", i, pos_x[i], pos_y[i]);
    }
    fprintf(fp, "\n************, ***************, ******************, *************, ***********\n\n");*/
    
    fclose(fp);
    
    return 0;
}


