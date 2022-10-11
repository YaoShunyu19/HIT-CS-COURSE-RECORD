package exercise_3_6;

import java.util.Map;

/**
 * A mutable ADT
 */
public interface ParkingField {
	
	/**
	 * 创建一个新的停车场
	 * 
	 * @param nos 各停车位的编号，均为自然数，且无重复
	 * @param widths 各停车位的宽度，包含的元素数量=nos中元素数量 且>=5 
	 * @return 一个停车场对象，包含了widths.length个车位，各车位的宽度与nos中相应数字一致，且各车位上均未有停车
	 * @throws 如果违反nos和widths不合法
	 */
	public static ParkingField create(int[] nos, int[] widths) throws Exception {
		return new ConcreteParkingField(nos, widths);
	}
	
	/**
	 * 创建一个新的停车场
	 * 
	 * @param lots key是车位编号（自然数），value是车位宽度（自然数），
	 * lots长度>=5，
	 * @return 一个停车场对象，包含了lots.size()个车位，各车位的编号与宽度与lots中的KV一致，且各车位上均未有停车
	 * @throws 如果lots不合法
	 */
	public static ParkingField create(Map<Integer, Integer> lots) throws Exception {
		return new ConcreteParkingField(lots);
	}
	
	/**
	   * 在某个停车位上停车
	   * 车牌号为plate的车辆，之前没停在车场，执行后停在了车位号为num的车位上，该车位宽度大于车宽度
	   * 其他车位的状态不变
	 * 
	 * @param plate  要停进来的车辆车牌号，not null
	 * @param width  车的宽度，自然数
	 * @param num 	  指定的停车位编号，自然数
	 * @throws 如果plate车已经停在该停车场，
	 * 或者num车位已被其他车占用，
	 * 或者num车位宽度不超过width，
	 * 或者num并不是合法车位
	 */
	public void parking(String plate, int width, int num) throws Exception;

	/**
	   * 在停车场停车，自动分配空闲停车位
	   * 车牌号为plate的车辆，之前没停在车场，执行后停在了一个之前空闲的车位上，该车位宽度大于车宽度
	   * 其他车位的状态不变
	 * 
	 * @param plate 要停进来的车辆车牌号
 	 * @param width  车的宽度，自然数
 	 * @throws 如果plate车已经停在该停车场，或者该车场已经没有超过width的空闲车位
	 */
	public void parking(String plate, int width);

	/**
	   * 将汽车驶离停车场，plate车原来占用的车位空出来了，计算出本次停车的费用（半小时10元，不足半小时按半小时计算）
	 * 
	 * @param plate 待驶离的车辆，not null
	 * @return 本次停车的费用（精确计算得到）
	 * @throws plate车并没有停在本车场
	 */
	public double depart(String plate) throws Exception;

	/**
	   * 返回当前停车场每个车位的状态（空，或被某车占用）
	 * 
	 * @return Key为停车位的编号，Value为该车位上的车辆车牌号。如果停车位上无车辆，则对应的Value为“”
	 */
	public Map<Integer, String> status();
	
	/**
	 * 获得车场中总的车位数目
	 * 
	 * @return 车场中总的车位数目
	 */
	public int getNumberOfLots();
	
	/**
	 * 看车场中是否存在指定车位号和宽度的车位
	 * 
	 * @return true如果存在；false如果不存在
	 */
	public boolean isLotInParkingField(int num, int width);
	
	/**
	 * 车场目前是否为空（没有车停）
	 * 
	 * @return true 如果空；false，否则
	 */
	public boolean isEmpty();

	/**
	 * 返回某车位宽度
	 *
	 * @param num 车位号
	 * @return 该车位宽度
	 * @throws 如果num是非法车位
	 */
	int getLotWidth(int num) throws Exception;
}
