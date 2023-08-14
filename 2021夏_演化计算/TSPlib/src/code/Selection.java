package code;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;


public class Selection {

    public static Population fitnessProportionate(GA ga,Population pop,int pop_size) {
    	int More_pop_size=pop.get_pop_size();
        List<Individual> ret=new ArrayList<>();
    	List<Individual> inds=pop.getIndividuals();
    	//从More_pop_size个个体中选择pop_size个
        
        List<Double> fitness=new ArrayList<>();
        double fitness_sum=0;
        for (Individual ind : pop.getIndividuals()) {
        	fitness.add(ga.get_fitness(ind));
        	fitness_sum+=ga.get_fitness(ind);
        } 
        
        List<Double> Probability=new ArrayList<>();
        for(int i=0; i<More_pop_size;i++) {
        	Probability.add(fitness.get(i)/fitness_sum);
        }
        
        List<Double> one=new ArrayList<>();
        List<Boolean> vis=new ArrayList<>();
        for (int i=0;i<More_pop_size;i++) {
        	if (i==0) one.add(Probability.get(i));
        	else one.add(one.get(i-1)+Probability.get(i));
        	vis.add(false);
        }
        Random ran=new Random();
        while (ret.size()<pop_size) {
        	double r=ran.nextDouble();
        	int choose=0;
        	for (int i=0;i<More_pop_size;i++) {
        		if (i==More_pop_size-1) choose=i;
        		else {
        			if (r>=one.get(i) && r<one.get(i+1)) {
        				choose=i;
        				break;
        			}
        		}
        	}
        	if (vis.get(choose)==true) continue;
        	vis.set(choose, true);
        	Individual n=inds.get(choose);
        	ret.add(new Individual(n.get_dna()));
        }

        return new Population(ret);
    }

    public static Population tournamentSelection(GA ga,Population pop,int pop_size,int sampleNum) {
    	if (sampleNum>pop_size || sampleNum<1) {
    		System.out.println("wrong!");
    		return null;
    	}
        int More_pop_size = pop.get_pop_size();
        List<Individual> ret = new ArrayList<>();
        List<Individual> parents = pop.getIndividuals();
        List<Individual> candidates = new ArrayList<>();
        Random r=new Random();
        
        while (ret.size()<pop_size) {
            candidates.clear();
            
            do {
                Individual candidate = parents.get(r.nextInt(More_pop_size));
                if(!candidates.contains(candidate)) {
                    candidates.add(candidate);
                }
            } while (candidates.size() != sampleNum);
            
            Individual bestCandidate = candidates.get(0);
            double leastDistance = Double.MAX_VALUE;
            for(Individual candidate : candidates) {
                double distance = ga.get_sum_distance(candidate);
                if(distance < leastDistance) {
                    bestCandidate = candidate;
                    leastDistance = distance;
                }
            }
            ret.add(bestCandidate);
        }

        return new Population(ret);
    }
    
    public static Population elitismSelection(GA ga,Population pop,int pop_size){
    	
    	double maxFitness=0;
    	Individual bestind=pop.getIndividuals().get(0);
    	for (Individual i : pop.getIndividuals()) {
    		if (ga.get_fitness(i)>maxFitness) {
    			maxFitness=ga.get_fitness(i);
    			bestind=i;
    		}
    	}
    	pop.deleteIndividual(bestind);
    	
    	Population p=fitnessProportionate(ga,pop,pop_size-1);
    	p.addIndividual(bestind);
    	return p;

    }
}
