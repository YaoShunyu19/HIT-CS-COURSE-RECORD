package exercise_3_6;

import static org.junit.Assert.*;

import java.util.HashMap;
import java.util.Map;

import org.junit.Test;

/**
 * 测试ParkingField的静态方法parking(String, int, int)
 *
 */

public class ParkingFieldInstanceTest {

	// Testing strategy
	// 测试对象：void parking(String plate, int width, int num)
	//  按plate划分：该车已经停在该停车场、该车未在停车
	//  按num划分：该车位是车场的合法车位、不是合法车位
	//  按num划分：该车位合法，已被其他车占用、未被占用
	//  按num和width划分：该车位宽度不超过plate车宽度，等于车宽度、大于车宽度
	// 使用“每个取值至少覆盖一次”策略 或 “笛卡尔积全覆盖”策略

	@Test(expected = AssertionError.class)
	public void testAssertionsEnabled() {
		assert false; // make sure assertions are enabled with VM argument: -ea
	}

	// 覆盖：车未在停车场、车位合法且未被占用，车宽=位宽
	@Test
	public void testLegalParking() throws Exception {
		// 创建车场
		Map<Integer, Integer> lots = new HashMap<>();
		lots.put(1, 10);
		lots.put(2, 15);
		lots.put(3, 20);
		lots.put(4, 20);
		lots.put(5, 20);
		ParkingField pf = ParkingField.create(lots);

		// 观察：车牌号为plate的车辆，之前没停在车场
		Map<Integer, String> beforeStatus = pf.status();
		assertTrue(!beforeStatus.containsValue("HA001"));

		// 停车
		pf.parking("HA001", 10, 1);

		// 观察：车牌号为plate的车辆停在了车位号为num的车位上
		Map<Integer, String> afterStatus = pf.status();
		assertEquals("HA001", afterStatus.get(1)); // 1为车位号

		// 观察：该车位宽度大于等于车宽度
		assertTrue(pf.getLotWidth(1) >= 10); // 1为车位号，10为车的宽度

		// 观察：其他车位的状态不变
		for (Integer lot : beforeStatus.keySet()) {
			if (lot.intValue() == 1)
				continue;
			assertTrue(beforeStatus.get(lot).equals(afterStatus.get(lot)));
		}
		
		System.out.println(pf.toString());
	}

	// 覆盖：车已在停车场、车位合法且未被占用
	@Test(expected = Exception.class)
	public void testCarAlreadyInParkingField() throws Exception {
		// 创建车场
		Map<Integer, Integer> lots = new HashMap<>();
		lots.put(1, 10);
		lots.put(2, 15);
		lots.put(3, 20);
		lots.put(4, 20);
		lots.put(5, 20);
		ParkingField pf = ParkingField.create(lots);

		// 停车：把HA001停在1号车位上，正常执行
		pf.parking("HA001", 10, 1);

		// 停车：把HA001停在2号车位上，应抛出异常，车已在停车场里
		pf.parking("HA001", 10, 2);
	}
}
