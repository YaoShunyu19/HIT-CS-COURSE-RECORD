package exercise_5_4.strategy;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Random;

import exercise_5_4.field.Lot;
import exercise_5_4.parkable.Parkable;


public class RandomParkingStrategy implements ParkingStrategy {

	@Override
	public Lot selectLot(List<Lot> lots, Map<Lot, Parkable> status, Parkable c)
			throws Exception {
		
		List<Lot> freeLots = new ArrayList<>();
		for (Lot lot : lots) {
			if (!status.containsKey(lot) && lot.getWidth() >= c.getWidth())
				freeLots.add(lot);
		}
		int num = freeLots.size();
		if (num > 1) {
			int i = new Random().nextInt(num - 1);
			return freeLots.get(i);
		}
		else if(num == 1)
			return freeLots.get(0);
		
		throw new Exception();
	}
}
