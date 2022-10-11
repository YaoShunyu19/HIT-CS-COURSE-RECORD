package exercise_5_4.client;

import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import exercise_5_4.decorator.ParkingFieldWithCompany;
import exercise_5_4.field.ParkingField;
import exercise_5_4.strategy.ParkingStrategy;
import exercise_5_4.strategy.RandomParkingStrategy;
import exercise_5_4.visitor.PercentageVisitor;

public class Client {

	public static void main(String[] args0) throws Exception {
		//读取语法文件
		List<ParkingField> pfs = TextFileReader.constructParkingFields("src\\exercise_5_4\\client\\example.txt");
		
		for (ParkingField pf : pfs) 
			System.out.println(pf);
		
		//创建车场
		Map<Integer, Integer> lots = new HashMap<>();
		lots.put(1, 10);
		lots.put(2, 15);
		lots.put(3, 20);
		lots.put(4, 20);
		lots.put(5, 20);
		
		ParkingField pf = ParkingField.create(lots);
		ParkingField pfwc = new ParkingFieldWithCompany(pf, "HIT_CS");
		pf.setParkingStrategy(new RandomParkingStrategy());

		//在调用parking之前为每个停车物构造疫情登记信息至args中
		pf.parking("car", "HA001", 10, new String[]{"Zhang San"}); //不指定车位停车
		pf.parking("motor", "HA002", 10, 2, new String[]{"Li Si"});
		pf.parking("plane", "HA003", 10, 3, new String[]{"Beijing", "2020-04-01"});
		pf.parking("car", "HA004", 10, 4, new String[]{"Wang Wu"});
		pf.parking("motor", "HA005", 10, new String[]{"Zhao Liu"}); //不指定车位停车
		System.out.println(pf);
		
		double ratio = pf.accept(new PercentageVisitor());

		Iterator<String> iterator = pf.iterator();
		while (iterator.hasNext()) {
			String c = iterator.next();
			System.out.println(c + " is now parked in the parking field");
		}
	}
}
