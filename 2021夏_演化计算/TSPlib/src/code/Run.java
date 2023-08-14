package code;

import java.util.ArrayList;
import java.util.List;

public class Run {

	private List<Integer> opt_List=new ArrayList<Integer>();
	private double opt_Distance=Double.MAX_VALUE;
	
	public Run() {}
	
	public void run(GA ga) {
		ga.run();
		this.opt_List=ga.get_opt_list();
		this.opt_Distance=ga.get_opt_distance();
	}
	
	public List<Integer> get_opt_List() {
		List<Integer> ret=new ArrayList<>();
		for (Integer i : this.opt_List) {
			ret.add(i);
		}
		return ret;
	}
	
	public double get_opt_Distance() {
		return this.opt_Distance;
	}
}
