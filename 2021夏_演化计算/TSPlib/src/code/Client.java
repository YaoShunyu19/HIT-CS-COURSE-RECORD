package code;

import java.io.IOException;

public class Client {

	public static void main (String[] args) throws IOException{
		TSPproblem tp=new TSPproblem("EAfile/st70.tsp");//读入文件，初始化TSP问题
//		TSPtest tt=new TSPtest("../file/eil51.opt.tour");
		int pop_size=100;//种群规模
		int gen_size=20000;//迭代代数
		double p_cross=0.8;//交叉概率
		double p_mutate=0.6;//变异概率
//		GA ga=new GA1(tp,pop_size,gen_size,p_cross,p_mutate);//选择算法GA1
//		GA ga=new GA2(tp,pop_size,gen_size,p_cross,p_mutate);//选择算法GA2
		GA ga=new GA3(tp,pop_size,gen_size,p_cross,p_mutate);//选择算法GA3
		Run r=new Run();
		r.run(ga);
//		System.out.println(r.get_opt_Distance());
//		System.out.println(r.get_opt_List());
	}
	
}
