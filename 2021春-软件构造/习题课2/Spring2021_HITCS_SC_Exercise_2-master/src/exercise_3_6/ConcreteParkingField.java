package exercise_3_6;

import java.text.NumberFormat;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class ConcreteParkingField implements ParkingField {

	// Rep
	private final List<Lot> lots = new ArrayList<>(); // 一组车位
	private final Map<Lot, Car> status = new HashMap<>(); // 占用情况
	private final List<Record> records = new ArrayList<>(); // 停车记录

	// Rep invariants:
	// lots.size() >= 5;
	// lots.size() >= status.size();
	// status中的每个key均在lots中出现；
	// status中的values中不存在重复；
	// status中的value，该Car的宽度小于等于相应的key（车位）的宽度；
	// 对records中的每个Record对象，如果record.getTimeOut()为空，
	// 则<record.getLot(),record.getCar()>必定出现在status中

	// Abstraction function:
	// 代表着一个停车场，该停车场有lots.size()个车位
	// lots中每个元素l代表着一个车位，其编号和宽度是l.getNumber()和l.getWidth()
	// l上停的车是status.get(l)
	// 该车场的所有停车记录是records，其中的元素r表示一个停车记录，表明：
	// 车辆r.getCar()在r.getTimeIn()时刻停到了r.getLot()车位上，在r.getTimeOut()离开，花费r.getFee()元

	private void checkRep() {

		assert lots.size() >= 5;
		assert lots.size() >= status.size();

		Set<Car> parkingCars = new HashSet<>();
		for (Lot lot : status.keySet()) {
			Car car = status.get(lot);
			assert lots.contains(lot);
			assert !parkingCars.contains(car);
			parkingCars.add(car);
			assert lot.getWidth() >= car.getWidth();
		}

		for (Record record : records) {
			if (record.getTimeOut() == null) {
				assert status.containsKey(record.getLot());
				assert status.get(record.getLot()).equals(record.getCar());
			}
		}
	}

	/**
	 * 创建一个新的停车场
	 * 
	 * @param lots key是车位编号（自然数），value是车位宽度（自然数），lots长度>=5，
	 * @return 一个停车场对象，包含了lots.size()个车位，各车位的编号与宽度与lots中的KV一致，且各车位上均未有停车
	 * @throws 如果lots不合法
	 */
	public ConcreteParkingField(Map<Integer, Integer> lotsInfo) throws Exception {

		// check pre-condition, fail fast
		if (lotsInfo.size() < 5)
			throw new Exception();

		for (Map.Entry<Integer, Integer> entry : lotsInfo.entrySet()) {
			int num = entry.getKey();
			int width = entry.getValue();

			// check pre-condition, fail fast
			if (width <= 0)
				throw new Exception();

			// work
			Lot lot = new Lot(num, width);
			this.lots.add(lot);
		}

		// check post-condition
		assert this.getNumberOfLots() == lotsInfo.size();
		assert this.status.size() == 0;

		// check RI
		checkRep();
	}

	public ConcreteParkingField(int[] nos, int[] widths) {
		// TODO Auto-generated method stub
		checkRep();
	}

	/**
	 * 在某个停车位上停车 车牌号为plate的车辆，之前没停在车场，执行后停在了车位号为num的车位上，该车位宽度大于车宽度 其他车位的状态不变
	 * 
	 * @param plate 要停进来的车辆车牌号，not null
	 * @param width 车的宽度，自然数
	 * @param num   指定的停车位编号，自然数
	 * @throws 如果plate车已经停在该停车场，或者num车位已被其他车占用，或者num车位宽度不超过width，或者num并不是合法车位
	 */
	@Override
	public void parking(String plate, int width, int num) throws Exception {

		Car car = new Car(plate, width);

		// plate车已经停在该停车场
		if (status.containsValue(car))
			throw new Exception();

		// num车位已被其他车占用
		for (Lot lot : status.keySet())
			if (lot.getNumber() == num)
				throw new Exception();

		boolean legal = false;
		for (Lot lot : lots) {
			if (lot.getNumber() == num) {
				// num车位宽度不超过width
				if (lot.getWidth() < width)
					throw new Exception();
				legal = true;

				// 正式停车
				status.put(lot, car);
				records.add(new Record(car, lot));
			}
		}
		// num并不是合法车位
		if (!legal)
			throw new Exception();

		checkRep();
	}

	@Override
	public void parking(String plate, int width) {
		// TODO Auto-generated method stub
		checkRep();
	}

	@Override
	public double depart(String plate) throws Exception {
		// TODO Auto-generated method stub
		checkRep();
		return 0;
	}

	@Override
	public Map<Integer, String> status() {
		Map<Integer, String> st = new HashMap<>();
		for (Lot lot : lots) {
			Car c = status.get(lot);
			if (c == null)
				st.put(lot.getNumber(), "");
			else
				st.put(lot.getNumber(), status.get(lot).getPlate());
		}
		checkRep();
		return st;
	}

	@Override
	public int getNumberOfLots() {
		return lots.size();
	}

	@Override
	public boolean isLotInParkingField(int num, int width) {
		for (Lot lot : lots) {
			if (lot.getNumber() == num && lot.getWidth() == width)
				return true;
		}

		return false;
	}

	@Override
	public boolean isEmpty() {
		return status.isEmpty();
	}

	@Override
	public int getLotWidth(int num) throws Exception {
		for (Lot lot : lots) {
			if (lot.getNumber() == num)
				return lot.getWidth();
		}

		throw new Exception("No such lot " + num);
	}

	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("The parking field has total number of lots: " + this.getNumberOfLots() + "\n");

		NumberFormat numberFormat = NumberFormat.getInstance();
		numberFormat.setMaximumFractionDigits(4);
		String ratio = numberFormat.format((double) this.status.size() / (double) this.getNumberOfLots() * 100);

		sb.append("Now " + ratio + "% lots are occupied" + "\n");
		for (Lot lot : lots) {
			if (status.get(lot) != null)
				sb.append("Lot " + lot.getNumber() + " (" + lot.getWidth() + "):\tCar " + status.get(lot).getPlate()
						+ "\n");
			else
				sb.append("Lot " + lot.getNumber() + " (" + lot.getWidth() + "):\tFree\n");

		}
		return sb.toString();
	}

}
