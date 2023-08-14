package code;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class TSPproblem {
	
	/**
	 * Rep:
	 * city_num:城市数量，即dna_size
	 * cityList:城市列表，包含城市序号信息及城市坐标（城市的序号自1开始）
	 * distance：距离矩阵，存储两城市间距离   城市个数+1 维
	 */
	private final int city_num;
	private final List<City> cityList=new ArrayList<City>();
	private final double distance[][];
	
	/*
	 * 使用正则表达式提取文件中的信息
	 * 用于初始化城市列表（cityList）及距离矩阵（distance）
	 */
	public TSPproblem(String filename) throws IOException
	{
		BufferedReader reader = new BufferedReader(new FileReader(filename));
		String line = reader.readLine().trim();
		
		Pattern p = Pattern.compile("^[0-9]");
		Matcher m = p.matcher(line);
		while(!m.find())
		{
			line = reader.readLine().trim();
			m = p.matcher(line);
		}
		cityList.add(new City(0,0,0));
		while(!line.equals("EOF"))
		{
			String[] list = line.split(" ");
			int num = Integer.valueOf(list[0]);
			double x = Double.valueOf(list[1]);
			double y = Double.valueOf(list[2]);
			City city = new City(num,x,y);
			cityList.add(city);
			line = reader.readLine().trim();
		}
		reader.close();
		
		this.city_num = cityList.size()-1;//城市数
		distance = new double[this.city_num+1][this.city_num+1];
		for(int i=0;i<this.city_num+1;i++)
		{
			distance[0][i] = 0;
			distance[i][0] = 0;
			distance[i][i] = 0;
		}
		for(int i=1;i<this.city_num+1-1;i++)
		{
			for(int j=i+1;j<this.city_num+1;j++)
			{
				distance[i][j] = calculateDistance(cityList.get(i),cityList.get(j));
				distance[j][i] = distance[i][j];
			}
		}
	}
	
	public double get_distance(int i,int j) {
		return this.distance[i][j];
	}
	
	public int getCityNum() {
		return this.city_num;
	}
	
	/*
	 * 计算两城市间距离
	 */
	private double calculateDistance(City a,City b)
	{
		double l = b.gety()-a.gety();
		double h = b.getx()-a.getx();
		return Math.sqrt(h*h+l*l);
	}
}

class City{
	
	int num;  //编号
	double x;  //横坐标
	double y;  //纵坐标
	
	public City(int num,double x,double y)
	{
		this.num = num;
		this.x = x;
		this.y = y;
	}
	
	public int getNum()
	{
		return num;
	}
	
	public double getx()
	{
		return x;
	}
	
	public double gety()
	{
		return y;
	}
	
}