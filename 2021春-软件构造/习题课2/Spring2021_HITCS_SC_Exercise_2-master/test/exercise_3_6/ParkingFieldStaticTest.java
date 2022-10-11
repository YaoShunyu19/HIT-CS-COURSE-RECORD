package exercise_3_6;

import static org.junit.Assert.*;

import java.util.HashMap;
import java.util.Map;

import org.junit.Test;

/**
 * 测试ParkingField的静态方法create()
 *
 */
public class ParkingFieldStaticTest {

	// Testing strategy
	// 测试对象：create(Map<Integer, Integer> lots)
	//	按车位数量划分：=0，<5, =5, >5
	//	按车位号划分：全部自然数、包含非自然数（0或负数）
	//	按车位宽度划分：全部自然数、包含0或负数
	//	特殊情况：车位宽度都一样、车位宽度不一样 //非必须
	// 使用“每个取值至少覆盖一次”策略  或  “笛卡尔积全覆盖”策略

	@Test(expected = AssertionError.class)
	public void testAssertionsEnabled() {
		assert false; // make sure assertions are enabled with VM argument: -ea
	}
	
	//覆盖车位数目=0
	@Test(expected = Exception.class)
	public void testZeroLots() throws Exception {
		Map<Integer, Integer> lots = new HashMap<>();
		ParkingField pf = ParkingField.create(lots);
	}
	
	//覆盖车位数目<5
	// 	<1,10><2,10><3,10><4,10>
	@Test(expected = Exception.class)
	public void testLessThanFiveLots() throws Exception {
		Map<Integer, Integer> lots = new HashMap<>();
		lots.put(1, 10);
		lots.put(2, 10);
		lots.put(3, 10);
		lots.put(4, 10);
		ParkingField pf = ParkingField.create(lots);
	}
	
	//覆盖车位数目=5
	// 	<1,10><2,10><3,10><4,10><5,10>
	@Test
	public void testFiveLots() throws Exception {
		//构造测试用例的输入数据
		Map<Integer, Integer> lots = new HashMap<>();
		lots.put(1, 10);
		lots.put(2, 10);
		lots.put(3, 10);
		lots.put(4, 10);
		lots.put(5, 10);
		//调用函数
		ParkingField pf = ParkingField.create(lots);
		//判断车位数是否正确
		assertEquals("Expected: 5", 5, pf.getNumberOfLots());
		//判断车位编号和尺寸是否包含
		for(Integer lot: lots.keySet()) 
			assertTrue(pf.isLotInParkingField(lot, lots.get(lot)));
		//判断是否停车
		assertTrue(pf.isEmpty());
	}
	
	// TODO 其他测试用例，请补充
}
