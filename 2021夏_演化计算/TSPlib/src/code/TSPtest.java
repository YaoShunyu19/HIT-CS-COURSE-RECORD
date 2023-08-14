package code;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class TSPtest {

	/**
	 * Rep:
	 * totalDistance:���Ž���������ܳ��ȣ�
	 * optiList:���Ž����Ӧ�ĳ��з���˳��
	 */
	private final double totalDistance; 
	private final List<Integer> opt_List;//��0��ʼ����
	
	/*
	 * ʹ��������ʽ��ȡ�ļ��е���Ϣ
	 * ���ڳ�ʼ�����Ž�������Ӧ�ĳ��з�������
	 */
	public TSPtest(String filename) throws IOException
	{
		BufferedReader reader = new BufferedReader(new FileReader(filename));
		String line = reader.readLine().trim();
		
		Pattern p = Pattern.compile("^COMMENT");
		Matcher m = p.matcher(line);
		while(!m.find())
		{
			line = reader.readLine().trim();
			m = p.matcher(line);
		}
		
		p= Pattern.compile("\\d+(?=\\))");
		m = p.matcher(line);
		m.find();
		String dis = m.group();
		totalDistance = Double.valueOf(dis);
		
		p = Pattern.compile("^[0-9]+");
		m = p.matcher(line);
		while(!m.find())
		{
			line = reader.readLine().trim();
			m = p.matcher(line);
		}
		opt_List = new ArrayList<>();
		while(!line.equals("-1"))
		{
			opt_List.add(Integer.valueOf(line));
			line = reader.readLine().trim();
		}
		reader.close();
	}
	
	/*
	 * �鿴���Ž��
	 */
	public double get_opt_Distance()
	{
		return this.totalDistance;
	}
	
	/*
	 * ������Ž������Ӧ�ĳ�������
	 */
	public List<Integer> get_opt_List()
	{
		return Collections.unmodifiableList(this.opt_List);
	}
	
}
