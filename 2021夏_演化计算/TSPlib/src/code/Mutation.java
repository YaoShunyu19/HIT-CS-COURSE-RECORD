package code;

import java.util.Collections;
import java.util.List;
import java.util.Random;

public class Mutation {

	public static Individual insert(Individual child) {
		List<Integer> dna=child.get_dna();
		Random r=new Random();
		int r1 = r.nextInt(dna.size());
		int r2=-1;
		while (r2==-1 || r2==r1) r2=r.nextInt(dna.size());
		if (r2<r1) {
			int temp=r1;
			r1=r2;
			r2=temp;
		}
		int temp=dna.get(r1);
		dna.remove(r1);
		dna.add(r2, temp);
		return new Individual(dna);
	}
	
	public static Individual swap(Individual child) {
		List<Integer> dna=child.get_dna();
		Random r=new Random();
		int r1 = r.nextInt(dna.size());
		int r2=-1;
		while (r2==-1 || r2==r1) r2=r.nextInt(dna.size());
		int temp=dna.get(r1);
		dna.set(r1, dna.get(r2));
		dna.set(r2, temp);
		return new Individual(dna);
	}
	
	public static Individual inversion(Individual child) {
		List<Integer> dna=child.get_dna();
		Random r=new Random();
		int r1 = r.nextInt(dna.size());
		int r2=-1;
		while (r2==-1 || r2==r1) r2=r.nextInt(dna.size());
		if (r2<r1) {
			int temp=r1;
			r1=r2;
			r2=temp;
		}
		List<Integer> part=dna.subList(r1, r2+1);
		Collections.reverse(part);
		for (int i=r1;i<=r2;i++) {
			dna.set(i, part.get(i-r1));
		}
		return new Individual(dna);
	}
	
	public static Individual scramble(Individual child) {
		List<Integer> dna=child.get_dna();
		Random r=new Random();
		int r1 = r.nextInt(dna.size());
		int r2=-1;
		while (r2==-1 || r2==r1) r2=r.nextInt(dna.size());
		if (r2<r1) {
			int temp=r1;
			r1=r2;
			r2=temp;
		}
		List<Integer> part=dna.subList(r1, r2+1);
		Collections.shuffle(part);
		for (int i=r1;i<=r2;i++) {
			dna.set(i, part.get(i-r1));
		}
		return new Individual(dna);
	}

}
