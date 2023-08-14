package code;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.Set;

public class Crossover {

	// 用OrderCrossover来产生装有两个后代个体的列表
	public static List<Individual> OrderCrossover(Individual p1,Individual p2) {
		List<Individual> ret=new ArrayList<>();
		int dna_size=p1.get_dna_size();
		Random r=new Random();
		int r1 = r.nextInt(dna_size);
		int r2=-1;
		while (r2==-1 || r2==r1) r2=r.nextInt(dna_size);
		if (r2<r1) {
			int temp=r1;
			r1=r2;
			r2=temp;
		}
		List<Integer> part1=p1.get_dna().subList(r1, r2+1);
		List<Integer> part2=p2.get_dna().subList(r1, r2+1);
		List<Integer> ret1=new ArrayList<>();
		List<Integer> ret2=new ArrayList<>();
		for (int i=0;i<dna_size;i++) {
			 ret1.add(0);
			 ret2.add(0);
		}
		for (int i=r1;i<=r2;i++) {
			ret1.set(i, part1.get(i-r1));
			ret2.set(i, part2.get(i-r1));
		}
		int i1=r2+1,i2=r2+1;  // i1指示p2的dna，i2指示ret
		while (ret1.contains(0)==true) {
			if (i1==dna_size) i1=0;
			if (i2==dna_size) i2=0;
			if (ret1.contains(p2.get_dna().get(i1))) {
				i1++;
				continue;
			}
			ret1.set(i2, p2.get_dna().get(i1));
			i1++;
			i2++;
		}
		i1=r2+1;i2=r2+1;  // i1指示p1的dna，i2指示ret
		while (ret2.contains(0)==true) {
			if (i1==dna_size) i1=0;
			if (i2==dna_size) i2=0;
			if (ret2.contains(p1.get_dna().get(i1))) {
				i1++;
				continue;
			}
			ret2.set(i2, p1.get_dna().get(i1));
			i1++;
			i2++;
		}
		ret.add(new Individual(ret1));
		ret.add(new Individual(ret2));
		return ret;
	}
	
	// 用PMXCrossover来产生装有两个后代个体的列表
	public static List<Individual> PMXCrossover(Individual p1,Individual p2) {
		List<Individual> ret=new ArrayList<>();
		int dna_size=p1.get_dna_size();
		Random r=new Random();
		int r1 = r.nextInt(dna_size);
		int r2=-1;
		while (r2==-1 || r2==r1) r2=r.nextInt(dna_size);
		if (r2<r1) {
			int temp=r1;
			r1=r2;
			r2=temp;
		}

		List<Integer> ret1=p1.get_dna();
		List<Integer> ret2=p2.get_dna();
		
		List<Integer> part1=new ArrayList<>();
		List<Integer> part2=new ArrayList<>();

		for (int i=r1;i<=r2;i++) {
			part1.add(ret1.get(i));
			part2.add(ret2.get(i));
		}

		for (int i=r1;i<=r2;i++) {
			ret1.set(i, part2.get(i-r1));
			ret2.set(i, part1.get(i-r1));
		}

		for (int i=r2+1;i!=r1;i++) {
			if (i==dna_size) {
				i=-1;
				continue;
			}
			int re=ret1.get(i);
			if (ret1.indexOf(re)==ret1.lastIndexOf(re)) continue; //该元素仅出现一次
			int oth=part1.get(part2.indexOf(re));
			
			if (ret1.contains(oth)==false) {
				ret1.set(i, oth);
				continue;
			}
			while (ret1.contains(oth)==true) {
				oth=part1.get(part2.indexOf(oth));
			}
			
			ret1.set(i, oth);
		}
		for (int i=r2+1;i!=r1;i++) {
			if (i==dna_size) {
				i=-1;
				continue;
			}
			int re=ret2.get(i);
			if (ret2.indexOf(re)==ret2.lastIndexOf(re)) continue; //该元素仅出现一次
			int oth=part2.get(part1.indexOf(re));//TODO
			
			if (ret2.contains(oth)==false) {
				ret2.set(i, oth);
				continue;
			}
			while (ret2.contains(oth)==true) {
				oth=part2.get(part1.indexOf(oth));
			}
			
			ret2.set(i, oth);
		}

		ret.add(new Individual(ret1));
		ret.add(new Individual(ret2));
		return ret;
	}
	
