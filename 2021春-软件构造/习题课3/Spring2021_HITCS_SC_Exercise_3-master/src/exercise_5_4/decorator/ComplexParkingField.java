package exercise_5_4.decorator;

import java.util.Iterator;
import java.util.Map;

import exercise_5_4.field.ParkingField;
import exercise_5_4.strategy.ParkingStrategy;
import exercise_5_4.visitor.ParkingVisitor;


public class ComplexParkingField implements ParkingField {

	protected final ParkingField pf;

	public ComplexParkingField(ParkingField pf) {
		assert pf != null;
		
		this.pf = pf;
	}

	@Override
	public void parking(String type, String plate, int width, int num, String[] extraRegistrationInfo)
			throws Exception {
		pf.parking(type, plate, width, num, extraRegistrationInfo);
	}

	@Override
	public void parking(String type, String plate, int width, String[] extraRegistrationInfo) {
		pf.parking(type, plate, width, extraRegistrationInfo);		
	}

	@Override
	public double depart(String plate) throws Exception {
		return pf.depart(plate);
	}

	@Override
	public Map<Integer, String> status() {
		return pf.status();
	}

	@Override
	public int getNumberOfLots() {
		return pf.getNumberOfLots();
	}

	@Override
	public boolean isLotInParkingField(int num, int width) {
		return pf.isLotInParkingField(num, width);
	}

	@Override
	public boolean isEmpty() {
		return pf.isEmpty();
	}

	@Override
	public int getLotWidth(int num) throws Exception {
		return pf.getLotWidth(num);
	}

	@Override
	public double accept(ParkingVisitor pv) {
		return pf.accept(pv);
	}

	@Override
	public Iterator<String> iterator() {
		return pf.iterator();
	}

	@Override
	public void setParkingStrategy(ParkingStrategy ps) {
		pf.setParkingStrategy(ps);		
	}

	@Override
	public String toString() {
		return pf.toString();
	}
}
