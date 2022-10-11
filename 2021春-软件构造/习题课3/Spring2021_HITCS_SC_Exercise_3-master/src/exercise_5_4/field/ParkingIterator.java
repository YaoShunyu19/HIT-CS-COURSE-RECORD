package exercise_5_4.field;

import java.util.Collections;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import exercise_5_4.parkable.Parkable;

public class ParkingIterator implements Iterator<String> {

	private final Map<Lot, Parkable> parkings;
	private final List<Lot> occupiedLots;
	private int count = 0;

	public ParkingIterator(Map<Lot, Parkable> parkings) {
		this.parkings = parkings;
		this.occupiedLots = new LinkedList<>(parkings.keySet());
		Collections.sort(this.occupiedLots);
	}

	@Override
	public boolean hasNext() {
		return count < occupiedLots.size();
	}

	@Override
	public String next() {
		Lot l = occupiedLots.get(count);
		count ++;
		return parkings.get(l).toString();
	}

	@Override
	public void remove() {
		throw new UnsupportedOperationException();
	}

}
