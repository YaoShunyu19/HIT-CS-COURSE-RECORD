package code;

import java.util.ArrayList;
import java.util.List;


public class Population {
	
	private List<Individual> individuals = new ArrayList<Individual>();
	private int pop_size;
	
	public Population(int pop_size, int dna_size) {
		this.pop_size=pop_size;
		for (int i = 0; i < pop_size; i++) {
			individuals.add(new Individual(dna_size));
		}
	}

	public Population(List<Individual> individuals) {
		this.individuals = individuals;
		this.pop_size=individuals.size();
	}

	public List<Individual> getIndividuals() {
		List<Individual> ret=new ArrayList<>();
		for (Individual i : this.individuals) {
			ret.add(new Individual(i.get_dna()));
		}
		return ret;
	}
	
	public int get_pop_size() {
		return this.pop_size;
	}
	
	public void addIndividual(Individual i) {
		this.individuals.add(i);
		this.pop_size++;
	}
	
	public void addIndividuals(List<Individual> i) {
		for (Individual ind : i) {
			this.individuals.add(ind);
			this.pop_size++;
		}
	}
	
	
	public void deleteIndividual(Individual i) {
		this.pop_size--;
		for (int j=0;j<this.individuals.size();j++) {
			if (this.individuals.get(j).equals(i)) {
				this.individuals.remove(j);
				break;
			}
		}
	}
	
	public Population copy() {
		List<Individual> l=new ArrayList<>();
		for (Individual ind : this.individuals) {
			l.add(new Individual(ind.get_dna()));
		}
		return new Population(l);
	}
	
	@Override
	public String toString() {
		String s="";
		for (Individual i : this.individuals) s+=i.toString()+"\n";
		return s;
	}
	
}
