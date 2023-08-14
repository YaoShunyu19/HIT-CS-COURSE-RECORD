package code;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class GA2 implements GA{

	private final TSPproblem tp;
	private final int pop_size;
	private final int gen_size;
	private final double p_cross;
	private final double p_mutate;
	private List<Integer> opt_List;
	private double opt_Distance=Double.MAX_VALUE;
	
	public GA2(TSPproblem tp,int pop_size,int gen_size,double p_cross,double p_mutate) {
		this.tp=tp;
		this.pop_size=pop_size;
		this.gen_size=gen_size;
		this.p_cross=p_cross;
		this.p_mutate=p_mutate;
	}

	@Override
	public void run() {
		Population pop=new Population(this.pop_size,tp.getCityNum());//随机生成一个种群
		Random r=new Random();
		List<Double> gen_opt_distance=new ArrayList<>();
		List<List<Integer>> gen_opt_list=new ArrayList<>();
		List<Individual> child1=new ArrayList<>();
		List<Individual> child=new ArrayList<>();
		
		//第一层循环表示迭代代数
		for (int i=0;i<gen_size;i++) {
			Population pop_copy=pop.copy();
			
			//第二层循环表示对每一个个体进行一定概率的交叉和变异
			for (int k=0;k<pop_copy.getIndividuals().size();k++) {
				Individual parent=pop_copy.getIndividuals().get(k);
				child1.clear();
				
				//生成随机数，根据概率进行交叉
				double pc=r.nextDouble();
				if (pc<this.p_cross) {
					int p=r.nextInt(pop_size);
					int choose=r.nextInt(4);
					switch (choose) {
					case 0: 
						child1=Crossover.OrderCrossover(parent, pop_copy.getIndividuals().get(p));
						child.addAll(child1);
						break;
					case 1:
						child1=Crossover.PMXCrossover(parent, pop_copy.getIndividuals().get(p));
						child.addAll(child1);
						break;
					case 2:
						child1=Crossover.CycleCrossover(parent, pop_copy.getIndividuals().get(p));
						child.addAll(child1);
						break;
					case 3:
						//child1=Crossover.EdgeRecombination(parent, pop_copy.getIndividuals().get(p));
						child.addAll(child1);
						break;
					}

				}

				//生成随机数，根据概率进行变异
				double pm=r.nextDouble();
				if (pm<this.p_mutate) {
					for (int j=0;j<child.size();j++) {
						int choose=r.nextInt(4);
						switch (choose) {
						case 0: 
							child.set(j, Mutation.insert(child.get(j)));
							break;
						case 1:
							child.set(j, Mutation.swap(child.get(j)));
							break;
						case 2:
							child.set(j, Mutation.inversion(child.get(j)));
							break;
						case 3:
							child.set(j, Mutation.scramble(child.get(j)));
							break;
						}
						
					}
				}
				
				//将子代加入整个种群中
				pop.addIndividuals(child);
				child.clear();
			}

			//轮盘赌选择算法（根据适应度）
			pop=Selection.fitnessProportionate(this, pop, this.pop_size);
			
			//更新当前最优解
			Individual bestIndividual=pop.getIndividuals().get(0);
			double bestDistance=Double.MAX_VALUE;
			for (Individual ind : pop.getIndividuals()) {
				double ts=this.get_sum_distance(ind);;
				if (ts<bestDistance) {
					bestIndividual=ind;
					bestDistance=ts;
				}
			}
			
			if (i==0) this.opt_List=bestIndividual.get_dna();
			if (this.opt_Distance>bestDistance) {
				this.opt_List=bestIndividual.get_dna();
				this.opt_Distance=bestDistance;
			}
			
			if((i+1)%200==0 || true) {
				System.out.println("generation "+(i+1)+": total distance: "+this.opt_Distance);
				System.out.println("     permutation: "+this.opt_List);
				System.out.println();
				if ((i+1)==5000 || (i+1)==10000 || (i+1)==20000) {
					gen_opt_distance.add(this.opt_Distance);
					gen_opt_list.add(this.opt_List);
				}
			}
			
			
		}
		for (int i=0;i<gen_opt_distance.size();i++) {
			System.out.println(gen_opt_distance.get(i));
			System.out.println(gen_opt_list.get(i));
		}
	}

	@Override
	public List<Integer> get_opt_list() {
		List<Integer> ret=new ArrayList<>();
		for (Integer i : this.opt_List) {
			ret.add(i);
		}
		return ret;
	}

	@Override
	public double get_opt_distance() {
		return this.opt_Distance;
	}
	
	@Override
	public double get_sum_distance(Individual ind) {
		double ret=0;
		for (int i=0;i<ind.get_dna_size()-1;i++) {
			ret+=this.tp.get_distance(ind.get_dna().get(i), ind.get_dna().get(i+1));
		}
		return ret;
	}
	
	@Override
	public double get_fitness(Individual ind) {
		return (double)1/this.get_sum_distance(ind);
	}
}
