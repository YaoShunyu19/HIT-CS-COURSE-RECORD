package code;

import java.util.List;

public interface GA {

	public void run();
	
	public List<Integer> get_opt_list();
	
	public double get_opt_distance();
	
	public double get_sum_distance(Individual ind);
	
	public double get_fitness(Individual ind);
}