	// 用CycleCrossover来产生装有两个后代个体的列表
	public static List<Individual> CycleCrossover(Individual p1,Individual p2) {
		List<Individual> ret=new ArrayList<>();
		List<Integer> l1=Collections.unmodifiableList(p1.get_dna());
		List<Integer> l2=Collections.unmodifiableList(p2.get_dna());
		List<Boolean> ifCycle=new ArrayList<>();
		List<Integer> ret1=new ArrayList<>();
		List<Integer> ret2=new ArrayList<>();
		for (int i=0;i<l1.size();i++) {
			ifCycle.add(false);
			ret1.add(0);
			ret2.add(0);
		}
		int flag=0;
		while (ret1.contains(0)==true) {
			if (flag%2==0) {
				
				List<Integer> cycle=getCycle(l1,l2,ifCycle);
				for (int i=0;i<cycle.size();i++) {
					ret1.set(l1.indexOf(cycle.get(i)), cycle.get(i));
					ret2.set(l2.indexOf(cycle.get(i)), cycle.get(i));
				}
			}
			else {
				List<Integer> cycle=getCycle(l2,l1,ifCycle);
				for (int i=0;i<cycle.size();i++) {
					ret2.set(l1.indexOf(cycle.get(i)), cycle.get(i));
					ret1.set(l2.indexOf(cycle.get(i)), cycle.get(i));
				}
			}
			flag++;
		}
		ret.add(new Individual(ret1));
		ret.add(new Individual(ret2));
		return ret;
	}
	
	// 用EdgeRecombination来产生装有  一个  后代个体的列表  注意！是一个！
	public static List<Individual> EdgeRecombination(Individual p1,Individual p2) {
		List<Individual> ret=new ArrayList<>();
		List<Integer> l1=p1.get_dna();
		List<Integer> l2=p2.get_dna();
		int dna_size=p1.get_dna_size();
		List<Map<Integer,Boolean>> chart=getChart(l1,l2);
		List<Integer> ret1=new ArrayList<>();
		Random r=new Random();
		int choice=l1.get(r.nextInt(dna_size));
		ret1.add(choice);
		for (int i=0;i<chart.size();i++) {
			chart.get(i).remove(choice);
		}
		for (int i=1;i<dna_size;i++) {
			Set<Integer> edges=chart.get(choice).keySet();
			int c=choice;
			int maxedgesnum=Integer.MAX_VALUE;
			for (int integer : edges) {
				if (chart.get(c).get(integer)==true) {
					choice=integer;
					break;
				}
				if (chart.get(integer).size()<maxedgesnum) {
					choice=integer;
					maxedgesnum=chart.get(integer).size();
				}
			}
			ret1.add(choice);
			for (int j=0;j<chart.size();j++) {
				chart.get(j).remove(choice);
			}
		}
		ret.add(new Individual(ret1));
		return ret;
	}
	
	//给定两个个体的dna，根据每个等位基因所在的圈是否已经找出的情况找出一个新的圈
	public static List<Integer> getCycle(List<Integer> l1,List<Integer> l2,List<Boolean> ifCycle) {
		List<Integer> ret=new ArrayList<>();
		for (int i=0;i<l1.size();i++) {
			if (ifCycle.get(l1.get(i)-1)==true) continue;
			Integer index=i;
			while (true) {
				Integer j=l1.get(index);
				if (ret.size()>0 && ret.get(0)==j) break;
				ifCycle.set(j-1, true);
				ret.add(j);
				index=l1.indexOf(l2.get(index));
			}
			break;
		}
		return ret;
	}
	
	//给定两个个体的dna，构建EdgeRecombination里面的表
	private static List<Map<Integer,Boolean>> getChart(List<Integer> l1,List<Integer> l2) {
		List<Map<Integer,Boolean>> ret=new ArrayList<>();
		int dna_size=l1.size();
		for (int i=0;i<=dna_size;i++) ret.add(new HashMap<Integer,Boolean>());//第一个元素占位，无实际意义
		for (int i=0;i<dna_size;i++) {
			int target=l1.get(i);
			int index1=l1.indexOf(target);
			int index2=l2.indexOf(target);
			int left1=(index1-1>=0)?(index1-1):(dna_size-1);
			int right1=(index1+1<dna_size)?(index1+1):0;
			int left2=(index2-1>=0)?(index2-1):(dna_size-1);
			int right2=(index2+1<dna_size)?(index2+1):0;
			Map<Integer,Boolean> m=ret.get(target);
			if (m.keySet().contains(l1.get(left1))) m.put(l1.get(left1), true);
			else m.put(l1.get(left1), false);
			if (m.keySet().contains(l1.get(right1))) m.put(l1.get(right1), true);
			else m.put(l1.get(right1), false);
			if (m.keySet().contains(l2.get(left2))) m.put(l2.get(left2), true);
			else m.put(l2.get(left2), false);
			if (m.keySet().contains(l2.get(right2))) m.put(l2.get(right2), true);
			else m.put(l2.get(right2), false);
		}
		return ret;
	}
	 
}
