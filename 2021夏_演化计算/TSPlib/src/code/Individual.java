package code;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Individual {

	private final List<Integer> dna=new ArrayList<>();
	
	public Individual(int dna_size) {
		for (int i=1;i<=dna_size;i++) {  // 初始化一个排列，从1到城市的个数
			dna.add(i);
		}
		Collections.shuffle(dna);
	}
	
	public Individual(List<Integer> dna) {
		for (int i=0;i<dna.size();i++) {  // 根据输入的list初始化一个排列
			this.dna.add(dna.get(i));
		}
	}
	
	public int get_dna_size() {
		return this.dna.size();
	}
	
	public List<Integer> get_dna() {
		List<Integer> ret=new ArrayList<>();
		for (int i=0;i<this.dna.size();i++) ret.add(this.dna.get(i));
		return ret;  // 返回这个个体的dna
	}
	
	@Override
	public boolean equals(Object o) {
		if (o==null) return false;
		if (!(o instanceof Individual)) return false;
		Individual i=(Individual)o;
		return (i.get_dna().equals(this.dna));
	}
	
	@Override
	public String toString() {
		return this.dna.toString();
	}

}
